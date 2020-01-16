#ifndef _AUXILIAR_H_
#define _AUXILIAR_H_

#include <stdint.h>
#include <lcom/lcf.h>

enum Estado{INI, MD, FI, NE, FEITO};

int (enable_reporting)();

int (subscribe_mouse)(uint8_t *bit_no);

int (unsubscribe_mouse)();

void (mouse_ih)();

void (reset_mouse1)(); //primeira funcao

void (remote_mouse)();

void (reset_mouse2)(); //segunda funcao

void (read_data)();

int (timer_subscribe_int)(uint8_t *bit_no);

int (timer_unsubscribe_int)();

void (processa_estado)(enum Estado *estado, struct packet a, uint8_t tolerance, uint8_t x_len);

int (abs)(int num);

#endif

