#ifndef _MACROS_
#define _MACROS_

#include <lcom/lcf.h>

/** @defgroup macros macros.h
 * @{
 *
 * Ficheiro que contem os macros do teclado e do mouse.
*/

/**
*Linha de interrupcoes do teclado.
*/
#define IRQ1      1

/**
*Contem o breakcode da tecla ESC.
*/
#define ESC_BREAKCODE      0x81

/**
*Contem o endereco do Status Register.
*/
#define STATUS_REGISTER      0x64

/**
*Comando para lermos o Command Byte.
*/
#define READ_CMD_BYTE     0x20

/**
*Endereco do Output Buffer.
*/
#define OUTPUT_BUFFER        0x60

/**
*Comando para escrevermos o Command Byte.
*/
#define WRITE_CMD_BYTE       0x60

/**
*Endereco do Input Buffer.
*/
#define INPUT_BUFFER        0x60

/**
*Tempo a esperar.
*/
#define DELAY_US    20000

/**
*Linha de interrupcoes do mouse.
*/
#define IRQ12       12

/**
*Comando para escrever alguma coisa para o mouse.
*/
#define WRITE_MOUSE     0xD4

/**
*Comando para escrevermos alguma coisa para o kbc.
*/
#define KBC_COMMAND         0x64

/**
*Comando para ativarmos o data reporting.
*/
#define ENABLE_DT_RPT      0xF4

/**
*Comando para desativarmos o data reporting.
*/
#define DISABLE_DTRP        0xF5

/**
*Comando para desativarmos o mouse.
*/
#define DISABLE_MOUSE       0xA7

/**
*Comando para ativarmos o remote mode.
*/
#define REMOTE_MODE         0xF0

/**
*Comando para ativarmos o stream mode.
*/
#define STREAM_MODE        0xEA

/**
*Comando para mandarmos o pedido de ler dados do mouse quando esta em remote mode.
*/
#define READ_DATA      0xEB

/**@}*/

#endif
