// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <lcom/lab5.h>

#include <stdint.h>
#include <stdio.h>

#include "auxi.h"
#include "_aux.h"
#include "videocard_macros.h"
#include "macros.h"
#include <lcom/lab3.h>
#include "game.h"

// Any header files included below this line should have been created by you

uint32_t hook_idK;
uint8_t scancode;
extern uint16_t bits;
int hook_idT;       //contem o identificador proveniente do hook_id
extern int cont;       //usada como contador global

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab5/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab5/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(video_test_init)(uint16_t mode, uint8_t delay) {

  //start_game();
  
  if(game()!=0)
    return 1;

  return 0;
}

int(video_test_rectangle)(uint16_t mode, uint16_t x, uint16_t y,
                          uint16_t width, uint16_t height, uint32_t color) {
  int ipc_status;
  message msg;
  uint32_t r, irq_set_kbc = 2;
  uint8_t bit_no = 1;

  if(graphic_mode(mode) != 0){
    vg_exit();
    return 1;
  }

  if (bits!=8)
    parseColor(&color);
  
  for(int i = 0; i < height; i++)
    vg_draw_hline(x, y + i, width, color);
     
  if(kbc_subscribe(&bit_no) != 0)
    return 1;

  while( scancode != ESC_BREAKCODE ) { /* You may want to use a different condition */
      /* Get a request message. */
      if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
          printf("driver_receive failed with: %d", r);
          continue;
      }
      if (is_ipc_notify(ipc_status)) { /* received notification */
          switch (_ENDPOINT_P(msg.m_source)) {
              case HARDWARE: /* hardware interrupt notification */				
                  if (msg.m_notify.interrupts & irq_set_kbc) { /* subscribed interrupt */
                      kbc_ih();
                  }
                  break;
              default:
                  break; /* no other notifications expected: do nothing */	
          }
      } else { /* received a standard message, not a notification */
          /* no standard messages expected: do nothing */
      }
  
  }

  if(kbc_unsubscribe()!=0)
    return 1;

  if(vg_exit() != 0)
    return 1;
  
  return 0;
}

int(video_test_pattern)(uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step) {
  int ipc_status;
  message msg;
  uint32_t r, irq_set_kbc = 2;
  uint8_t bit_no = 1;

  if(graphic_mode(mode) != 0){
    vg_exit();
    return 1;
  }

  unsigned xRes = getXRes();
  unsigned yRes = getYRes();

  uint16_t width = xRes / no_rectangles;
  uint16_t height = yRes /no_rectangles;
  uint32_t color, R, G, B, Rfi, Gfi, Bfi,aux=first;
  uint16_t x = 0, y = 0;
  for(int j = 0; j < no_rectangles; j++){
    for(int i = 0; i < no_rectangles; i++){
      if(mode == 0x105)
        color = (first + (j * no_rectangles + i) * step) % (1 << getBitsPixel());
      else{
          Rfi = ((aux >> getRedPos()) << (32 - getRedSize())) >> (32 - getRedSize());
          aux=first;
          Gfi = ((aux >> getGreenPos()) << (32 - getGreenSize())) >> (32 - getGreenSize());
          aux=first;
          Bfi = ((aux >> getBluePos()) << (32 - getBlueSize())) >> (32 - getBlueSize());
          R = (Rfi + i * step) % (1 << getRedSize());
          G = (Gfi+ j * step) % (1 << getGreenSize());
          B = (Bfi+ (i + j) * step) % ( 1 << getBlueSize());
          color = (R<< getRedPos()) | (G << getGreenPos()) | (B << getBluePos());
      }
      vg_draw_rectangle(x, y, width, height, color);
      x += width;
    }
    x = 0;
    y += height;
  }

  if(kbc_subscribe(&bit_no) != 0)
    return 1;

  while( scancode != ESC_BREAKCODE ) { /* You may want to use a different condition */
      /* Get a request message. */
      if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
          printf("driver_receive failed with: %d", r);
          continue;
      }
      if (is_ipc_notify(ipc_status)) { /* received notification */
          switch (_ENDPOINT_P(msg.m_source)) {
              case HARDWARE: /* hardware interrupt notification */				
                  if (msg.m_notify.interrupts & irq_set_kbc) { /* subscribed interrupt */
                      kbc_ih();
                  }
                  break;
              default:
                  break; /* no other notifications expected: do nothing */	
          }
      } else { /* received a standard message, not a notification */
          /* no standard messages expected: do nothing */
      }
  
  }

  if(kbc_unsubscribe()!=0)
    return 1;

  if(vg_exit() != 0)
    return 1;

  return 0;
}

int(video_test_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y) {
   int ipc_status;
  message msg;
  uint32_t r, irq_set_kbc = 2;
  uint8_t bit_no = 1;

  if(graphic_mode(0x105) != 0){
    vg_exit();
    return 1;
  }

  draw_xpm(xpm,x, y);
 
  if(kbc_subscribe(&bit_no) != 0)
    return 1;

  while( scancode != ESC_BREAKCODE ) { /* You may want to use a different condition */
      /* Get a request message. */
      if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
          printf("driver_receive failed with: %d", r);
          continue;
      }
      if (is_ipc_notify(ipc_status)) { /* received notification */
          switch (_ENDPOINT_P(msg.m_source)) {
              case HARDWARE: /* hardware interrupt notification */				
                  if (msg.m_notify.interrupts & irq_set_kbc) { /* subscribed interrupt */
                      kbc_ih();
                  }
                  break;
              default:
                  break; /* no other notifications expected: do nothing */	
          }
      } else { /* received a standard message, not a notification */
          /* no standard messages expected: do nothing */
      }
  
  }

  if(kbc_unsubscribe()!=0)
    return 1;

  if(vg_exit() != 0)
    return 1;

  return 0;
}

int(video_test_move)(xpm_map_t xpm, uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf,
                     int16_t speed, uint8_t fr_rate) {
  int ipc_status;
  message msg;
  uint32_t r, irq_set_kbc = 2, irq_set_timer = 1;
  uint8_t bit_no = 1, bit_noT = 0;
  
  int dx, dy;

  bool dif; //diz se as coordenadas iniciais sao diferentes

  if(speed == 0)
    return 1;

  if(speed > 0){

    if(xi == xf){
      dx = 0;
      if(yi < yf)
        dy = speed; 
      else
        dy = -speed;
    }
    else{
      dy = 0;
      if(xi < xf)
        dx = speed; 
      else
        dx = -speed;
    }
  }
  else{
    if(xi == xf){
      dx = 0;
      if(yi < yf)
        dy = 1; 
      else
        dy = -1;
    }
    else{
      dy = 0;
      if(xi < xf)
        dx = 1; 
      else
        dx = -1;
    }
  }

  uint16_t x = xi, y = yi;

  if(graphic_mode(0x105) != 0){
    vg_exit();
    return 1;
  }
  if(timer_subscribe_int(&bit_noT) != 0)
    return 1;
 
  if(kbc_subscribe(&bit_no) != 0)
    return 1;
  
  int num_frames = 60 / fr_rate;

  draw_xpm(xpm, xi, yi);    //inicia o desenho 

  bool fim = false; //diz se ja chegou as coordenadas finais

  int cont_frames = 0;   //conta o numero de frames

  if(xi == xf && yi == yf){
    dif = false;
    fim = true;
  }
  else
    dif = true;

  while( scancode != ESC_BREAKCODE ) { /* You may want to use a different condition */
      /* Get a request message. */
      if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
          printf("driver_receive failed with: %d", r);
          continue;
      }
      if (is_ipc_notify(ipc_status)) { /* received notification */
          switch (_ENDPOINT_P(msg.m_source)) {
              case HARDWARE: /* hardware interrupt notification */				
                  if (msg.m_notify.interrupts & irq_set_kbc) { /* subscribed interrupt */ 
                      kbc_ih();
                  }
                  if((msg.m_notify.interrupts & irq_set_timer)){
                      timer_int_handler();
                      if(fim && dif)
                        continue;
                      if(cont % num_frames == 0){
                        if(speed > 0){
                          clean_xpm(xpm,x, y); //mete a cor preto na posicao anterior
                          x += dx;
                          y += dy;
                          if(x >= xf && xi != xf){
                            x = xf;
                            fim = true;
                          }
                          if(y >= yf && yi != yf){
                            y = yf;
                            fim = true;
                          }
                          draw_xpm(xpm, x, y);
                        }
                        else{
                            if(abs(speed) == cont_frames){
                              clean_xpm(xpm,x, y); //mete a cor preto na posicao anterior
                              x += dx;
                              y += dy;
                              if(x >= xf && xi != xf){
                                x = xf;
                                fim = true;
                              }
                              if(y >= yf && yi != yf){
                                y = yf;
                                fim = true;
                              }
                              draw_xpm(xpm, x, y);
                              cont_frames = -1;
                            }
                            cont_frames++;
                        }
                    }
                  }
                  break;
              default:
                  break; /* no other notifications expected: do nothing */	
          }
      } else { /* received a standard message, not a notification */
          /* no standard messages expected: do nothing */
      }
  
  }

  if(timer_unsubscribe_int() != 0)
    return 1;

  if(kbc_unsubscribe()!=0)
    return 1;

  if(vg_exit() != 0)
    return 1;

  return 1;
}

int(video_test_controller)() {
  


  return 1;
}
