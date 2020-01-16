#ifndef _AUXI_H_
#define _AUXI_H_

#include <stdint.h>
#include <lcom/lcf.h>

/** @defgroup video video.h
 * @{
 *
 * Ficheiro que contem as declaracoes das funcoes relacionadas com o video graphics.
*/

/**
 * Ativa o modo de video passado como parametro e invoca a funcao read_adress().
 *@param1 modo de video a ativar
 *@return retorna 1 caso ocorra algum problema, 0 caso contrario  
 */  
int (graphic_mode)(uint16_t mode);

/**
 * Le o endereco de memoria para o qual estao localizados os pixeis que vamos utilizar. 
 *@param1 modo de video a ativar
 *@return retorna 1 caso ocorra algum problema, 0 caso contrario  
 */  
int (read_adress)(uint16_t mode);

/**
 * Desenha o xpm passado como parametro. 
 * @param1 endereco do xpm
 * @param2 coordenada X para comecar o desenho
 * @param3 coordenada Y para comecar o desenho
 * @param4 altura do xpm
 * @param5 comprimento do xpm
 */ 
void (draw_xpm)(uint8_t* aux, uint16_t x, uint16_t y, int alt, int cmp);

/**
 * Desenha o mouse alterando apenas as cores necessarias (se for branco nao desenha).
 * @param1 endereco do xpm
 * @param2 coordenada X para comecar o desenho
 * @param3 coordenada Y para comecar o desenho
 * @param4 altura do xpm
 * @param5 comprimento do xpm
 */ 
void draw_mouse(uint8_t* aux, uint16_t x, uint16_t y, int alt, int cmp);

/**
 * Desenha a preto o xpm.
 * @param1 endereco do xpm
 * @param2 coordenada X para comecar o desenho
 * @param3 coordenada Y para comecar o desenho
 * @param4 altura do xpm
 * @param5 comprimento do xpm
 */ 
void (clean_xpm)(uint8_t* aux, uint16_t x, uint16_t y, int alt, int cmp);

/**
 * Copia da memoria auxiliar para a memoria de video.
 * @return retorna o valor retornado pela funcao ja fornecida memcpy()
 */ 
void *copia_buffer();

/**@}*/

#endif

