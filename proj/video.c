#include "video.h"
#include "videocard_macros.h"
#include <minix/driver.h>
#include <sys/mman.h>
#include <lcom/lcf.h>
#include <math.h>

static void *video_mem; 

static vbe_mode_info_t info;

int tam;   //tamanho de memoria necessaria

extern char* video_buffer;  //enderecos da memoria auxiliar para o double buffering.

uint16_t bits; //

//-------------------------------------------
int (graphic_mode)(uint16_t mode){

  if(read_adress(mode) != 0)
    return 1;

  struct reg86 r86;
  
  memset(&r86, 0, sizeof(r86));	/* zero the structure */


  r86.intno = BIOS; /* BIOS video services */
  r86.ah = VBE_FUNC;    
  r86.al = VD_MODE;  
  //r86.ax = 0x4F02;  
  r86.bx = BIT14 | mode;  

  if( sys_int86(&r86) != OK ) 
    return 1;

  return 0;
}

//-----------------------------------------------------------
int (read_adress)(uint16_t mode){
  //bool enable_logging = true;
  struct minix_mem_range mr;
  int r;	

  mr.mr_base = 0;	
  mr.mr_limit = 1024 * 1024;  

  if( OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
    panic("sys_privctl (ADD_MEM) failed: %d\n", r);

  //lm_init(enable_logging);

  memset(&info, 0, sizeof(info));

  if(vbe_get_mode_info(mode, &info)!=0)
    return 1;

  unsigned int vram_base = info.PhysBasePtr;  /* VRAM's physical addresss */
  unsigned int vram_size = info.XResolution * info.YResolution * ceil(info.BitsPerPixel / 8.0);  /* VRAM's size, but you can use the frame-buffer size, instead */			    
  tam = vram_size;
  /* Allow memory mapping */

  mr.mr_base = (phys_bytes) vram_base;	
  mr.mr_limit = mr.mr_base + vram_size;  

  if( OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
    panic("sys_privctl (ADD_MEM) failed: %d\n", r);

  /* Map memory */

  video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);

  if(video_mem == MAP_FAILED)
    panic("couldn't map video memory");

  bits = info.BitsPerPixel;

  return 0;
}

void draw_mouse(uint8_t* aux, uint16_t x, uint16_t y, int alt, int cmp){
  uint8_t *end;
  for(int i = 0; i < alt; i++){
    for(int j = 0; j < cmp; j++){
      end = (uint8_t *)video_buffer + ( (y+i) * info.XResolution +(x+j));
      if(*aux != 63)
        *end= *aux;
      aux++;
    }
  }
}

void (draw_xpm)(uint8_t* aux, uint16_t x, uint16_t y, int alt, int cmp){
  uint8_t *end;
  for(int i = 0; i < alt; i++){
    for(int j = 0; j < cmp; j++){
      end = (uint8_t *)video_buffer + ( (y+i) * info.XResolution +(x+j));
      *end= *aux;
      aux++;
    }
  }
}

void (clean_xpm)(uint8_t* aux, uint16_t x, uint16_t y, int alt, int cmp){
  uint8_t *end;
  for(int i = 0; i < alt; i++){
    for(int j = 0; j < cmp; j++){
      end = (uint8_t *)video_buffer + ( (y+i) * info.XResolution +(x+j));
      *end= 0;
    }
  }  
}

void *copia_buffer(){
  return memcpy(video_mem, video_buffer, tam);
}

