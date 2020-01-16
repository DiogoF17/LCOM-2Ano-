#include <lcom/lcf.h>

#include <stdint.h>

#include "i8254.h"

int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
  uint16_t temp;

  //veirifica se nao esta a apontar para nada
  if(lsb == NULL)
    return 1;

  temp = val << 8;   //desloca 8 bits para a esquerda o valor
  temp = temp >> 8; //desloca 8 bits para a direita o valor
  (*lsb) = (uint8_t) temp;

  return 1;
}

int(util_get_MSB)(uint16_t val, uint8_t *msb) {
  uint16_t temp;

  //veirifica se nao esta a apontar para nada
   if(msb == NULL)
    return 1;

  temp = val >> 8;    //desloca 8 bits para a direita o valor
  (*msb) = (uint8_t) temp;

  return 1;
}

int(util_sys_inb)(int port, uint8_t *value) {
  uint32_t temp;

  //verifica se nao esta a apontar para nada
  if(value == NULL)
    return 1;
  
  //escreve o read back command de acordo com o timer
  if(port == TIMER_0)
    temp=0x000000E2;
  else if(port == TIMER_1)
    temp=0x000000E4;
  else
    temp=0x000000E8;
  
  sys_outb(TIMER_CTRL, temp);
  
  sys_inb(port, &temp);
  
  (*value) = (uint8_t) temp;
  
  return 0;
}
