#ifndef _AUXILIAR_H_
#define _AUXILIAR_H_

#include <stdint.h>
#include <lcom/lcf.h>

/** @defgroup mouse mouse.h
 * @{
 *
 * Ficheiro que contem as declaracoes das funcoes relacionadas com o mouse.
*/

/**
 * Coloca o rato em stream mode e ativa o data reporting.
 * @return retorna 1 caso ocorra algum problema, 0 caso contrario
 */
int (enable_reporting)();

/**
 * Subscreve as interrupcoes do mouse.
 * @param1 contem a posicao do bit que deve ficar a 1 caso ocorra alguma interrupcao proveniente deste dispositivo
 * @return retorna 1 caso ocorra algum problema, 0 caso contrario
 */
int (subscribe_mouse)(uint8_t *bit_no);

/**
 * Da unsubscrive das interrupcoes do mouse.
 * @return retorna 1 caso ocorra algum problema, 0 caso contrario  
 */ 
int (unsubscribe_mouse)();

/**
 * Le o packet proveniente da interrupcao do mouse.
 */ 
void (mouse_ih)();

/**
 * Desativa o data reporting.
 */ 
void (reset_mouse1)(); 

/**
*Verifica se o Output Buffer esta cheio.
*@return retorn 1 caso esteja vazio, 0 caso contrario
*/
int(outputBufferFull)();

/**
*Verifica se o Input Buffer esta cheio.
*@return retorna 0 caso esteja vazio, 1 caso contrario
*/
int (inputBufferFull)();

/**@}*/

#endif

