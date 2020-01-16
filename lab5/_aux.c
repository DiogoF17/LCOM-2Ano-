#include <lcom/lcf.h>

#include <lcom/lab3.h>

#include <stdbool.h>
#include <stdint.h>

#include "macros.h"

extern int hook_idK;
extern uint8_t scancode; 
int cont1 = 0;

#ifdef LAB3
int cont = 0;

void counter(){
  cont++;
}
#endif

void(kbc_ih)(){
  uint32_t aux;  //variavel auxiliar
  uint32_t aux1 = 0xC0;    //vai ser usada para verficar se ocorreu algum erro 
  
  scancode = 0;     //inicializa com 0 para o caso de nao ler

  sys_inb(STATUS_REGISTER, &aux);//le o Status Register
  #ifdef LAB3
  counter();
  #endif

  //verifica se o bit 7 ou 6 do status register esta a 1 se sim e porque houve erro logo nao executa mais
  if((aux & aux1) != 0){
    return; //nao faz nada
  }

  //verifica se tem algo para ler do output buffer
  aux1 = 0x01;   //usada para verificar se output buffer esta cheio
  if((aux & aux1) == 0){//nao tem nada para ler
    return; //nao faz nada  
  }
  
  sys_inb(OUTPUT_BUFFER, &aux);
  #ifdef LAB3
  counter();
  #endif
  scancode = (uint8_t) (aux); 
  
}

int (kbc_subscribe)(uint8_t *bit_no){
  if(bit_no == NULL)   //veirifica se nao esta a apontar para nada
    return 1;

  hook_idK = *bit_no;
  
  if(sys_irqsetpolicy(IRQ1, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_idK) != OK)
    return 1;

  return 0;
}

int (kbc_unsubscribe)(){
  if(sys_irqrmpolicy(&hook_idK) != 0)
    return 1;
  return 0;
}

void(kbc_Enable)(){
  uint32_t aux;
  sys_outb(STATUS_REGISTER, READ_CMD_BYTE);
  sys_inb(OUTPUT_BUFFER, &aux);
  
  #ifdef LAB3
  counter();
  #endif

  aux = aux | 0x01;

  sys_outb(STATUS_REGISTER, WRITE_CMD_BYTE);
  sys_outb(INPUT_BUFFER, aux);
}



