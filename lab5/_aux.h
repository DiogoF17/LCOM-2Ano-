#ifndef _AUX_H_
#define _AUX_H_

void(kbc_ih)();

int (kbc_subscribe)(uint8_t *bit_no);
int (kbc_unsubscribe)();
int(util_sys_inb)(int port, uint8_t *value);
void(kbc_ih)();
void(kbc_Enable)();

#endif
