// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>

// Any header files included below this line should have been created by you

#include "auxiliar.h"
#include "macros.h"
#include "i8254.h"

int hook_id;
extern uint8_t byte;
extern int cont;
extern int declive;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need/ it]
  lcf_trace_calls("/home/lcom/labs/lab4/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab4/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}


int (mouse_test_packet)(uint32_t cnt) {

  int ipc_status;
  message msg;
  uint32_t r, irq_set_mouse = 4;
  uint8_t bit_no = 2;
  bool segundo = false, terceiro = false;
  struct packet a;


  if(enable_reporting() != 0)
    return 1;

  if(subscribe_mouse(&bit_no) != 0)
    return 1;

  uint8_t x_sign, y_sign;

  while( cnt != 0 ) { /* You may want to use a different condition */
      /* Get a request message. */
      if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
          printf("driver_receive failed with: %d", r);
          continue;
      }
      if (is_ipc_notify(ipc_status)) { /* received notification */
          switch (_ENDPOINT_P(msg.m_source)) {
              case HARDWARE: /* hardware interrupt notification */				
                  if (msg.m_notify.interrupts & irq_set_mouse) { /* subscribed interrupt */
                      mouse_ih();
                      if((byte & 0x08) != 0  && segundo == false && terceiro == false){
                        segundo = true;
                        a.bytes[0] = byte;
                        a.rb = byte & 0x02;
                        a.lb = byte & 0x01;
                        a.mb = byte & 0x04;
                        a.y_ov = byte & 0x80;
                        a.x_ov = byte & 0x40; 
                        x_sign = byte & 0x10;
                        y_sign = byte & 0x20;
                      }
                      else if(segundo == true){
                        terceiro = true;
                        segundo = false;
                        a.bytes[1] = byte;
                        if(x_sign == 16)
                          a.delta_x = byte - 256;
                        else
                          a.delta_x =  byte;
                      }
                      else if(terceiro == true){
                        terceiro = false;
                        a.bytes[2] = byte;
                        if(y_sign == 32)
                          a.delta_y = byte - 256;
                        else
                          a.delta_y =  byte;
                        cnt--;
                        mouse_print_packet(&a);
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

  if(unsubscribe_mouse() != 0)
    return 1;

  reset_mouse1();

  return 0;
}

int (mouse_test_remote)(uint16_t period, uint8_t cnt) {
  struct packet a;
  uint8_t x_sign, y_sign;
  uint8_t bit_no = 2;

  if(subscribe_mouse(&bit_no) != 0)    //subscreve para depois desativar
    return 1;

  sys_irqdisable(&hook_id); //desativa interrupcoes

  remote_mouse();   //ativa modo remoto

  while(cnt != 0){
    read_data();
    mouse_ih();
    if(byte != 0){
      if((byte & 0x08) != 0){
        a.bytes[0] = byte;
        a.rb = byte & 0x02;
        a.lb = byte & 0x01;
        a.mb = byte & 0x04;
        a.y_ov = byte & 0x80;
        a.x_ov = byte & 0x40; 
        x_sign = byte & 0x10;
        y_sign = byte & 0x20;
        tickdelay(micros_to_ticks(DELAY_US));
        //----------------------------------
        mouse_ih();
        a.bytes[1] = byte;
        if(x_sign == 16)
          a.delta_x = byte - 256;
        else
          a.delta_x =  byte;

        tickdelay(micros_to_ticks(DELAY_US));
        //---------------------------------
        mouse_ih();
        a.bytes[2] = byte;
        if(y_sign == 32)
          a.delta_y = byte - 256;
        else
          a.delta_y =  byte;
        //---------------------------------
        cnt--;
        mouse_print_packet(&a);
        tickdelay(micros_to_ticks(period*1000));
      }

    }
  }
  
  reset_mouse2();

  return 0;
}

int (mouse_test_async)(uint8_t idle_time) {
  int ipc_status;
  message msg;
  uint32_t r, irq_set_mouse = 4, irq_set_timer = 1;
  uint8_t bit_no = 2, bit_noT = 0;
  bool segundo = false, terceiro = false;
  struct packet a;

  uint8_t n = idle_time;

  if(timer_subscribe_int(&bit_noT)!= 0)
    return 1;

  if(enable_reporting() != 0)
    return 1;

  if(subscribe_mouse(&bit_no) != 0)
    return 1;

  uint8_t x_sign, y_sign;

  while( n != 0 ) { /* You may want to use a different condition */
      /* Get a request message. */
      if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
          printf("driver_receive failed with: %d", r);
          continue;
      }
      if (is_ipc_notify(ipc_status)) { /* received notification */
          switch (_ENDPOINT_P(msg.m_source)) {
              case HARDWARE: /* hardware interrupt notification */
                  if ((msg.m_notify.interrupts & irq_set_timer)) { /* subscribed interrupt */
                    timer_int_handler();
                    if(cont % sys_hz() == 0){
                      timer_print_elapsed_time();
                      n--;
                    }
                  }				
                  if (msg.m_notify.interrupts & irq_set_mouse) { /* subscribed interrupt */
                      n = idle_time;
                      cont = 0;
                      mouse_ih();
                      if((byte & 0x08) != 0  && segundo == false && terceiro == false){
                        segundo = true;
                        a.bytes[0] = byte;
                        a.rb = byte & 0x02;
                        a.lb = byte & 0x01;
                        a.mb = byte & 0x04;
                        a.y_ov = byte & 0x80;
                        a.x_ov = byte & 0x40; 
                        x_sign = byte & 0x10;
                        y_sign = byte & 0x20;
                      }
                      else if(segundo == true){
                        terceiro = true;
                        segundo = false;
                        a.bytes[1] = byte;
                        if(x_sign == 16)
                          a.delta_x = byte - 256;
                        else
                          a.delta_x =  byte;
                      }
                      else if(terceiro == true){
                        terceiro = false;
                        a.bytes[2] = byte;
                        if(y_sign == 32)
                          a.delta_y = byte - 256;
                        else
                          a.delta_y =  byte;
                        mouse_print_packet(&a);
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

  if(unsubscribe_mouse() != 0)
    return 1;

  reset_mouse1();

  if(timer_unsubscribe_int() != 0)
    return 1;

  return 0;
}

int (mouse_test_gesture)(uint8_t x_len, uint8_t tolerance) {
    
  int ipc_status;
  message msg;
  uint32_t r, irq_set_mouse = 4;
  uint8_t bit_no = 2;
  bool segundo = false, terceiro = false;
  struct packet a;
  enum Estado estado = NE; //pode ser INI->estado quando estamos a desenhar a linha para cima, MD-> estado em que inverte vai para baixo, FI-> quando estamos na fase do desenho da linha para baixo, NE->nao esta en nenhum daqueles, FEITO-> estado quando o desenho e completo

  if(enable_reporting() != 0)
    return 1;


  if(subscribe_mouse(&bit_no) != 0)
    return 1;

  uint8_t x_sign, y_sign;

  while( estado != FEITO ) { /* You may want to use a different condition */
      /* Get a request message. */
      if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
          printf("driver_receive failed with: %d", r);
          continue;
      }
      if (is_ipc_notify(ipc_status)) { /* received notification */
          switch (_ENDPOINT_P(msg.m_source)) {
              case HARDWARE: /* hardware interrupt notification */		
                  if (msg.m_notify.interrupts & irq_set_mouse) { /* subscribed interrupt */
                      mouse_ih();
                      if((byte & 0x08) != 0  && segundo == false && terceiro == false){
                        segundo = true;
                        a.bytes[0] = byte;
                        a.rb = byte & 0x02;
                        a.lb = byte & 0x01;
                        a.mb = byte & 0x04;
                        a.y_ov = byte & 0x80;
                        a.x_ov = byte & 0x40; 
                        x_sign = byte & 0x10;
                        y_sign = byte & 0x20;
                      }
                      else if(segundo == true){
                        terceiro = true;
                        segundo = false;
                        a.bytes[1] = byte;
                        if(x_sign == 16)
                          a.delta_x = byte - 256;
                        else
                          a.delta_x =  byte;
                      }
                      else if(terceiro == true){
                        terceiro = false;
                        a.bytes[2] = byte;
                        if(y_sign == 32)
                          a.delta_y = byte - 256;
                        else
                          a.delta_y =  byte;
                        mouse_print_packet(&a);
                        processa_estado(&estado, a, tolerance, x_len);
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

  if(unsubscribe_mouse() != 0)
    return 1;

  reset_mouse1();

  return 0;
}
