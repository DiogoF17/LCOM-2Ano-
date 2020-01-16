#ifndef _GAME_H_
#define _GAME_H_

#include "lcom/lcf.h"
#include "video.h"
#include "keyboard.h"
#include "lcom/lab2.h"
#include "lcom/lab3.h"
#include "videocard_macros.h"
#include "macros.h"
#include "mouse.h"
#include "i8254.h"
#include "figuras.h"
#include <math.h>
#include <time.h>

/** @defgroup game game.h
 * @{
 *
 * Ficheiro que contem a estrutura Coordenadas.
 * Contem tambem as declaracoes das funcoes usadas para o jogo!
 */

/**
 * Estrutura que guarda as coordenadas e o deslocamento de cada xpm.
 */ 
struct Coordenadas{
    /**
     * X inicial do xpm.
     */ 
    int xi;
    /**
     * Y inicial do xpm.
     */ 
    int yi;
    /**
     * X atual do xpm.
     */ 
    int x;
    /**
     * Y atual do xpm.
     */ 
    int y;  
    /**
     * Deslocamento em x.
     */ 
    int dx;
    /**
     * Deslocamento em y.
     */ 
    int dy; 
};

/**
 * Subcreve as interrupcoes do timer, teclado, mouse e graphics mode (0x105).
 * Criamos uma memoria auxiliar para fazer double buffering.
 * @return retorna 1 caso ocorra algum problema na preparacao das configuracoes para o jogo, 0 caso contrario 
 */
int entra();

/**
 * Repoem a configuracao inicial do mouse, sai do modo de video e da unsubcribe das interrupcoes subscritas na funcao entra().
 * @return retorna 1 caso ocorra algum problema na reposicao das configuracoes, 0 caso contrario
 */ 
int sai();

/**
 * Carrega todos os xpms no inicio do jogo.
 */ 
void load_xpm();

/**
 * Imprime o tempo atual do jogo em segundos.
 * @param1 contem o tempo atual em segundos
 */ 
void print_crono(int tempo);

/**
 * Desenha o campo, as barras nas suas posicoes iniciais, a bola na sua posicao inicial, as pontuacoes de cada jogador e chama a funcao print_crono(3). 
 */ 
void draw_field();

/**
 * Repoem a bola e as barras nas suas posicoes iniciais, chama a funcao geraXY(), reseta as variaveis UP_PLAYER1, UP_PLAYER2, DOWN_PLAYER1 e DOWN_PLAYER2 para false e chama a funcao draw_field().
 */ 
void reboot_field();

/**
 * Atribui as coordenadas inicias e atuais a cada xpm, chama a funcao draw_field() iniciando o jogo.
 */ 
void start_game();

/**
 * Move a bola atualizando a sua posicao atual de acordo com o deslocamento.
 * @return retorna true caso a funcao checkGoal() retorne true, falso caso contrario
 */ 
bool move_bola();

/**
 * Move a barra.
 * @param1 estrutura que contem as coordenadas da barra
 * @param2 diz se o utilizador quer se mover para cima (UP=true) ou para baixo (UP=false)
 */ 
void move_barra(struct Coordenadas *player, bool UP);

/**
 * Verifica se ocorreu uma colisao entre a bola e a barra, se sim inverte o deslocamento em X.
 */ 
void checkLateralBarColision();

/**
 * Verifica se ocorreu algum ponto (se a bola colidiu com as linhas finais).
 * @return retorna true caso ocorra um ponto, falso caso contrario
 */ 
bool checkGoal();

/**
 * Verifica se a bola colidiu com as linhas laterais, alterando o deslocamento em Y.
 */ 
void checkLineColision();

/**
 * Gera o deslocamento em X e em Y da bola.
 * @param1 variavel que vai conter o deslocamento em X
 * @param2 variavel que vai conter o deslocamento em Y
 */ 
void geraXY(int *x, int *y);

/**
 * Funcao que comanda o jogo. 
 * @return retorna 1 caso ocorra algum problema durante o jogo ou 0 caso contrario
 */ 
int game();

/**
 * Funcao que atualiza a velocidade da bola a cada 3 segundos.
 */ 
void update_speed();

/**
 * Verifica se o utilizador clicou na tecla W, S, seta para cima ou seta para baixo.
 */ 
void verify_scancode();

/**
 * Verifica a posicao do mouse no menu de pausa.
 * @param1 coordenada atual em X do mouse
 * @param2 coordenada atual em Y do mouse
 * @param3 diz se o mouse se encontra em cima da opcao "Restart" 
 * @param4 diz se o mouse se encontra em cima da opcao "Exit to Menu" 
 */ 
void verify_pause_mouse_position(int x, int y , bool *restart, bool *exittomenu);

/**
 * Verifica a posicao do mouse no menu principal.
 * @param1 coordenada atual em X do mouse
 * @param2 coordenada atual em Y do mouse
 * @param3 diz se o mouse se encontra em cima da opcao "Start Game" 
 * @param4 diz se o mouse se encontra em cima da opcao "Rules" 
 * @param5 diz se o mouse se encontra em cima da opcao "Exit" 
 */ 
void verify_menu_mouse_position(int x, int y , bool *start_game, bool *rules, bool *Exit);

/**
 * Verifica a posicao do mouse no menu de vitoria(winner()).
 * @param1 coordenada atual em X do mouse
 * @param2 coordenada atual em Y do mouse
 * @param3 diz se o mouse se encontra em cima da opcao "Play Again" 
 * @param4 diz se o mouse se encontra em cima da opcao "Exit to Menu" 
 */ 
void verify_winner_mouse_position(int x, int y , bool *playagain, bool *exittomenu);

/**
 * Mostra o menu principal.
 * @return retorna false se quer comecar a jogar ou true se quer sair
 */ 
bool main_menu();

/**
 * Mostra o menu de vitoria.
 * @return retorna 1 se pretende voltar a jogar ou 2 se optou por voltar ao menu principal
 */ 
int winner();

/**
 * Mostra o menu de pausa.
 * @return retorna 0 caso tenha pressionado ESC, 1 se optou pela opcao restart ou 2 se pretende voltar ao menu principal
 */ 
int menu_pause();

/**
 * Mostra as regras e os controlos do jogo.
 */ 
void menu_rules();

/**
 * Verifica se os utilizadores estao prontos para jogar, isto e, se estao a pressionar as teclas W e seta para cima em simultaneo.
 * @param1 fica true se os utilizadores estiverem prontos para jogar, ou false caso contrario
 * @param2 contem o makecode ou o breakcode da tecla pressionada pelo utilizador
 */ 
void getReady(bool *ready_to_start, uint8_t scancode);

/**@}*/

#endif
