#ifndef _AUXI_H_
#define _AUXI_H_

#include <stdint.h>
#include <lcom/lcf.h>

void get_vbe_mode_info(uint16_t mode, vbe_mode_info_t *info);

int (graphic_mode)(uint16_t mode);

int (read_adress)(uint16_t mode);

void parseColor(uint32_t *color);

unsigned (getXRes)();

unsigned (getYRes)();

unsigned (getBitsPixel)();

unsigned (getBlueSize)();

unsigned (getRedSize)();

unsigned (getGreenSize)();

unsigned (getBluePos)();

unsigned (getRedPos)();

unsigned (getGreenPos)();

void (draw_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y);

void (clean_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y);

#endif

