#include "ZLib.h"
#include "ZKernel.h"

/* ------------------------------------------------------------------------*/
/* --------------------------------- LCD ----------------------------------*/
/* ------------------------------------------------------------------------*/

    struct Cursor {
      int x=0;
      int y=0;
      BOOL isBlinking = FALSE;
    };
    
    struct LcdResource {
      char *sourceMatrix;       // screen[WINDOW][LCD_ROWS][LCD_COLS]
      Cursor cursor;
      int window = 1;
      LiquidCrystal *lcd;
      int rows;
      int cols;
      int wRows;
      int wCols;
      int wCount = 1;
      int top = 0;
      int left = 0;
      BOOL continueBelow = FALSE;
      State state = TERMINATED;
    };

    void getReadyLCD(void *lcdResource);
    void blockLCD(void* lcd);
    void killLCD(void* lcd);


    LcdResource lcdRsrc[LCD_RESOURCES];
    struct LcdResource *_lcd;
    int lcdRsrcCount = 0;                            


void redirectScreen(char *sourceMatrix);
void resizeAndMove(int left, int top, int c, int f);
void selectWindow(int w);

void moveCursor(int c, int f);
void setScreen(char c);
void clrMem();

void refreshWindow();
void refreshScreen();
void refreshCursor();

void printNext(String s);
void printS(int c, int f, String s);
void printN(int c, int f, int i);
void printDebugAt(int c, int f, String s, int duration);

void disableContinueBelow();
void enableContinueBelow();

BOOL addLcdResource (LiquidCrystal *lcd, char *sourceMatrix, int rows, int cols, int windows);
void _getReadyLCD(void* rsrc);
void _blockLCD(void *rsrc);
void _killLCD(void *rsrc);

void clrLCD();
void showCursor();
void hideCursor();


#define _SCREEN  _lcd->sourceMatrix
#define _COLS    _lcd->cols
#define _ROWS    _lcd->rows
#define _W_COLS  _lcd->wCols
#define _W_ROWS  _lcd->wRows
#define _CURSOR  _lcd->cursor
#define _WINDOWS _lcd->wCount
#define _WIN     _lcd->window
#define _LCD     _lcd->lcd
#define _TOP     _lcd->top
#define _LEFT    _lcd->left


void redirectScreen(char *sourceMatrix) {
  _SCREEN = sourceMatrix;
}

void selectWindow(int w) {
  if ( w>0 && w<_WINDOWS )
    _WIN = w;
}

#define MIN(a,b)  ( ( a < b ) ? a : b )
void moveAndResize(int left, int top, int cols, int rows) {
  _TOP = top;
  _LEFT = left;
  _W_COLS = MIN(cols, _COLS);
  _W_ROWS = MIN(rows, _ROWS);
}

void moveCursor(int c, int f) {
  if ( c < 0 || c >=_W_COLS || f < 0 || f >= _W_ROWS )
    return;
  _CURSOR.x = c + _LEFT;
  _CURSOR.y = f + _TOP;  
}

void writeScreenBuff(char c) {
  char *p = &_SCREEN[_WIN];
  int y = _CURSOR.y - _TOP;
  int x = _CURSOR.x - _LEFT;
  *( p + y * _W_COLS + x ) = c;
  if ( ++x == _W_COLS ) {
    x = (_lcd->continueBelow) ? 0 : _W_COLS - 1;
    if ( ++y == _W_ROWS )
      y = (_lcd->continueBelow) ? 0 : _W_ROWS - 1;      
  }
  _CURSOR.y = y + _TOP;
  _CURSOR.x = x + _LEFT;
}

void refreshWindow() {
  char *p = &_SCREEN[_WIN];
  for ( int y=_TOP; y<_TOP + _W_ROWS; y++) {
    _LCD->setCursor(_LEFT,y);
    for ( int x=_LEFT; x<_LEFT + _W_COLS; x++)
        _LCD->print(*(p++));
  }
  refreshCursor();   
}

void refreshScreen() {
  char *p = &_SCREEN[_WIN];
  for ( int y=0 ; y<_ROWS; y++) {
    _LCD->setCursor(0 ,y);
    for ( int x=0 ; x<_COLS; x++) {
      if ( x >= _LEFT && x < (_LEFT + _W_COLS) 
          && y >= _TOP && y < (_TOP + _W_ROWS) )
        _LCD->print(*(p++));
      else
        _LCD->print(' ');
    }
  }
  refreshCursor();
}

void clrMem() {
  char *p = &_SCREEN[_WIN];
  for ( int i=0; i<_W_ROWS*_W_COLS; i++)
    *(p++)=' ';
}

void refreshCursor() {
  _LCD->setCursor(_CURSOR.x, _CURSOR.y);
}

void printDebugAt(int c, int f, String s, int duration) {
  _LCD->setCursor(c,f);
  _LCD->print(s);
  delay(duration);
}

void printNext(String s) {
  for ( unsigned int i = 0 ; i < s.length() ; i++ )
    writeScreenBuff(s[i]);
}

void printS(int c, int f, String s) {
  moveCursor(c,f);
  printNext(s);
}

void printN(int c, int f, int i) {
  printS(c,f, String(i) );
}

void disableContinueBelow(){
  _lcd->continueBelow = FALSE;
}

void enableContinueBelow(){
  _lcd->continueBelow = TRUE;
}

BOOL selectLcd(int id) {
  if ( --id > 0  &&  id < lcdRsrcCount  )
    return FALSE;
  
}

int initLcd(LiquidCrystal *lcd, char *sourceMatrix, int cols, int rows, int windows) {
  if ( lcdRsrcCount == LCD_RESOURCES )
    return FALSE;
  int i=-1;
  while( ++i < SIZE(lcdRsrc) && lcdRsrc[i].state != TERMINATED );
  if ( i == SIZE(lcdRsrc) ) // WARNING : error conteo en lcdRsrcCount
    return FALSE; 
  lcdRsrcCount++;
  lcdRsrc[i].sourceMatrix = sourceMatrix;
  lcdRsrc[i].lcd = lcd;
  lcdRsrc[i].rows = rows;
  lcdRsrc[i].cols = cols;
  lcdRsrc[i].wRows = rows;
  lcdRsrc[i].wCols = cols;
  lcdRsrc[i].wCount = windows;
  lcdRsrc[i].cursor.x = 0;
  lcdRsrc[i].cursor.y = 0;
  lcdRsrc[i].window = 0;
  lcdRsrc[i].top = 0;
  lcdRsrc[i].left = 0;
  lcdRsrc[i].state = BLOCKED;
  lcd->begin(cols, rows);
  _getReadyLCD( (void *)&lcdRsrc[i] );
  return i;
} 

BOOL addLcdResource (LiquidCrystal *lcd, char *sourceMatrix, int cols, int rows, int windows) {
  int i = initLcd(lcd, sourceMatrix, rows, cols, windows);
  if ( ! i )
    return FALSE;
  if ( ! addResource(_getReadyLCD, _blockLCD, _killLCD, (void *)&lcdRsrc[i] ) ) {
    lcdRsrc[i].state = TERMINATED;
    return FALSE; 
  }
  lcdRsrc[i].state = READY;
  return TRUE;
}

void _getReadyLCD(void* rsrc) {
  _lcd = (struct LcdResource *) rsrc;
  ((struct LcdResource *) rsrc)->state = READY;
  refreshCursor();
}

void _blockLCD(void *rsrc) {
  _lcd = NULL;
  ((struct LcdResource *) rsrc)->state = BLOCKED;
}

void _killLCD(void *rsrc) {
  _lcd = NULL;
  ((struct LcdResource *) rsrc)->state = TERMINATED;
  lcdRsrcCount--;
}

void clrLCD() {
  _LCD->begin(_COLS,_ROWS);
}

void showCursor() {
  _CURSOR.isBlinking = TRUE;
  _LCD->blink();
}

void hideCursor() {
  _CURSOR.isBlinking = FALSE;
  _LCD->noBlink();
}
