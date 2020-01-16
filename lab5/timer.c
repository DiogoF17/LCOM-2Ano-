#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

extern int hook_idT;       //contem o identificador proveniente do hook_id
int cont = 0;       //usada como contador global

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  uint32_t temp;            //contem o read back command
  uint8_t res, st;        //res -> valor a escrever em 8 bits
  uint32_t resu = TIMER_FREQ / freq;    //valor a escrever no timer
  uint16_t valor = (uint16_t) resu;    
  if(timer == 0){
    timer_get_conf(TIMER_2, &st);  //obtem a configuracao do timer
    temp = 0x0000000F & st;      //mantem os 4 bits menos significativos
    temp = temp | 0x00000030;     //escreve os 4 bits mais significaticos
    //verifica se teve sucesso ao invocar a funcao
    if(sys_outb(TIMER_CTRL, temp) != 0)
      return 1;
    //verifica se teve sucesso ao invocar a funcao
    if(util_get_LSB(valor, &res) != 0)
      return 1;
    //verifica se teve sucesso ao invocar a funcao
    if (sys_outb(TIMER_0, res)!= 0)    //escreve no timer os 8 bits menos significativos
      return 1;      
    //verifica se teve sucesso ao invocar a funcao
    if(util_get_MSB(valor, &res) != 0)
      return 1;
    //verifica se teve sucesso ao invocar a funcao
    if(sys_outb(TIMER_0, res) != 0)    //escreve no timer os 8 bits mais significativos
      return 1;
    return 0;
  }
  else if(timer == 1){
    timer_get_conf(TIMER_2, &st);  //obtem a configuracao do timer
    temp = 0x0000000F & st;      //mantem os 4 bits menos significativos
    temp = temp | 0x00000070;     //escreve os 4 bits mais significaticos
    //verifica se teve sucesso ao invocar a funcao
    if(sys_outb(TIMER_CTRL, temp) != 0)
      return 1;
    //verifica se teve sucesso ao invocar a funcao
    if(util_get_LSB(valor, &res) != 0)
      return 1;
    //verifica se teve sucesso ao invocar a funcao
    if (sys_outb(TIMER_0, res)!= 0)    //escreve no timer os 8 bits menos significativos
      return 1;      
    //verifica se teve sucesso ao invocar a funcao
    if(util_get_MSB(valor, &res) != 0)
      return 1;
    //verifica se teve sucesso ao invocar a funcao
    if(sys_outb(TIMER_0, res) != 0)    //escreve no timer os 8 bits mais significativos
      return 1;
    return 0;
  }
  else{
    timer_get_conf(TIMER_2, &st);  //obtem a configuracao do timer
    temp = 0x0000000F & st;      //mantem os 4 bits menos significativos
    temp = temp | 0x000000B0;     //escreve os 4 bits mais significaticos
    sys_outb(TIMER_CTRL, temp);
    util_get_LSB(valor, &res);
    sys_outb(TIMER_2, res);           //escreve no timer os 8 bits menos significativos
    util_get_MSB(valor, &res);        
    sys_outb(TIMER_2, res);   //escreve no timer os 8 bits mais significativos
    return 0;
  }
 
  return 1;
}

int (timer_subscribe_int)(uint8_t *bit_no) {

  //veirifica se nao esta a apontar para nada
  if(bit_no == NULL)
    return 1;

  hook_idT = *bit_no;
  if(sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_idT) != OK)
    return 1;

  return 0;
}

int (timer_unsubscribe_int)() {
  sys_irqrmpolicy(&hook_idT);

  return 0;
}

void (timer_int_handler)() {
 cont++;
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  
  //veirifica se nao esta a apontar para nada
  if(st == NULL)
    return 1;

  if(timer == 0){
    
    //veirifica se invocou a funcao com sucesso
    if(util_sys_inb(TIMER_0, st) != 0)
      return 1;
    return 0;
  }
  else if(timer == 1){
    
    //veirifica se invocou a funcao com sucesso
    if(util_sys_inb(TIMER_1,st) != 0)
      return 1;
    return 0;
  }
  else{

    //veirifica se invocou a funcao com sucesso
    if(util_sys_inb(TIMER_2, st) != 0)
      return 1;
    return 0;
  }  

  return 1;
}

int (timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field) {
  uint8_t aux;
  union timer_status_field_val u;

  if(field == 3){
    aux = st & 0x01;
    if(aux == 0)  
       u.bcd = false;
    else
       u.bcd = true;
  }
  
 else if(field == 2){
  aux = st & 0x0E;

  switch(aux){
    case 0:
      u.count_mode = 0;
      break;
    case 4:
    case 12:
      u.count_mode = 2;
      break;
    case 2:
      u.count_mode = 1;
      break;
    case 6:
    case 14:
      u.count_mode = 3;
      break;
    case 8:
      u.count_mode = 4;
      break;
    case 10:
      u.count_mode = 5;
      break;
  }
}
  else if(field == 1){

  aux = st & 0x30;

  switch(aux){
    case 0:
      u.in_mode = INVAL_val;
      break;
    case 16:
      u.in_mode = LSB_only;
      break;
    case 32:
      u.in_mode = MSB_only;
      break;
    case 48:
      u.in_mode = MSB_after_LSB;
      break;
  }
  }
  else if(field == 0)
    u.byte = st;

  timer_print_config(timer, field, u);
 
  return 1;
}
