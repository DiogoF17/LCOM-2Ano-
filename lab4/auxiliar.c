#include "auxiliar.h"
#include "macros.h"

extern int hook_id;
uint8_t byte;
int y_ini = 0, y_fin, x_ini = 0, x_fin, x_len_ut = 0;

int (abs)(int num){
  if(num < 0)
     return 0 - num;
  return num;
}

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

void (mouse_ih)(){

  byte = 0;

  uint32_t aux;

 while(outputBufferFull())
    tickdelay(micros_to_ticks(DELAY_US));

  sys_inb(OUTPUT_BUFFER, &aux);
  
  byte = (uint8_t) aux;
}

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

void (remote_mouse)(){
  uint32_t aux;

  while(inputBufferFull())
    tickdelay(micros_to_ticks(DELAY_US));
  sys_outb(KBC_COMMAND, WRITE_MOUSE);

  while(inputBufferFull())
    tickdelay(micros_to_ticks(DELAY_US));
  sys_outb(INPUT_BUFFER, REMOTE_MODE);    //ativa o remote mode
  
  tickdelay(micros_to_ticks(DELAY_US));
  sys_inb(OUTPUT_BUFFER, &aux);   //acknowledgment byte

  while(aux != 0xFA) //enquanto nao estiver bem
  {
    tickdelay(micros_to_ticks(DELAY_US));

    while(inputBufferFull())
      tickdelay(micros_to_ticks(DELAY_US));
    sys_outb(INPUT_BUFFER, REMOTE_MODE);    //ativa o remote mode

    tickdelay(micros_to_ticks(DELAY_US));
    sys_inb(OUTPUT_BUFFER, &aux);   //acknowledgment byte
  }
}

void (reset_mouse2)(){
  uint32_t aux;

  while(inputBufferFull())
      tickdelay(micros_to_ticks(DELAY_US));
    sys_outb(KBC_COMMAND, WRITE_MOUSE);

  while(inputBufferFull())
    tickdelay(micros_to_ticks(DELAY_US));
  sys_outb(INPUT_BUFFER, STREAM_MODE);    //ativa stream mode
  
  tickdelay(micros_to_ticks(DELAY_US));
  sys_inb(OUTPUT_BUFFER, &aux);   //acknowledgment byte

  while(aux != 0xFA) //enquanto nao estiver bem
  {
    tickdelay(micros_to_ticks(DELAY_US));
    while(inputBufferFull())
      tickdelay(micros_to_ticks(DELAY_US));
    sys_outb(INPUT_BUFFER, STREAM_MODE);    //ativa stream mode
    tickdelay(micros_to_ticks(DELAY_US));
    sys_inb(OUTPUT_BUFFER, &aux);   //acknowledgment byte
  }

  while(inputBufferFull())
    tickdelay(micros_to_ticks(DELAY_US));
  sys_outb(KBC_COMMAND, WRITE_MOUSE);
  
  while(inputBufferFull())
    tickdelay(micros_to_ticks(DELAY_US));
  sys_outb(INPUT_BUFFER, DISABLE_DTRP);    //disable data reporting
  
  tickdelay(micros_to_ticks(DELAY_US));
  sys_inb(OUTPUT_BUFFER, &aux);   //acknowledgment byte

  while(aux != 0xFA) //enquanto nao estiver bem
  {
    tickdelay(micros_to_ticks(DELAY_US));

    while(inputBufferFull())
      tickdelay(micros_to_ticks(DELAY_US));
    sys_outb(INPUT_BUFFER, DISABLE_DTRP);    //disable data reporting
    
    tickdelay(micros_to_ticks(DELAY_US));
    sys_inb(OUTPUT_BUFFER, &aux);   //acknowledgment byte
  }

  sys_irqenable(&hook_id);
  //--------------------------------------  
  aux = minix_get_dflt_kbc_cmd_byte();

  while(inputBufferFull())
    tickdelay(micros_to_ticks(DELAY_US));
  sys_outb(STATUS_REGISTER, WRITE_CMD_BYTE);

  while(inputBufferFull())
    tickdelay(micros_to_ticks(DELAY_US));
  sys_outb(INPUT_BUFFER, aux);   //reset o command byte
}

void (read_data)(){
  uint32_t aux;

  while(inputBufferFull())
    tickdelay(micros_to_ticks(DELAY_US));
  sys_outb(KBC_COMMAND, WRITE_MOUSE);

  while(inputBufferFull())
    tickdelay(micros_to_ticks(DELAY_US));
  sys_outb(INPUT_BUFFER, READ_DATA);    //envia o pedido

  tickdelay(micros_to_ticks(DELAY_US));
  sys_inb(OUTPUT_BUFFER, &aux);   //acknowledgment byte

  while(aux != 0xFA){ //se nao estiver bem
  tickdelay(micros_to_ticks(DELAY_US));

    while(inputBufferFull())
      tickdelay(micros_to_ticks(DELAY_US));
    sys_outb(INPUT_BUFFER, READ_DATA);    //envia o pedido
    
    tickdelay(micros_to_ticks(DELAY_US));
    sys_inb(OUTPUT_BUFFER, &aux);   //acknowledgment byte
  }
}

void (processa_estado)(enum Estado *estado, struct packet a, uint8_t tolerance, uint8_t x_len){
  if(*estado == INI){
      if(a.lb == 1 && (a.rb & a.mb ) == 0){ //verifica se so pressionou o botao esquerdo
        if(a.delta_x > -abs(tolerance) && a.delta_y > -abs(tolerance)){ //verifica se excedeu o valor da tolerancia
            x_len_ut += a.delta_x;
            x_fin += a.delta_x;
            y_fin += a.delta_y;
        }
        else  //excedeu
          *estado = NE;
      }
      else if((a.mb & a.rb) != 0) //verifica se pressionou algum botao que nao seja o esquerdo
        *estado = NE;
      else if((a.lb & (a.mb & a.rb)) == 0){
        if(((y_fin - y_ini)/(x_fin -x_ini)) >= 1 && x_len_ut >= x_len) //verifica se o declive e maior que 1 e se o comprimento do x for no minimo x_len passa para o estado seguinte se nao volta para o inicio
          *estado = MD; //passa ao estado seguinte
        else
          *estado = NE;//volta ao inicio
      }
  }
  else if(*estado == MD){
      x_fin = 0, y_fin = 0, x_len_ut = 0;  //reinicializa os valores
      if(a.rb == 1 && (a.lb & a.mb ) == 0) //se premir o botao direito passa para o estado seguinte
        *estado = FI;
      else if(((a.lb & a.mb) != 0) || abs(a.delta_x) > abs(tolerance) || abs(a.delta_y) > abs(tolerance))   //se premir um botao que nao seja o direito volta ao inicio
        *estado = NE;
  }
  else if(*estado == NE){ //estado inicial
      x_fin = 0, y_fin = 0, x_len_ut = 0;
      if(a.lb == 1 && (a.rb & a.mb ) == 0){//verifica se presionou so o botao esquerdo
        *estado = INI;
      }
  }
  else if(*estado == FI){
      if(a.rb == 1 && (a.lb & a.mb) == 0){ //verifica se so esta a pressionar o botao direito
        if(a.delta_x > -abs(tolerance) && a.delta_y < abs(tolerance)){ //verifica se excedeu o valor da tolerancia
          x_len_ut += a.delta_x;
          x_fin += a.delta_x;
          y_fin += a.delta_y;
        }
        else//excedeu
          *estado = NE;
      }
      else if((a.lb & a.rb) != 0) //pressionou outro boato para alem do direito
        *estado = NE; //voltao ao inicio
      else if((a.lb & (a.mb & a.rb)) == 0){ //verifica se nenhum botao foi pressionado
        if(abs((y_fin - y_ini)/(x_fin -x_ini)) >= 1 && x_len_ut >= x_len)//verifica se o declive e maior que 1 e se o comprimento do x for no minimo x_len passa para o estado seguinte se nao volta para o inicio
          *estado = FEITO; //passa ao estado de aceitacao
        else
          *estado = NE; //volta ao inicio
      }
  }
}
