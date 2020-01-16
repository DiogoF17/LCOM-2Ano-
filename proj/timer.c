#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

/** @defgroup timer timer.c
 * @{
 *
 * Ficheiro que contem as declaracoes das funcoes relacionadas com o timer.
*/

/**
 * Contem o identificador proveniente do hook_id do timer.
 */ 
extern int hook_idT;

/**
 * Usada como contador global para as interrupcoes do timer.
 */ 
int cont = 0;       

/**
 * Subscreve as interrupcoes do timer.
 * @param1 contem a posicao do bit que deve ficar a 1 caso ocorra alguma interrupcao proveniente deste dispositivo
 * @return retorna 1 caso ocorra algum problema, 0 caso contrario
 */
int (timer_subscribe_int)(uint8_t *bit_no) {

  //veirifica se nao esta a apontar para nada
  if(bit_no == NULL)
    return 1;

  hook_idT = *bit_no;
  if(sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_idT) != OK)
    return 1;

  return 0;
}

/**
 * Da unsubscrive das interrupcoes do timer.
 * @return retorna 1 caso ocorra algum problema, 0 caso contrario  
 */ 
int (timer_unsubscribe_int)() {
  sys_irqrmpolicy(&hook_idT);

  return 0;
}

/**
 * Incrementa um contador global a cada interrupcao do timer.
 */ 
void (timer_int_handler)() {
 cont++;
}

/**@}*/
