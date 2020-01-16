#include "mouse.h"
#include "macros.h"

extern int hook_id;
uint8_t byte;

//----------------------------------------------------------
int (subscribe_mouse)(uint8_t *bit_no){
  if(bit_no == NULL)
    return 1;
  
  hook_id = *bit_no;

  if(sys_irqsetpolicy(IRQ12, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id) != OK)
    return 1;

  return 0;
}

int (unsubscribe_mouse)(){
  if(sys_irqrmpolicy(&hook_id) != 0)
    return 1;

  return 0;
}

//------------------------------------------------
int (inputBufferFull)(){
  uint32_t aux;

  if(sys_inb(STATUS_REGISTER, &aux) != 0)
    return 0;

  //verifica se ocorreu algum erro na leitura 
  if((aux & 0xC0000000) != 0)
    return 0;

  //verifica se o input buffer esta cheio
  if((aux & 0x02) == 0)
    return 0;
  
  return 1;
}

int (outputBufferFull)(){
  uint32_t aux;

  if(sys_inb(STATUS_REGISTER, &aux) != 0)
    return 1;

  //verifica se ocorreu algum erro na leitura 
  if((aux & 0xC0000000) != 0)
    return 1;

  //verifica se o output buffer esta cheio
  if((aux & 0x01) == 0)
    return 1;

  return 0;

}

//--------------------------------------------------------
void (mouse_ih)(){

  byte = 0;

  uint32_t aux;

 while(outputBufferFull())
    tickdelay(micros_to_ticks(DELAY_US));

  sys_inb(OUTPUT_BUFFER, &aux);
  
  byte = (uint8_t) aux;
}

//------------------------------------------------------
int (enable_reporting)(){
  uint32_t aux;

  while(inputBufferFull())
    tickdelay(micros_to_ticks(DELAY_US));

  if(sys_outb(STATUS_REGISTER, WRITE_MOUSE)!= 0)
    return 1;

  while(inputBufferFull())
    tickdelay(micros_to_ticks(DELAY_US));

  if(sys_outb(INPUT_BUFFER, STREAM_MODE) != 0)
    return 1;

  sys_inb(OUTPUT_BUFFER, &aux);   //acknowledgment byte
  
  while(aux != 0xFA) //enquanto nao estiver bem
  {
    tickdelay(micros_to_ticks(DELAY_US));
    
    while(inputBufferFull())
      tickdelay(micros_to_ticks(DELAY_US));
    sys_outb(INPUT_BUFFER, STREAM_MODE);    //enable data reporting
    sys_inb(OUTPUT_BUFFER, &aux);   //acknowledgment byte
  }
  
  while(inputBufferFull())
    tickdelay(micros_to_ticks(DELAY_US));

  if(sys_outb(STATUS_REGISTER, WRITE_MOUSE)!= 0)
    return 1;

  while(inputBufferFull())
    tickdelay(micros_to_ticks(DELAY_US));

  if(sys_outb(INPUT_BUFFER, ENABLE_DT_RPT) != 0)
    return 1;

  tickdelay(micros_to_ticks(DELAY_US));
  sys_inb(OUTPUT_BUFFER, &aux);   //acknowledgment byte
  
  while(aux != 0xFA) //enquanto nao estiver bem
  {
    tickdelay(micros_to_ticks(DELAY_US));
    
    while(inputBufferFull())
      tickdelay(micros_to_ticks(DELAY_US));
    sys_outb(INPUT_BUFFER, ENABLE_DT_RPT);    //enable data reporting
    tickdelay(micros_to_ticks(DELAY_US));
    sys_inb(OUTPUT_BUFFER, &aux);   //acknowledgment byte
  }
  
  return 0;
  
}

//------------------------------------------------------------
void (reset_mouse1)(){
  uint32_t aux;

  while(inputBufferFull())//aguarda que o input buffer fique vazio
    tickdelay(micros_to_ticks(DELAY_US));
  sys_outb(KBC_COMMAND, WRITE_MOUSE);

  while(inputBufferFull())//aguarda que o input buffer fique vazio
    tickdelay(micros_to_ticks(DELAY_US));
  sys_outb(INPUT_BUFFER, DISABLE_DTRP);    //disable data reporting
  
  tickdelay(micros_to_ticks(DELAY_US));
  sys_inb(OUTPUT_BUFFER, &aux);   //acknowledgment byte

  while(aux != 0xFA) //enquanto nao estiver bem
  {
    tickdelay(micros_to_ticks(DELAY_US));
    while(inputBufferFull()) //aguarda que o input buffer fique vazio
      tickdelay(micros_to_ticks(DELAY_US));
    sys_outb(INPUT_BUFFER, DISABLE_DTRP);    //disable data reporting
   
    tickdelay(micros_to_ticks(DELAY_US));
    sys_inb(OUTPUT_BUFFER, &aux);   //acknowledgment byte
  }

  //sys_outb(KBC_COMMAND, DISABLE_MOUSE);    //disable mouse*/
}


