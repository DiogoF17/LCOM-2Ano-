#ifndef _VIDEOCARD_MACROS_H_
#define _VIDEOCARD_MACROS_H_

/** @defgroup videocard_macros videocard_macros.h
 * @{
 *
 * Ficheiro que contem os macros do video graphics.
*/

/**
*Interrupcao 0x10.
*/
#define BIOS      0x10  

/**
*Valor a colocar no ah register para distinguir das funcoes VGA BIOS padroes.
*/
#define VBE_FUNC   0x4F

/**
*Funcao usada para dar set do mode que queremos.
*/
#define VD_MODE    0x02

/**
*Ativa o bit 14, e os restantes ficam a 0.
*/
#define BIT14  1 << 14 

/**@}*/

#endif

