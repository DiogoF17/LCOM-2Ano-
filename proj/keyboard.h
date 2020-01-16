#ifndef _AUX_H_
#define _AUX_H_

/** @defgroup keyboard keyboard.h
 * @{
 *
 * Ficheiro que contem as declaracoes das funcoes relacionadas com o teclado.
*/

/**
 * Subscreve as interrupcoes do teclado.
 * @param1 contem a posicao do bit que deve ficar a 1 caso ocorra alguma interrupcao proveniente deste dispositivo
 * @return retorna 1 caso ocorra algum problema, 0 caso contrario
 */ 
int (kbc_subscribe)(uint8_t *bit_no);

/**
 * Da unsubscrive das interrupcoes do teclado.
 * @return retorna 1 caso ocorra algum problema, 0 caso contrario  
 */ 
int (kbc_unsubscribe)();

/**
 * Le o scancode da tecla premida pelo utilizador.
 */ 
void(kbc_ih)();

/**
 * Ativa as interrupcoes no output buffer do teclado.
 */ 
void(kbc_Enable)();

/**@}*/

#endif
