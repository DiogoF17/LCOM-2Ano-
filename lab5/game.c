#include "game.h"

struct CoordIni Bola;
struct CoordIni BarraEsq;
struct CoordIni BarraDir;
struct CoordIni PontEsq; 
struct CoordIni PontDir;
struct CoordIni TempEsq;
struct CoordIni TempDir;

int pontesq = 0;
int pontdir = 0;

uint32_t hook_idK;
uint8_t scancode;
int hook_idT;       //contem o identificador proveniente do hook_id
extern int cont;       //usada como contador global

//-----------------------------------------------------------

void clean_crono(int tempo){
    int partesq, partdir;
    partesq = tempo / 10;
    partdir = tempo % 10;
    clean_xpm(numeros[partesq], TempEsq.xi, TempEsq.xi);
    clean_xpm(numeros[partdir], TempDir.xi, TempDir.xi);
}

void print_crono(int tempo){
    int partesq, partdir;
    partesq = tempo / 10;
    partdir = tempo % 10;
    draw_xpm(numeros[partesq], TempEsq.xi, TempEsq.yi);
    draw_xpm(numeros[partdir], TempDir.xi, TempDir.yi);
}

//---------------------------------------------------------------

void draw_field(){
    draw_xpm(campo, 0, 0);   //desenha o campo
    draw_xpm(barra, BarraEsq.xi, BarraEsq.yi); //desenha barras
    draw_xpm(barra, BarraDir.xi, BarraDir.yi); //desenha barras
    draw_xpm(bola, Bola.xi, Bola.yi);
    draw_xpm(numeros[pontesq], PontEsq.xi, PontEsq.yi);
    draw_xpm(numeros[pontdir], PontDir.xi, PontDir.yi);
    print_crono(60);
}

void reboot_field(){
    clean_xpm(campo, 0, 0);
    clean_xpm(barra, BarraEsq.x, BarraEsq.y);
    clean_xpm(barra, BarraDir.x, BarraDir.y);
    clean_xpm(bola, Bola.x, Bola.y);
    clean_xpm(numeros[0], PontEsq.xi, PontEsq.yi);
    clean_xpm(numeros[0], PontDir.xi, PontDir.yi);
    draw_field();
}

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
    if((Bola.y + 25) >= 721){
        Bola.y = 721 - 25;
        Bola.dy = -(Bola.dy);
    }
}

void checkLateralBarColision(){
    if((Bola.x + 25) >= BarraDir.xi){ //25 largura da bola
        if(BarraDir.y <= Bola.y && Bola.y <= (BarraDir.y + 150)){ //150 comprimento da barra
            Bola.x = BarraDir.xi - 25;
            Bola.dx = -(Bola.dx);
        }
    }
    if(Bola.x <= (BarraEsq.xi + 17)){ //17 largura da barra
        if(BarraEsq.y <= Bola.y && Bola.y <= (BarraEsq.y + 150)){
            Bola.x = BarraEsq.xi + 17;
            Bola.dx = -(Bola.dx);
        }
    }
}

bool move_bola(){
    int xant = Bola.x, yant = Bola.y;
    Bola.x += Bola.dx;
    Bola.y += Bola.dy;
    checkLateralBarColision();
    checkLineColision();
    if(checkGoal())
        return true;
    clean_xpm(bola, xant, yant);
    draw_xpm(bola, Bola.x, Bola.y);
    return false;
}

void geraXY(int *x, int *y){
    srand(time(NULL));
    *x = (rand() % 20) - 10;
    while(x == 0)
        *x = (rand() % 20) - 10;
    *y = (rand() % 20) - 10;
}

void start_game(){
    //----------------------LE A BOLA--------------------------
    Bola.xi = 499; Bola.yi = 397;
    Bola.x = Bola.xi; Bola.y = Bola.yi;
    int x, y;
    geraXY(&x, &y);
    Bola.dx = (x / abs(x)) * 2; Bola.dy = (y/ abs(y)) * 2;
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
}

int game(){

    int ipc_status, tempo = 60, espera = 3, x, y;
    message msg;
    uint32_t r, irq_set_kbc = 2, irq_set_timer = 1;
    uint8_t bit_no = 1, bit_noT = 0;
    bool comeca = true; //passado tres segundos comeca o jogo

    //-----------------------------------------------------
    if(graphic_mode(0x105) != 0){
        vg_exit();
        return 1;
    }

    if(timer_subscribe_int(&bit_noT) != 0)
        return 1;
 
    if(kbc_subscribe(&bit_no) != 0)
        return 1;

    //------------------------------------------------------

    start_game(); // comeca o jogo;

    //-------------------------------------------------------

    while( tempo > 0 ) { /* You may want to use a different condition */
      /* Get a request message. */
      if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
          printf("driver_receive failed with: %d", r);
          continue;
      }
      if (is_ipc_notify(ipc_status)) { /* received notification */
          switch (_ENDPOINT_P(msg.m_source)) {
              case HARDWARE: /* hardware interrupt notification */				
                  if (msg.m_notify.interrupts & irq_set_kbc) { /* subscribed interrupt */ 
                      //kbc_ih();
                  }
                  if((msg.m_notify.interrupts & irq_set_timer)){
                      timer_int_handler();
                      //espera tres segundos antes de comecar a movimentar a bola
                      if((cont % (sys_hz()*3) == 0) && comeca){
                        comeca = false;
                        espera--;
                        cont = 0;
                      }
                      if(!comeca){
                          //sempre que passa um segundo imprime
                        if(cont % sys_hz() == 0){
                            tempo--;
                            clean_crono(tempo + 1); print_crono(tempo);
                        }
                        //a cada tres segundos aumenta em um o deslocamento da bola
                        if(cont % (sys_hz()*3) == 0){
                            if(Bola.dx > 0)
                                Bola.dx = Bola.dx + 1;
                            else
                                Bola.dx = Bola.dx - 1;
                            if(Bola.dy > 0)
                                Bola.dy = Bola.dy + 1;
                            else
                                Bola.dy = Bola.dy - 1;
                        }
                        //movimenta a bola e se for golo volta as posicoes iniciais
                        if(move_bola()){
                            clean_crono(tempo);
                            reboot_field();
                            cont = 0;tempo = 60;
                            comeca = true;
                            espera = 3;
                            Bola.x = Bola.xi;
                            Bola.y = Bola.yi;
                            geraXY(&x, &y);
                            Bola.dx = (x / abs(x)) * 2;
                            if(y != 0)
                                Bola.dy = (y / abs(y)) * 2;
                            else
                                Bola.dy = 0;
                            if(pontesq == 3 || pontdir == 3)
                                tempo = 0;
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

  printf("\nfora do ciclo\n");

  //---------------------------------------------
  if(timer_unsubscribe_int() != 0)
    return 1;

  if(kbc_unsubscribe()!=0)
    return 1;

  if(vg_exit() != 0)
    return 1;

  printf("fim");

  //--------------------------------------------

  return 0;

}
