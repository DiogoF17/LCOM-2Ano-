#include "auxi.h"
#include "videocard_macros.h"
#include <minix/driver.h>
#include <sys/mman.h>
#include <lcom/lcf.h>
#include <math.h>

static void *video_mem;

static vbe_mode_info_t info;

uint16_t bits;

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

void get_vbe_mode_info(uint16_t mode, vbe_mode_info_t *info){
  struct reg86 r86;
  
  memset(&r86, 0, sizeof(r86));	/* zero the structure */


  r86.intno = BIOS; /* BIOS video services */
  r86.ah = 0x01;    
  r86.al = mode;  
  //r86.ax = 0x4F02;  
  r86.bx = BIT14 | mode;  

  sys_int86(&r86);

}

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

unsigned (getXRes)(){
  return info.XResolution;
}

unsigned (getYRes)(){
  return info.YResolution;
}

unsigned (getBitsPixel)(){
  return info.BitsPerPixel;
}

unsigned (getBlueSize)(){
  return info.BlueMaskSize;
}

unsigned (getRedSize)(){
  return info.RedMaskSize;
}

unsigned (getGreenSize)(){
  return info.GreenMaskSize;
}

unsigned (getBluePos)(){
  return info.BlueFieldPosition;
}

unsigned (getRedPos)(){
  return info.RedFieldPosition;
}

unsigned (getGreenPos)(){
  return info.GreenFieldPosition;
}

void parseColor(uint32_t *color){
  uint32_t aux = *color;
  int redpos = getRedPos(), redsi = getRedSize(), greenpos = getGreenPos(), greensi = getGreenSize(), bluepos = getBluePos(), bluesi = getBlueSize();
  uint32_t red = ((aux >> redpos) << (32 -redsi)) >> (32 - redsi);
  aux=*color;
  uint32_t green =((aux >> greenpos) << (32 -greensi)) >> (32-greensi) ;
  aux=*color;
  uint32_t blue = ((aux >>bluepos) << (32 -bluesi)) >> (32-bluesi);
  *color = (red << redpos) | (green << greenpos) | (blue << bluepos);
}

int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color){
  int fator = ceil(info.BitsPerPixel / 8.0);
  uint8_t *end;
  for(int i = 0; i < len; i++){
    end = ((uint8_t *)video_mem + ( y * info.XResolution + x + i) * fator);
    if(bits == 8)
      *end = color;
    else{
      for(int j = 0; j < fator; j++)
        end[j] = (uint8_t) ((color >> (8*j)) & 0xFF);
    } 
  }
  return 0;
}

int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color){
  for(int i = 0; i < height; i++)
    vg_draw_hline(x, y + i, width, color);
    return 0;
}


void (draw_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y){
  xpm_image_t aux1;
  uint8_t *aux;
  aux= xpm_load(xpm, XPM_INDEXED, &aux1);
  uint8_t *end;
  for(int i = 0; i < aux1.height; i++){
    for(int j = 0; j < aux1.width; j++){
      end = (uint8_t *)video_mem + ( (y+i) * info.XResolution +(x+j));
      *end= *aux;
      aux++;
    }
  }  
}

void (clean_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y){
  xpm_image_t aux1;
  uint8_t *aux;
  aux= xpm_load(xpm, XPM_INDEXED, &aux1);
  uint8_t *end;
  for(int i = 0; i < aux1.height; i++){
    for(int j = 0; j < aux1.width; j++){
      end = (uint8_t *)video_mem + ( (y+i) * info.XResolution +(x+j));
      *end= 0;
    }
  }  
}

