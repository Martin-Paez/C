#include "LcdMachine.h"
#include "ZLib.h"

enum LcdId { MACHINE, SEL_PERIOD, SEL_PLAY_DURATION };

#define winPos      6,0
#define MENU_ROWS   8
#define LCD_COLS    16
#define LCD_ROWS    4
#define G_COLS      3
#define G_ROWS      4
#define M_LEFT   0
#define M_TOP    0
#define G_LEFT      (LCD_COLS-G_COLS)/2
#define G_TOP       0
#define INDENTATION         2
#define LAST_NOT_INDENTED   3
// menuM se modifica en conjunto con configParameters()
char menuM[1][MENU_ROWS][LCD_COLS] = {    
    "1)T TOTAL",                            
    "2)RAND FACTOR",
    "3)SELEC RUEDA",
    "  4)CANT FIG", 
    "  5)MAX PERIODO",
    "  6)RETARDO",
    "  7)T MAX",
    "8)SALIR" };

struct MachineLcdRsrc {
  char inputM[1][1][LCD_COLS] = {"               "};
  char gameM[1][G_ROWS][G_COLS] = {"  ","  ","  ","  "};
  Machine m;
  char *scroll;
  State state = TERMINATED;
  int dataId;
  int screenPos = 0;
  int selectedWheel = 0;
};

Machine *lcdM;
MachineLcdRsrc *lcdMR;

#define MIN_ADDRESS(win) (&win[0][0][0])
#define MAX_MENU_ADD ( MIN_ADDRESS(menuM) + ( MENU_ROWS - LCD_ROWS ) * LCD_COLS )

struct MachineLcdRsrc mLcdPs[LCD_MACHINES_COUNT];
int mLCount = 0;

int exeLcdMachine(LiquidCrystal *lcd){
  if ( mLCount == LCD_MACHINES_COUNT )
    return machineError(1);
  int i=-1;
  while( ++i < SIZE(mLcdPs) && mLcdPs[i].state != TERMINATED );
  if ( i == SIZE(mLcdPs) ) // WARNING : error conteo en lcdRsrcCount
    return machineError(2); 
  int psId = createProcess(refreshMachines, FALSE);
  if ( ! psId )
    return machineError(3);
  if ( ! addLcdResource(lcd, MIN_ADDRESS(mLcdPs[i].gameM), LCD_ROWS, LCD_COLS, 1) 
     || ! addResourceTo(psId, getReadyMachineLCD, blockMachineLCD
                       , killMachineLCD, (void *) &mLcdPs[i] ) ) {
    killPs(psId);
    return machineError(4);
  }
  mLCount++;
  mLcdPs[i].scroll = MIN_ADDRESS(menuM); // Inicializar igual, se reutilizan
  mLcdPs[i].dataId = i;
  getReadyMachineLCD((void *) &mLcdPs[i]);
  setType(lcdM,LCD);
  moveAndResize(G_LEFT,G_TOP,G_COLS,G_ROWS);
  turnOnMachine(lcdM);
  return psId;
}

void refreshMachines(char keyPressed) {
  if ( keyPressed == '+' ) {
    if ( addThread(configMenuLoop, TRUE) ) {
      blockActiveThread();
      redirectScreen(lcdMR->scroll);
      moveAndResize(M_LEFT,M_TOP,LCD_COLS,LCD_ROWS);
    }
  } else if ( isRunning(&lcdM->t) ) 
    continueSpinning(lcdM);
  else if ( keyPressed == 'C' )
    spin(lcdM);
  refreshScreen();
}

void configMenuLoop(char keyPressed) {
  String current;
  switch( keyPressed ) {
    case '+':
        if ( lcdMR->scroll + LCD_COLS <= MAX_MENU_ADD ) {
          redirectScreen(lcdMR->scroll += LCD_COLS);
          lcdMR->screenPos++;
        }
        break;
    case '-':
        if ( lcdMR->scroll - LCD_COLS >= MIN_ADDRESS(menuM)  ) {
          redirectScreen(lcdMR->scroll -= LCD_COLS);
          lcdMR->screenPos--;
        }
        break;
    case '1':
          current = String(getPlayDuration(lcdM));
          configParameter("Millis: ",keyPressed-'0', 5, current, setDurationListener);
        break;
    case '2':
          current = String(lcdM->randSpeedFactor);
          configParameter("",keyPressed-'0', 5, current, setRandSpeedFactor);
        break;
    case '3':
          current = String(lcdMR->selectedWheel + 1);
          configParameter("Rueda(1-3):", keyPressed-'0',1, current, selectWheel);
        break;
    case '4':
          current = String(lcdM->wheels[lcdMR->selectedWheel].valuesPerWheel);
          configParameter("(1-10):",keyPressed-'0', 2, current, setValuesPerWheel);
          spin(lcdM);
        break;  
    case '5':
          current = String(lcdM->wheels[lcdMR->selectedWheel].maxPeriod);
          configParameter("Millis:",keyPressed-'0', 3, current, setMaxPeriod);
        break;
    case '6':
          current = String(lcdM->wheels[lcdMR->selectedWheel].t.start);
          configParameter("Millis:",keyPressed-'0', 4, current, setDelay);
        break;
    case '7':
          current = String(lcdM->wheels[lcdMR->selectedWheel].t.end);
          configParameter("Millis:",keyPressed-'0', 5, current, setEnds);
        break;
    case '8':
        clrLCD();
        killActiveThread();
        redirectScreen(MIN_ADDRESS(lcdMR->gameM));
        moveAndResize(G_LEFT,G_TOP,G_COLS,G_ROWS);
        lcdMR->screenPos=0;
        lcdMR->selectedWheel=0;
        break;
  }
  
  refreshWindow();
}

void setMenuWindow() {
  clrLCD();
  clrMem();
  redirectScreen(lcdMR->scroll);
  moveAndResize(M_LEFT,M_TOP,LCD_COLS,LCD_ROWS);
  enableContinueBelow();
  refreshWindow();
}

void configParameter(String msg, int opt, int digits, String currentVal, void (*listener)(int value)) {
  if ( opt < 1 || opt > (MENU_ROWS-1) )
    return;
  int col = INDENTATION;
  if ( opt > LAST_NOT_INDENTED )
    col = INDENTATION * 2;
  int row = opt - lcdMR->screenPos - 1;
  int len = LCD_COLS - col;
  redirectScreen(MIN_ADDRESS(lcdMR->inputM));
  moveAndResize(col,row,len,1);
  clrMem();
  disableContinueBelow();
  if ( ! modifN(0,0,msg, currentVal, digits, listener, TRUE) )
    setMenuWindow();
}

void setEnds(int value) {
  if ( value < 0 )
    value = 0;
  setMenuWindow();
  lcdM->wheels[lcdMR->selectedWheel].t.end = value;   
}

void setDelay(int value) {
  if ( value < 0 )
    value = 0;
  setMenuWindow();
  lcdM->wheels[lcdMR->selectedWheel].t.start = value;   
}

void setMaxPeriod(int value) {
  if ( value < 0 )
    value = 2;
  setMenuWindow();
  lcdM->wheels[lcdMR->selectedWheel].maxPeriod = value;   
}

void selectWheel(int value) {
  if ( value > WHEELS_PER_MACHINE || value < 1)
    value = WHEELS_PER_MACHINE;
  setMenuWindow();
  lcdMR->selectedWheel = value - 1;  
}

void setRandSpeedFactor(int value) {
  setMenuWindow();
  lcdM->randSpeedFactor = value;  
}

void setValuesPerWheel(int value) {
  if ( value > 10 )
    value = 10;
  else if ( value < 1 )
    value = 1;
  setMenuWindow();
  lcdM->wheels[lcdMR->selectedWheel].valuesPerWheel = value;  
}

void setDurationListener(int value) {
  if ( value < 0 )
    value = 0;
  setMenuWindow();
  setPlayDuration(lcdM, value); 
}

void showWheel(Machine *m, int w) {
  if (  m->type == LCD )
    showLcdWheel(m, w);
} 

void showLcdWheel(Machine *m, int w) {
  for ( int i=1 ; i<=G_ROWS ; i++ ) {
    int value = ( m->wheels[w].value + i ) % m->wheels[w].valuesPerWheel;
    printN(w , G_ROWS - i, value);
  }
}

BOOL machineError(int err){
    powerOnErrorLcd->begin(16,4);
    powerOnErrorLcd->setCursor(1,0);
    powerOnErrorLcd->print("No enciende la ");
    powerOnErrorLcd->setCursor(2,1);
    powerOnErrorLcd->print("maquina LCD");
    powerOnErrorLcd->setCursor(2,3);
    powerOnErrorLcd->print("ErrorCode: " + String(err));
    return FALSE;  
}

/* TODO
 *    void getReadyMachineLCD(void *m) {
 *      lcdMR = ((struct MachineLcdRsrc *) m);
 *      lcdMR->state = READY;
 *      lcdM = &lcdMR->m;
 *    }
 */
void getReadyMachineLCD(void *m) {
  lcdM = &((struct MachineLcdRsrc *) m)->m;
  lcdMR = ((struct MachineLcdRsrc *) m);
  lcdMR->state = READY;
}

void blockMachineLCD(void *m) {
  lcdM = NULL;
  lcdMR = NULL;
  ((struct MachineLcdRsrc *) m)->state = BLOCKED;
}

void killMachineLCD(void *m) {
  lcdM = NULL;
  lcdMR = NULL;
  ((struct MachineLcdRsrc *) m)->state = TERMINATED;
  mLCount--;
}
