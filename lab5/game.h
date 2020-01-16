#ifndef _GAME_H_
#define _GAME_H_

#include "lcom/lcf.h"
#include "auxi.h"
#include "_aux.h"
#include "lcom/lab2.h"
#include "lcom/lab3.h"
#include "videocard_macros.h"
#include "macros.h"
#include "i8254.h"
#include "figuras.h"
#include <math.h>
#include <time.h>

struct CoordIni{
    int xi;
    int yi;
    int x;  //so para a bola e as barras
    int y;  //so para bola e as barras
    int dx; //so para a bola
    int dy; //so para a bola
};

void clean_crono(int tempo);

void print_crono(int tempo);

void reboot_field();

void draw_field();

void start_game();

bool move_bola();

void checkLateralBarColision();

bool checkGoal();

void checkLineColision();

void geraXY(int *x, int *y);

int game();

#endif
