#ifndef LIB_A_H
#define LIB_A_H

#include "ZKernel.h"

/***************************************************************************/
/* ------------------------------- ZLib.h ---------------------------------*/
/* --------------- BIBLIOTECA DE FUNCIONES PARA EL USUARIO ----------------*/
/* ------------------------------------------------------------------------*/
/***************************************************************************/


/* ------------------------------------------------------------------------*/
/* ---------------------- Administracion del tiempo -----------------------*/
/* ------------------------------------------------------------------------*/


/* ---------------------------- Cronometro --------------------------------*/

    #define STOPPED 0  
    struct Timer {
      unsigned int start = STOPPED;
      unsigned int end = 0;
    };
    
    BOOL isRunning(struct Timer *t);
    void play(struct Timer *t);
    int elapsed(struct Timer *t);


/* ------------------------- Lapso de tiempo -----------------------------*/
  
    typedef Timer Lapse;
    
    BOOL isOutside(Lapse *lapse, unsigned int snapShot);


/* ------------------------------------------------------------------------*/
/* ------------------------------- Keypad ---------------------------------*/
/* ------------------------------------------------------------------------*/


    /* Importante : Maxima cantidad en simultaneo de llamados a modifN */
    #define MODIF_N_RSRC    4
    
    BOOL modifN(int c, int f, String msg, String original, int digits
                , void (*listener)(int), BOOL restrictive );


/* ------------------------------------------------------------------------*/
/* --------------------------------- LCD ----------------------------------*/
/* ------------------------------------------------------------------------*/

            
        union Data {
            char            *buff;
            struct Node     *tabs;
        };

        struct ScreenElem {
            Data      e;
            uBYTE     cols;
            uBYTE     rows;
            BYTE      x = 0;
            BYTE      y = 0;
        };

    #define _MOVE_INDEX(n,i,L)   if ( i + n <= L  && i + n >= -L ) i += n;
    #define _TABS(w)            (w)->e.tabs
    #define _TAB(i,w)           ( (Tab*) ( getFromLast(i, _TABS(w))->e ) )
    #define _BUFF(tab)          (char *)( (tab)->e.buff + (tab)->y * (tab)->cols + (tab)->x )

    #define W_POS(w)            (w)->x ,  (w)->y
    #define W_SIZE(w)           (w)->cols ,  (w)->rows
    #define V_MOVE(n,e)         _MOVE_INDEX( n, (e)->y, (e)->rows )
    #define H_MOVE(n,e)         _MOVE_INDEX( n, (e)->x, (e)->cols )
   
    #define BUFF(i,w)           _BUFF( _TAB(i,w) )
    #define V_SCROLL(n,i,w)     _MOVE_INDEX( n, _TAB(i,w)->y, (w)->rows )
    #define H_SCROLL(n,i,w)     _MOVE_INDEX( n, _TAB(i,w)->x, (w)->cols )
     
    typedef struct ScreenElem Window;
    typedef struct ScreenElem Tab;


    /* Importante : Maxima cantidad de LiquidCrystals */
    #define LCD_RESOURCES     2
    BOOL addLcdResource (LiquidCrystal *lcd, char *sourceMatrix, int cols, int rows, int windows);

    BOOL initLcd(LiquidCrystal *lcd, char *sourceMatrix, int cols, int rows, int windows) ;

    void refreshScreen();
    void refreshWindow();

    void redirectScreen(char *sourceMatrix);
    void moveAndResize(int left, int top, int c, int f);
    
    void printS(int c, int f, String s);
    void printN(int c, int f, int i);

    void clrLCD();
    void clrMem();

    void showCursor();
    void hideCursor();
    void moveCursor(int c, int f);

    void disableContinueBelow();
    void enableContinueBelow();
    
    void printDebugAt(int c, int f, String s, int duration);


/* ------------------------------------------------------------------------*/
/* ------------------------ INTERFAZ - SYSCALLS ---------------------------*/
/* ------------------------------------------------------------------------*/

    #define LISTENERS(f,g,h)  void (*f)(void*), void (*g)(void*), void (*h)(void*)

    int createProcess(void (*ps)(char k), BOOL hasParent);
    BOOL addThread(void (*run)(char key), BOOL saveParent);
    int addResource(LISTENERS(ready, block, kill), void *data);
    int addResourceTo( int psId, LISTENERS(ready, block, kill), void *data );
                        
    void killPs(int psId);
    BOOL killThread(int psId, int thId);
    BOOL killActiveThread();
    int removeActivePsResource(int id);
    
    int blockActiveThread();
    BOOL wakeUpThread(int psId, int thId);
    
    int getActivePsId();


#endif
