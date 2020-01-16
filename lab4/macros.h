#ifndef _MACROS_
#define _MACROS_

#include <lcom/lcf.h>

#define TIMER_RB_COUNT_  BIT(5)
#define TIMER_RB_STATUS_ BIT(4)
#define TIMER_RB_SEL(n)  BIT((n) + 1)

#define IRQ1      1

#define ESC_BREAKCODE      0x81

#define STATUS_REGISTER      0x64

#define READ_CMD_BYTE     0x20

#define OUTPUT_BUFFER        0x60

#define WRITE_CMD_BYTE       0x60

#define INPUT_BUFFER        0x60

#define DELAY_US    20000

#define IRQ12       12

#define WRITE_MOUSE     0xD4

#define KBC_COMMAND         0x64

#define ENABLE_DT_RPT      0xF4

#define DISABLE_DTRP        0xF5

#define DISABLE_MOUSE       0xA7

#define REMOTE_MODE         0xF0

#define STREAM_MODE        0xEA

#define READ_DATA      0xEB

#endif
