#include "game.h"

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//                      VARIAVEIS GLOBAIS
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

//-------------------------------------------------------------------
//              ESTRUTURAS QUE GUARDAM AS COORDENADAS DAS FIGURAS
//-------------------------------------------------------------------
struct Coordenadas Bola;       //coordenadas da bola
struct Coordenadas BarraEsq;   //coordenadas da barra esquerda
struct Coordenadas BarraDir;   //coordenadas da brra direita
struct Coordenadas PontEsq;    //coordenadas da pontuacao esquerda
struct Coordenadas PontDir;    //coordenadas da pontuacao direita
struct Coordenadas TempEsq;    //coordenadas do tempo esquerdo
struct Coordenadas TempDir;    //coordenadas do tempo direito
struct Coordenadas Mouse;      //coordenadas do mouse
//-------------------------------------------------------------------

//-------------------------------------------------------------------
message msg;
uint32_t r, irq_set_kbc = 2, irq_set_timer = 1, irq_set_mouse = 4;
int ipc_status;
//-------------------------------------------------------------------

//-------------------------------------------------------------------
int pontesq = 0;            //contem o numero de pontos do jogador 1    
int pontdir = 0;             //contem o numero de pontos do jogador 2
//-------------------------------------------------------------------

//-------------------------------------------------------------------
uint32_t hook_idK;    //contem o identificador proveniente do hook_id para subscrever o teclado
uint8_t scancode;   //scancode lido
int hook_idT;       //contem o identificador proveniente do hook_id para subscrever o timer
int hook_id;            //mouse
extern int cont;       //usada como contador global
char *video_buffer;     //usada para fazer o double buffering
extern int tam;         //contem o tamanho necessario a reservar para a memoria do double buffering
extern uint8_t byte;
//-------------------------------------------------------------------
 
//-------------------------------------------------------------------
bool UP_PLAYER1 =false, UP_PLAYER2=false, DOWN_PLAYER1=false, DOWN_PLAYER2 =false, player1Ready=false, player2Ready=false;
//-------------------------------------------------------------------

//-------------------------------------------------------------------
uint8_t* numeros[10];           //array com os numeros em xpm ja carregados
int numeros_alt[10], numeros_cmp[10];       //array com as dimensoes dos numeros em xpm ja carregados
uint8_t *l_rulesControls,*l_exittomenu,*l_playagain,*l_restart,*l_exit,*l_rules,*l_startgame,*l_pressW, *l_pressSeta, *l_ready,*l_mouse, *l_pause,*l_menu,*l_win2,*l_win1, * l_campo, *l_bola, *l_barra, *l_zero, *l_um, *l_dois, *l_tres, *l_quatro, *l_cinco, *l_seis, *l_sete, *l_oito, *l_nove;   //os xpm ja carregados
int  l_rulesControls_alt,l_exittomenu_alt,l_playagain_alt,l_restart_alt,l_exit_alt,l_rules_alt,l_startgame_alt,l_pressW_alt,l_pressSeta_alt,l_ready_alt,l_mouse_alt, l_pause_alt,l_menu_alt,l_win1_alt, l_win2_alt, l_campo_alt, l_bola_alt, l_barra_alt, l_zero_alt, l_um_alt, l_dois_alt, l_tres_alt, l_quatro_alt, l_cinco_alt, l_seis_alt, l_sete_alt, l_oito_alt, l_nove_alt;  //dimensoes dos xpm
int  l_rulesControls_cmp,l_exittomenu_cmp,l_playagain_cmp,l_restart_cmp,l_exit_cmp,l_rules_cmp,l_startgame_cmp,l_pressW_cmp,l_pressSeta_cmp,l_ready_cmp,l_mouse_cmp, l_pause_cmp,l_menu_cmp,l_win1_cmp, l_win2_cmp, l_campo_cmp, l_bola_cmp, l_barra_cmp, l_zero_cmp, l_um_cmp, l_dois_cmp, l_tres_cmp, l_quatro_cmp, l_cinco_cmp, l_seis_cmp, l_sete_cmp, l_oito_cmp, l_nove_cmp;      //dimensoes dos xpm
//-------------------------------------------------------------------

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//                              FUNCOES
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

int entra(){
    uint8_t bit_no = 1, bit_noT = 0, bit_noM = 2;

    load_xpm();  //carrega todos os xpm

    
    if(enable_reporting() != 0)
        return 1;

    if(graphic_mode(0x105) != 0){
        vg_exit();
        return 1;
    }

    if(kbc_subscribe(&bit_no) != 0)
        return 1;

    if(timer_subscribe_int(&bit_noT) != 0)
        return 1;

        
    if(subscribe_mouse(&bit_noM) != 0)
        return 1;

    video_buffer = (char *) malloc(tam);  //double buffering
    clean_xpm(l_campo, 0, 0, l_campo_alt, l_campo_cmp);   //mete tudo a preto
    copia_buffer();

    return 0;
}

int sai(){
    kbc_Enable();
    if(timer_unsubscribe_int() != 0)
        return 1;

    reset_mouse1();

    if(kbc_unsubscribe()!=0)
        return 1;

    if(vg_exit() != 0)
        return 1;

    if(unsubscribe_mouse() != 0)
        return 1;

    return 0;
}

//-----------------------------------------------------------
void load_xpm(){
    xpm_image_t aux1;
    l_menu = xpm_load(menu, XPM_INDEXED, &aux1);
    l_menu_alt = aux1.height; l_menu_cmp = aux1.width;
    l_campo = xpm_load(campo, XPM_INDEXED, &aux1);
    l_campo_alt = aux1.height; l_campo_cmp = aux1.width;
    l_bola = xpm_load(bola, XPM_INDEXED, &aux1);
    l_bola_alt = aux1.height; l_bola_cmp = aux1.width;
    l_barra = xpm_load(barra, XPM_INDEXED, &aux1);
    l_barra_alt = aux1.height; l_barra_cmp = aux1.width;
    l_zero = xpm_load(zero, XPM_INDEXED, &aux1);
    l_zero_alt = aux1.height; l_zero_cmp = aux1.width;
    l_um = xpm_load(um, XPM_INDEXED, &aux1);
    l_um_alt = aux1.height; l_um_cmp = aux1.width;
    l_dois = xpm_load(dois, XPM_INDEXED, &aux1);
    l_dois_alt = aux1.height; l_dois_cmp = aux1.width;
    l_tres = xpm_load(tres, XPM_INDEXED, &aux1);
    l_tres_alt = aux1.height; l_tres_cmp = aux1.width;
    l_quatro = xpm_load(quatro, XPM_INDEXED, &aux1);
    l_quatro_alt = aux1.height; l_quatro_cmp = aux1.width;
    l_cinco = xpm_load(cinco, XPM_INDEXED, &aux1);
    l_cinco_alt = aux1.height; l_cinco_cmp = aux1.width;
    l_seis = xpm_load(seis, XPM_INDEXED, &aux1);
    l_seis_alt = aux1.height; l_seis_cmp = aux1.width;
    l_sete = xpm_load(sete, XPM_INDEXED, &aux1);
    l_sete_alt = aux1.height; l_sete_cmp = aux1.width;
    l_oito = xpm_load(oito, XPM_INDEXED, &aux1);
    l_oito_alt = aux1.height; l_oito_cmp = aux1.width;
    l_nove = xpm_load(nove, XPM_INDEXED, &aux1);
    l_nove_alt = aux1.height; l_nove_cmp = aux1.width;
    numeros[0] = l_zero; numeros_alt[0] = l_zero_alt; numeros_cmp[0] = l_zero_cmp;
    numeros[1] = l_um; numeros_alt[1] = l_um_alt; numeros_cmp[1] = l_um_cmp;
    numeros[2] = l_dois; numeros_alt[2] = l_dois_alt; numeros_cmp[2] = l_dois_cmp;
    numeros[3] = l_tres; numeros_alt[3] = l_tres_alt; numeros_cmp[3] = l_tres_cmp;
    numeros[4] = l_quatro; numeros_alt[4] = l_quatro_alt; numeros_cmp[4] = l_quatro_cmp;
    numeros[5] = l_cinco; numeros_alt[5] = l_cinco_alt; numeros_cmp[5] = l_cinco_cmp;
    numeros[6] = l_seis; numeros_alt[6] = l_seis_alt; numeros_cmp[6] = l_seis_cmp;
    numeros[7] = l_sete; numeros_alt[7] = l_sete_alt; numeros_cmp[7] = l_sete_cmp;
    numeros[8] = l_oito; numeros_alt[8] = l_oito_alt; numeros_cmp[8] = l_oito_cmp;
    numeros[9] = l_nove; numeros_alt[9] = l_nove_alt; numeros_cmp[9] = l_nove_cmp;
    l_win2 = xpm_load(win2, XPM_INDEXED, &aux1);
    l_win2_alt = aux1.height; l_win2_cmp = aux1.width;
    l_win1 = xpm_load(win1, XPM_INDEXED, &aux1);
    l_win1_alt = aux1.height; l_win1_cmp = aux1.width;
    l_pause = xpm_load(pausa, XPM_INDEXED, &aux1);
    l_pause_alt = aux1.height; l_pause_cmp = aux1.width;
    l_mouse = xpm_load(mouse, XPM_INDEXED, &aux1);
    l_mouse_alt = aux1.height; l_mouse_cmp = aux1.width;
    l_pressW = xpm_load(pressW,XPM_INDEXED, &aux1);
    l_pressW_alt= aux1.height; l_pressW_cmp = aux1.width;
    l_pressSeta = xpm_load(pressSeta,XPM_INDEXED, &aux1);
    l_pressSeta_alt= aux1.height; l_pressSeta_cmp = aux1.width;
    l_ready = xpm_load(ready,XPM_INDEXED, &aux1);
    l_ready_alt= aux1.height; l_ready_cmp = aux1.width;
    l_startgame = xpm_load(startgame,XPM_INDEXED, &aux1);
    l_startgame_alt= aux1.height; l_startgame_cmp = aux1.width;
    l_rules = xpm_load(rules,XPM_INDEXED, &aux1);
    l_rules_alt= aux1.height; l_rules_cmp = aux1.width;
    l_exit = xpm_load(Exit,XPM_INDEXED, &aux1);
    l_exit_alt= aux1.height; l_exit_cmp = aux1.width;
    l_restart = xpm_load(restart,XPM_INDEXED, &aux1);
    l_restart_alt= aux1.height; l_restart_cmp = aux1.width;
    l_playagain = xpm_load(playagain,XPM_INDEXED, &aux1);
    l_playagain_alt= aux1.height; l_playagain_cmp = aux1.width;
    l_exittomenu = xpm_load(exittomenu,XPM_INDEXED, &aux1);
    l_exittomenu_alt= aux1.height; l_exittomenu_cmp = aux1.width;
    l_rulesControls = xpm_load(rules_controls,XPM_INDEXED, &aux1);
    l_rulesControls_alt= aux1.height; l_rulesControls_cmp = aux1.width;

}

//----------------------------------------------------------------------------
void print_crono(int tempo){
    int partesq, partdir;
    partesq = tempo / 10;
    partdir = tempo % 10;
    draw_xpm(numeros[partesq], TempEsq.xi, TempEsq.yi, numeros_alt[partesq], numeros_cmp[partesq]);
    draw_xpm(numeros[partdir], TempDir.xi, TempDir.yi, numeros_alt[partdir], numeros_cmp[partdir]);
}

//---------------------------------------------------------------
void draw_field(){
    draw_xpm(l_campo, 0, 0, l_campo_alt, l_campo_cmp);   //desenha o campo
    draw_xpm(l_barra, BarraEsq.xi, BarraEsq.yi, l_barra_alt, l_barra_cmp); //desenha barras
    draw_xpm(l_barra, BarraDir.xi, BarraDir.yi, l_barra_alt, l_barra_cmp); //desenha barras
    draw_xpm(l_bola, Bola.xi, Bola.yi, l_bola_alt, l_bola_cmp);
    draw_xpm(numeros[pontesq], PontEsq.xi, PontEsq.yi, numeros_alt[pontesq], numeros_cmp[pontesq]);
    draw_xpm(numeros[pontdir], PontDir.xi, PontDir.yi, numeros_alt[pontdir], numeros_cmp[pontdir]);
    print_crono(3);
}

void reboot_field(){
    int x, y;
    Bola.x = Bola.xi;
    Bola.y = Bola.yi;
    BarraEsq.x = BarraEsq.xi;
    BarraEsq.y = BarraEsq.yi;
    BarraDir.x = BarraDir.xi;
    BarraDir.y = BarraDir.yi;
    geraXY(&x, &y);
    Bola.dx = (x / abs(x)) * 4;
    if(y != 0)
        Bola.dy = (y / abs(y)) * 4;
    else
        Bola.dy = 0;
    UP_PLAYER1 =false, UP_PLAYER2=false, DOWN_PLAYER1=false, DOWN_PLAYER2 =false;
    draw_field();
    
}

//---------------------------------------------------------------------
bool checkGoal(){
    if((Bola.x + 25) >= 1002){ //25 largura da bola, 1002 e o x da linha direita
        pontesq++;
        Bola.x = 1002 - 25;
        return true;
    }
    if(Bola.x <= 23){ //17 largura da barra
        pontdir++;
        Bola.x = 23;
        return true;
    }
    return false;
}

void checkLineColision(){
    if(Bola.y <= 73){ //73 linha superior
        Bola.y = 73;
        Bola.dy = -(Bola.dy);
    }
    if(Bola.y>= 721){
        Bola.y = 721;
        Bola.dy = -(Bola.dy);
    }
}

void checkLateralBarColision(){
    if((Bola.x + 25) >= BarraDir.xi){ //25 largura da bola
        if((BarraDir.y <= Bola.y && Bola.y <= BarraDir.y + 150) || (BarraDir.y <= Bola.y + 25 && Bola.y + 25 <= BarraDir.y + 150)){ //150 comprimento da barra
            Bola.x = BarraDir.xi - 25;
            Bola.dx = -(Bola.dx);
        }
    }
    if(Bola.x <= (BarraEsq.xi + 17)){ //17 largura da barra
        if((BarraEsq.y <= Bola.y && Bola.y <= BarraEsq.y + 150) || (BarraEsq.y <= Bola.y + 25 && Bola.y + 25 <= BarraEsq.y + 150)){
            Bola.x = BarraEsq.xi + 17;
            Bola.dx = -(Bola.dx);
        }
    }
}

//------------------------------------------------------------------------
bool move_bola(){
    int xant = Bola.x, yant = Bola.y;
    Bola.x += Bola.dx;
    Bola.y += Bola.dy;
    checkLateralBarColision();
    checkLineColision();
    if(checkGoal())
        return true;
    clean_xpm(l_bola, xant, yant, l_bola_alt, l_bola_cmp);
    draw_xpm(l_bola, Bola.x, Bola.y, l_bola_alt, l_bola_cmp);
    return false;
}

void move_barra(struct Coordenadas *player , bool UP){
    if (UP){
        clean_xpm(l_barra,player->x,player->y,l_barra_alt,l_barra_cmp);
        player->y= player->y - 5;
        draw_xpm(l_barra,player->x,player->y,l_barra_alt,l_barra_cmp);
    }
    else{
        clean_xpm(l_barra,player->x,player->y,l_barra_alt,l_barra_cmp);
        player->y= player->y + 5;
        draw_xpm(l_barra,player->x,player->y,l_barra_alt,l_barra_cmp);
    }
}

//----------------------------------------------------------------------
void geraXY(int *x, int *y){
    *x = (rand() % 20) - 10;
    while(*x == 0)
        *x = (rand() % 20) - 10;
    *y = (rand() % 20) - 10;
}

//----------------------------------------------------
void update_speed(){
    if(Bola.dx > 0)
        Bola.dx = Bola.dx + 1;
    else
        Bola.dx = Bola.dx - 1;
    if(Bola.dy > 0)
        Bola.dy = Bola.dy + 1;
    else
        Bola.dy = Bola.dy - 1;
}

//------------------------------------------------
void verify_scancode(){
    if (scancode == 0x48){ 
        UP_PLAYER2=true;
        DOWN_PLAYER2=false;
    }
    else if (scancode== 0xc8){
        UP_PLAYER2=false;
    }
    else if (scancode == 0x11){
        UP_PLAYER1=true;
        DOWN_PLAYER1=false;
    }
    else if (scancode== 0x91){
        UP_PLAYER1=false;
    }
    else if (scancode == 0x50){
        DOWN_PLAYER2=true;
        UP_PLAYER2=false;
    }
    else if (scancode == 0xd0){
        DOWN_PLAYER2=false;
    }
    else if (scancode == 0x1f){
        DOWN_PLAYER1=true;
        UP_PLAYER1=false;
    }
    else if (scancode == 0x9f){
        DOWN_PLAYER1=false;
    }
}

void verify_pause_mouse_position(int x,int y, bool *restart, bool *exittomenu){
    if ((430<=x)&&(x<=591)&&(359<=y)&&(y<=407)){
        *restart=true;
        *exittomenu=false;
        draw_xpm(l_restart,430,359,l_restart_alt,l_restart_cmp);
    }
    else if ((386<=x)&&(x<=643)&&(414<=y)&&(y<=462)){
        *restart=false;
        *exittomenu=true;
        draw_xpm(l_exittomenu,386,414,l_exittomenu_alt,l_exittomenu_cmp);
    }
    else{
        *restart=false;
        *exittomenu=false;
    }
}

void verify_menu_mouse_position(int x,int y, bool *start_game, bool *rules, bool *Exit){
    if ((380<=x)&&(x<=616)&&(398<=y)&&(y<=449)){
        *start_game=true;
        *rules=false;
        *Exit=false;
        draw_xpm(l_startgame,380,398,l_startgame_alt,l_startgame_cmp);
    }
    else if ((436<=x)&&(x<=558)&&(450<=y)&&(y<=502)){
        *start_game=false;
        *rules=true;
        *Exit=false;
        draw_xpm(l_rules,436,450,l_rules_alt,l_rules_cmp);
    }
    else if ((451<=x)&&(x<=543)&&(509<=y)&&(y<=553)){
        *start_game=false;
        *rules=false;
        *Exit=true;
        draw_xpm(l_exit,451,509,l_exit_alt,l_exit_cmp);
    }else{
        *start_game=false;
        *rules=false;
        *Exit=false;
    }
}

void verify_winner_mouse_position(int x,int y, bool *playagain, bool *exittomenu){
    if ((408<=x)&&(x<=616)&&(390<=y)&&(y<=445)){
        *playagain=true;
        *exittomenu=false;
        draw_xpm(l_playagain,408,390,l_playagain_alt,l_playagain_cmp);
    }
    else if ((382<=x)&&(x<=639)&&(455<=y)&&(y<=503)){
        *playagain=false;
        *exittomenu=true;
        draw_xpm(l_exittomenu,382,455,l_exittomenu_alt,l_exittomenu_cmp);
    }
    else{
        *playagain=false;
        *exittomenu=false;
    }
}

//--------------------------------------------------------------------------------
int menu_pause(){
    bool segundo = false, terceiro = false, lb = false,restart=false,exittomenu=false;
    uint8_t x_sign, y_sign;
    Mouse.x = 491; Mouse.y = 351; 
    draw_xpm(l_pause, 257, 228, l_pause_alt, l_pause_cmp);
    draw_mouse(l_mouse, Mouse.x, Mouse.y,l_mouse_alt, l_mouse_cmp);
    copia_buffer();
    scancode = 0;
    while( scancode != ESC_BREAKCODE ) { /* You may want to use a different condition */
      /* Get a request message. */
      if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
          printf("driver_receive failed with: %d", r);
          continue;
      }
      if (is_ipc_notify(ipc_status)) { /* received notification */
          switch (_ENDPOINT_P(msg.m_source)) {
              case HARDWARE: /* hardware interrupt notification */				
                    if (msg.m_notify.interrupts & irq_set_kbc) { /* subscribed interrupt */ 
                        kbc_ih();
                    }
                    if (msg.m_notify.interrupts & irq_set_mouse) { /* subscribed interrupt */ 
                        mouse_ih();
                        if((byte & 0x08) != 0  && segundo == false && terceiro == false){
                        segundo = true;
                        if(byte & 0x01)
                            lb = true;
                        x_sign = byte & 0x10;
                        y_sign = byte & 0x20;
                      }
                      else if(segundo == true){
                        terceiro = true;
                        segundo = false;
                        if(x_sign == 16)
                          Mouse.dx = byte - 256;
                        else
                          Mouse.dx =  byte;
                      }
                      else if(terceiro == true){
                        terceiro = false;
                        if(y_sign == 32)
                            Mouse.dy = byte - 256;
                        else
                         Mouse.dy =  byte;
                        if(Mouse.dx != 0 || Mouse.dy != 0){
                            Mouse.x += Mouse.dx; Mouse.y -= Mouse.dy;
                            if (Mouse.x<257||Mouse.x>749)
                                Mouse.x -=Mouse.dx;
                            if (Mouse.y<228||Mouse.y>479)
                                Mouse.y +=Mouse.dy;
                            draw_xpm(l_pause, 257, 228, l_pause_alt, l_pause_cmp);
                            verify_pause_mouse_position(Mouse.x,Mouse.y, &restart, &exittomenu);
                            draw_mouse(l_mouse, Mouse.x, Mouse.y,l_mouse_alt, l_mouse_cmp);
                            copia_buffer();
                            lb = false;
                        }
                        if(Mouse.dx == 0 && Mouse.dy == 0){
                            if (lb){
                                if (restart){
                                   return 1; 
                                }
                                else if (exittomenu){
                                      return 2;  
                                }
                            }
                        }
                      }
                    }
                  break;
              default:
                  break; /* no other notifications expected: do nothing */	
          }
      } else { /* received a standard message, not a notification */
          /* no standard messages expected: do nothing */
      }
  }
  clean_xpm(l_pause, 257, 228, l_pause_alt, l_pause_cmp);
  draw_xpm(l_bola, Bola.x, Bola.y, l_bola_alt, l_bola_cmp);
  copia_buffer();

  return 0;
}

void menu_rules(){
    scancode=0;
    draw_xpm(l_rulesControls,0,0,l_rulesControls_alt,l_rulesControls_cmp);
    copia_buffer();
    while( scancode != ESC_BREAKCODE) { /* You may want to use a different condition */
      /* Get a request message. */
      if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
          printf("driver_receive failed with: %d", r);
          continue;
      }
      if (is_ipc_notify(ipc_status)) { /* received notification */
          switch (_ENDPOINT_P(msg.m_source)) {
              case HARDWARE: /* hardware interrupt notification */
                    if (msg.m_notify.interrupts & irq_set_kbc) { /* subscribed interrupt */ 
                        kbc_ih();	
                    }
                	if (msg.m_notify.interrupts & irq_set_mouse) { /* subscribed interrupt */ 
                        mouse_ih();
                    }		
                  break;
              default:
                  break; /* no other notifications expected: do nothing */	
          }
      } else { /* received a standard message, not a notification */
          /* no standard messages expected: do nothing */
      }
  }
}


bool main_menu(){
    bool segundo = false, terceiro = false, lb = false,start_game=false,rules=false,Exit=false;
    uint8_t x_sign, y_sign;
    Mouse.x = 491; Mouse.y = 351;
    draw_xpm(l_menu, 0, 0, l_menu_alt, l_menu_cmp);
    draw_mouse(l_mouse, Mouse.x, Mouse.y,l_mouse_alt, l_mouse_cmp);
    copia_buffer();
    while( 1) { /* You may want to use a different condition */
      /* Get a request message. */
      if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
          printf("driver_receive failed with: %d", r);
          continue;
      }
      if (is_ipc_notify(ipc_status)) { /* received notification */
          switch (_ENDPOINT_P(msg.m_source)) {
              case HARDWARE: /* hardware interrupt notification */
                    if (msg.m_notify.interrupts & irq_set_kbc) { /* subscribed interrupt */ 
                            kbc_ih();	
                    }
                	if (msg.m_notify.interrupts & irq_set_mouse) { /* subscribed interrupt */ 
                        mouse_ih();
                        if((byte & 0x08) != 0  && segundo == false && terceiro == false){
                        segundo = true;
                        if(byte & 0x01)
                            lb = true;
                        x_sign = byte & 0x10;
                        y_sign = byte & 0x20;
                      }
                      else if(segundo == true){
                        terceiro = true;
                        segundo = false;
                        if(x_sign == 16)
                          Mouse.dx = byte - 256;
                        else
                          Mouse.dx =  byte;
                      }
                      else if(terceiro == true){
                        terceiro = false;
                        if(y_sign == 32)
                            Mouse.dy = byte - 256;
                        else
                         Mouse.dy =  byte;
                        if(Mouse.dx != 0 || Mouse.dy != 0){
                            Mouse.x += Mouse.dx; Mouse.y -= Mouse.dy;
                            if (Mouse.x<0||Mouse.x>1007)
                                Mouse.x -=Mouse.dx;
                            if (Mouse.y<0||Mouse.y>747)
                                Mouse.y +=Mouse.dy;
                            draw_xpm(l_menu, 0, 0, l_menu_alt, l_menu_cmp);
                            verify_menu_mouse_position(Mouse.x,Mouse.y,&start_game,&rules,&Exit);
                            draw_mouse(l_mouse, Mouse.x, Mouse.y,l_mouse_alt, l_mouse_cmp);
                            copia_buffer();
                            lb = false;
                        }
                        if(Mouse.dx == 0 && Mouse.dy == 0){
                            if (lb){
                                if (start_game){
                                    return false;
                                }
                                else if (rules){
                                    menu_rules();
                                    draw_xpm(l_menu,0,0,l_menu_alt,l_menu_cmp);
                                    draw_mouse(l_mouse, Mouse.x, Mouse.y,l_mouse_alt, l_mouse_cmp);
                                    copia_buffer();
                                }
                                else if (Exit){
                                    return true;    
                                }
                            }
                        }
                      }
                    }		
                  break;
              default:
                  break; /* no other notifications expected: do nothing */	
          }
      } else { /* received a standard message, not a notification */
          /* no standard messages expected: do nothing */
      }
  }
}


int winner(){
    bool segundo = false, terceiro = false, lb = false,playagain=false,exittomenu=false,win1=false;
    uint8_t x_sign, y_sign;
    Mouse.x = 491; Mouse.y = 351; 
    if(pontdir == 3){
        draw_xpm(l_win2, 257, 288, l_win2_alt, l_win2_cmp);
        win1=false;
    } 
    else if(pontesq == 3){
        draw_xpm(l_win1, 257, 288, l_win1_alt, l_win1_cmp);
        win1=true;
    }
    draw_mouse(l_mouse, Mouse.x, Mouse.y,l_mouse_alt, l_mouse_cmp);
    copia_buffer();
    while( 1 ) { /* You may want to use a different condition */
      /* Get a request message. */
      if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
          printf("driver_receive failed with: %d", r);
          continue;
      }
      if (is_ipc_notify(ipc_status)) { /* received notification */
          switch (_ENDPOINT_P(msg.m_source)) {
              case HARDWARE: /* hardware interrupt notification */				
                    if (msg.m_notify.interrupts & irq_set_kbc) { /* subscribed interrupt */ 
                        kbc_ih();
                    }
                    if (msg.m_notify.interrupts & irq_set_mouse) { /* subscribed interrupt */ 
                        mouse_ih();
                        if((byte & 0x08) != 0  && segundo == false && terceiro == false){
                        segundo = true;
                        if(byte & 0x01)
                            lb = true;
                        x_sign = byte & 0x10;
                        y_sign = byte & 0x20;
                      }
                      else if(segundo == true){
                        terceiro = true;
                        segundo = false;
                        if(x_sign == 16)
                          Mouse.dx = byte - 256;
                        else
                          Mouse.dx =  byte;
                      }
                      else if(terceiro == true){
                        terceiro = false;
                        if(y_sign == 32)
                            Mouse.dy = byte - 256;
                        else
                         Mouse.dy =  byte;
                        if(Mouse.dx != 0 || Mouse.dy != 0){
                            Mouse.x += Mouse.dx; Mouse.y -= Mouse.dy;
                            if (Mouse.x<257||Mouse.x>750)
                                Mouse.x -=Mouse.dx;
                            if (Mouse.y<288||Mouse.y>540)
                                Mouse.y +=Mouse.dy;
                            if (win1){
                                draw_xpm(l_win1, 257, 288, l_win1_alt, l_win1_cmp);
                            }else
                            {
                                draw_xpm(l_win2,257, 288, l_win2_alt, l_win2_cmp);
                            }
                            verify_winner_mouse_position(Mouse.x,Mouse.y,&playagain,&exittomenu);
                            draw_mouse(l_mouse, Mouse.x, Mouse.y,l_mouse_alt, l_mouse_cmp);
                            copia_buffer();
                            lb = false;
                        }
                        if(Mouse.dx == 0 && Mouse.dy == 0){
                            if (lb){
                                if (playagain){
                                    return 1;
                                }
                                else if (exittomenu){
                                     return 2;   
                                }
                            }
                        }
                      }
                    }		
                  break;
              default:
                  break; /* no other notifications expected: do nothing */	
          }
      } else { /* received a standard message, not a notification */
          /* no standard messages expected: do nothing */
      }
  }
}

void getReady(bool *ready_to_start,uint8_t scancode){
    
    if (scancode == 0x48){ 
        clean_xpm(l_pressSeta,600,170,l_pressSeta_alt,l_pressSeta_cmp);
        draw_xpm(l_ready,600,170,l_ready_alt,l_ready_cmp);
        player2Ready=true;
    }
    else if (scancode== 0xc8){
        draw_xpm(l_pressSeta,600,170,l_pressSeta_alt,l_pressSeta_cmp);
        player2Ready=false;
    }
    else if (scancode == 0x11){
        clean_xpm(l_pressW,182,170,l_pressW_alt,l_pressW_cmp);
        draw_xpm(l_ready,182,170,l_ready_alt,l_ready_cmp);
        player1Ready=true;
    }
    else if (scancode== 0x91){
        draw_xpm(l_pressW,182,170,l_pressW_alt,l_pressW_cmp);
        player1Ready=false;
    }
    if (player1Ready && player2Ready){
        *ready_to_start=true;
        clean_xpm(l_ready,600,170,l_ready_alt,l_ready_cmp);
        clean_xpm(l_ready,182,170,l_ready_alt,l_ready_cmp);
    }
    
}

//--------------------------------------------------------------------------------
int game(){

    int tempo = 60, espera = 2;
    bool comeca = false,ready_to_start=false; //passado tres segundos comeca o jogo
    srand(time(NULL));

    //-----------------------------------------------------
    if(entra() != 0)
        return 1;

    //-----------------------------------------------------
    if(!main_menu()){  //e true se o utilizador clicar no exit no main_menu() 
    
        start_game();
        copia_buffer();
        //-------------------------------------------------------
        while (pontesq != 3 || pontdir != 3)
        {
            while( tempo > -1 ) { /* You may want to use a different condition */
            /* Get a request message. */
            if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
                printf("driver_receive failed with: %d", r);
                continue;
            }
            if (is_ipc_notify(ipc_status)) { /* received notification */
                switch (_ENDPOINT_P(msg.m_source)) {
                    case HARDWARE: /* hardware interrupt notification */
                            if (msg.m_notify.interrupts & irq_set_mouse) { /* subscribed interrupt */ 
                                mouse_ih();
                            }				
                            if (msg.m_notify.interrupts & irq_set_kbc) { /* subscribed interrupt */ 
                                kbc_ih();
                                //---------------------------------------------
                                if (!ready_to_start)
                                    getReady(&ready_to_start,scancode);
                                //enquanto nao tiver comecado nao interessa o movimento ds setas
                                if(comeca)
                                    verify_scancode();
                                if(scancode == ESC_BREAKCODE){
                                    int valor = menu_pause();
                                    if( valor == 1){
                                        pontesq = 0;  
                                        pontdir = 0;
                                        start_game();
                                        copia_buffer();
                                        tempo = 60; cont = 0;
                                        ready_to_start = false;
                                        comeca = false;
                                        espera = 3;
                                        player1Ready = false;
                                        player2Ready = false;
                                    }
                                    else if(valor == 2){
                                        if(!main_menu()){
                                            pontesq = 0;  
                                            pontdir = 0;
                                            start_game();
                                            copia_buffer();
                                            tempo = 60; cont = 0;
                                            ready_to_start = false;
                                            comeca = false;
                                            espera = 3;
                                            player1Ready = false;
                                            player2Ready = false;
                                        }
                                        else{
                                            if(sai() != 0)
                                                return 1;
                                            return 0;
                                        }
                                    }
                                }
                            }
                            if((msg.m_notify.interrupts & irq_set_timer)){
                                timer_int_handler();
                                //--------------------------------------------------------
                                //espera dois segundos antes de comecar a movimentar a bola
                                if((cont % sys_hz() == 0) && ready_to_start && (!comeca)){
                                    print_crono(espera);
                                    if(espera == 0){
                                        comeca=true;
                                        cont = 0;
                                    }
                                    espera--;
                                }
                                if(comeca){
                                    //-------------------------------------------------------
                                    //          MOVIMENTA A BARRA
                                    //-------------------------------------------------------
                                    //verifica se algum dos jogadores esta a movimentar a barra
                                    if(UP_PLAYER1 && BarraEsq.y>75){      
                                        move_barra(&BarraEsq,UP_PLAYER1);
                                    }
                                    if(DOWN_PLAYER1 && BarraEsq.y<595){      
                                        move_barra(&BarraEsq,UP_PLAYER1);
                                    }
                                    if(UP_PLAYER2 && BarraDir.y>75){      
                                        move_barra(&BarraDir,UP_PLAYER2);
                                    }
                                    if(DOWN_PLAYER2 && BarraDir.y<595){      
                                        move_barra(&BarraDir,UP_PLAYER2);
                                    }
                                    //----------------------------------------------------------------
                                    //          IMPRIME CRONOMETRO
                                    //-----------------------------------------------------------------
                                    //sempre que passa um segundo imprime
                                    if(cont % sys_hz() == 0){
                                        print_crono(tempo);
                                        tempo--;
                                    }
                                    //----------------------------------------------------------------
                                    //     ATUALIZA O SPEED DA BOLA
                                    //------------------------------------------------------------------
                                    //a cada tres segundos aumenta em um o deslocamento da bola
                                    if(cont % (sys_hz()*3) == 0)
                                        update_speed();
                                    //-----------------------------------------------------
                                    //              ATUALIZA A POSICAO DA BOLA
                                    //-------------------------------------------------
                                    //atualiza a bola a uma taxa de frame rate de 30
                                    if (cont % 2 == 0){
                                        if(move_bola()){ //movimenta a bola e se for golo volta as posicoes iniciais
                                            cont = 0; tempo = 60;
                                            comeca = false;
                                            espera = 2;
                                            reboot_field();
                                            if(pontesq == 3 || pontdir == 3){
                                                tempo = 0;
                                                if(winner() == 1){
                                                    pontesq = 0;  
                                                    pontdir = 0;
                                                    start_game();
                                                    copia_buffer();
                                                    tempo = 60; cont = 0;
                                                    ready_to_start = false;
                                                    comeca = false;
                                                    player1Ready = false;
                                                    player2Ready = false;
                                                }
                                                else{
                                                    if(!main_menu()){
                                                        pontesq = 0;  
                                                        pontdir = 0;
                                                        start_game();
                                                        copia_buffer();
                                                        tempo = 60; cont = 0;
                                                        ready_to_start = false;
                                                        comeca = false;
                                                        espera = 3;
                                                        player1Ready = false;
                                                        player2Ready = false;
                                                    }
                                                    else{
                                                        if(sai() != 0)
                                                            return 1;
                                                        return 0;
                                                    }
                                                }
                                            }
                                            scancode = 0;
                                        }
                                    }
                                    //-----------------------------------------------------------
                                    //----------------------------------------------------------
                                }
                                //faz copia da memoria auxiliar para a memoria de video
                                if(copia_buffer() == NULL){
                                            if(timer_unsubscribe_int() != 0)
                                                return 1;

                                                kbc_Enable();
                                            if(kbc_unsubscribe()!=0)
                                                return 1;

                                            if(vg_exit() != 0)
                                                return 1;
                                }
                            }
                        break;
                    default:
                        break; /* no other notifications expected: do nothing */	
                }
            } else { /* received a standard message, not a notification */
                /* no standard messages expected: do nothing */
            }
        
        }
        if (tempo==-1){
            print_crono(0);
            cont = 0; tempo = 60;
            comeca = false;
            espera = 2;
            if((Bola.x+13) > 511)
                pontesq++;
            else if((Bola.x+13) <511)
                pontdir++;
            reboot_field();
            copia_buffer();
        }
    }
    }
    //---------------------------------------------
    if(sai() != 0)
        return 1;
    //--------------------------------------------



  return 0;

}

void start_game(){
    //----------------------LE A BOLA--------------------------
    Bola.xi = 499; Bola.yi = 397;
    Bola.x = Bola.xi; Bola.y = Bola.yi;
    int x, y;
    geraXY(&x, &y);
    Bola.dx = (x / abs(x)) * 4; 
    if(y != 0)
        Bola.dy = (y/ abs(y)) * 4;
    else
        Bola.dy = 0;
    //---------------------------------------------------------
    //----------------------LE AS BARRAS-----------------------
    BarraEsq.xi = 51; BarraEsq.yi = 335;
    BarraEsq.x = BarraEsq.xi; BarraEsq.y = BarraEsq.yi; 
    //-------------------
    BarraDir.xi = 957; BarraDir.yi = 335;
    BarraDir.x = BarraDir.xi; BarraDir.y = BarraDir.yi;
    //---------------------------------------------------------
    //-------------------LE OS PONTOS--------------------------
    PontEsq.xi = 127; PontEsq.yi = 15;
    //-------------------
    PontDir.xi = 858; PontDir.yi = 15;
    //---------------------------------------------------------
    //-----------------LE O TEMPO------------------------------
    TempEsq.xi = 483; TempEsq.yi = 15;
    //-------------------
    TempDir.xi = 520; TempDir.yi = 15;
    //---------------------------------------------------------
    //---------------------------------------------------------
    draw_field();
    draw_xpm(l_pressW,182,170,l_pressW_alt,l_pressW_cmp);
    draw_xpm(l_pressSeta,600,170,l_pressSeta_alt,l_pressSeta_cmp);
}
