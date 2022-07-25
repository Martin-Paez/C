#include "ZKernel.h"
#include "LcdMachine.h"
#include "Demultiplexer.h"
#include "ZLib.h"
#include "List.h"

char bb[1][4][16] = { "ccccccccccccccc", "ddddddddddddddd", "ccccccccccccccc","ddddddddddddddd",};

LiquidCrystal myLcd(TX,RX,D4,D5,D6,D7);
Window *w;

void setup() {
  initDemultiplexer();
  selectOutput(3);
  initLcd(&myLcd, &bb[0][0][0], w->cols, w->rows, 1);
  w = newEmptyWindow(0,0,16,2);
  loadTab(w,16,4,&bb[0][0][0]);
  /*int psId;
  initDemultiplexer();
  selectOutput(3);
  psId = exeLcdMachine( &machineLcd );
  if ( ! psId || ! addDemultiplexer(psId, 3) )
    while(1);
  selectOutput(0);
  psId = exeLcdMachine( &myLcd );
  if ( ! psId || ! addDemultiplexer(psId, 0) )
    while(1);*/
}

void loop()
{
  //kernelLoop();  
  moveAndResize(W_POS,W_SIZE);
  redirectScreen( BUFF(0,w) );
  refreshScreen();
  switch ( keypad.getKey() ) {
    case '8':
      V_MOVE(1,w);
      break;
    case '5':
      V_MOVE(-1,w);
      break;
    case '4':
      H_MOVE(-1,w);
      break;
    case '6':
      H_MOVE(1,w);
      break;
    case '2':
      V_MOVE(1,_TAB(0,w));
      break;
    case '0':
      V_MOVE(-1,_TAB(0,w));
      break;
    case 'C':
      H_MOVE(-1,_TAB(0,w));
      break;
    case '=':
      H_MOVE(1,_TAB(0,w));
      break;
  } 
}

