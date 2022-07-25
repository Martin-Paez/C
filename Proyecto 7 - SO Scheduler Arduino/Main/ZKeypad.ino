#include "ZLib.h"
#include "ZKernel.h"

/* ------------------------------------------------------------------------*/
/* ------------------------------- Keypad ---------------------------------*/
/* ------------------------------------------------------------------------*/

struct ModifNRrsrc {
  int i;
  int c;
  char s[6];
  int digits;
  int f;
  State state = TERMINATED;
  int rId;
  void (*listener)(int);
  int thId;
  int psId;
};

ModifNRrsrc mnRsrc[MODIF_N_RSRC];
int modifNCount = 0;
ModifNRrsrc *mNr;


void modifNLoop(char k);
void readAndShow(char k);
void deleteChar();
void endModifNLoop();
BOOL modifN(int c, int f, String msg, String original
            , int digits, void (*listener)(int), BOOL restrictive );
void modifNgetReady(void* r);
void modifNblock(void *r);
void modifNkill(void *r);



#define IS_NOT_DIGIT(k)  ( k<'0' || k>'9' )
void modifNLoop(char k) {
  switch (k) {
    case '-': deleteChar();     break;
    case '=': endModifNLoop();  break;
    default:
      if ( IS_NOT_DIGIT(k) )
        return; // CURSOR VISIBLE, evita refresh
      readAndShow(k);
  }
  refreshWindow();
}

#define INDEX_POS         mNr->c + mNr->i , mNr->f
#define REMAIN_SPACE      mNr->i + 1 < mNr->digits
#define IS_THE_LAST       mNr->i == mNr->digits - 1
#define IS_NOT_EMPTY      mNr->i > -1
#define CURSOR_BACK       moveCursor( INDEX_POS ); // CURSOR = STACK POINTER
#define DEL_SCREEN_CHAR   printS( INDEX_POS , " "); CURSOR_BACK
#define SHIFT_LEFT        for ( int a = 0 ; a < SIZE(mNr->s)-1 ; a++) mNr->s[a]=mNr->s[a+1]

#define THERE_IS_A_ZERO_TO_THE_LEFT   mNr->s[0] == '0' && IS_NOT_EMPTY

void readAndShow(char k) {
  if ( THERE_IS_A_ZERO_TO_THE_LEFT )
    SHIFT_LEFT;
  else if ( REMAIN_SPACE )
    mNr->i++;

  mNr->s[mNr->i] = k;
  printS( INDEX_POS , String(k) );
  if ( IS_THE_LAST )
    CURSOR_BACK;
}

void deleteChar() {
  if ( IS_NOT_EMPTY ) {
    DEL_SCREEN_CHAR;
    mNr->i--;
  } else
    mNr->i = -1;
}

// Ojo con el orden de estas instrucciones
void endModifNLoop() {
  mNr->s[ ++mNr->i ] = '\0';
  long b = strtol(mNr->s, NULL, 10);
  mNr->listener( (b < 32000) ? (int)b : 32000 );
  wakeUpThread(mNr->psId, mNr->thId);
  removeActivePsResource(mNr->rId);
  killActiveThread();
}

BOOL modifN(int c, int f, String msg, String original, int digits
            , void (*listener)(int), BOOL restrictive ) {
  if ( modifNCount == MODIF_N_RSRC || digits < 1 )
    return FALSE - 1;
  if ( original.length() > (unsigned) digits )
    return FALSE - 2;
  int killIfErr;
  if ( ! ( killIfErr = addThread(modifNLoop, FALSE)) )
    return FALSE - 3;
  int toWakeUpLater = blockActiveThread();
  int i = -1;
  int id;
  while ( ++i < SIZE(mnRsrc) && mnRsrc[i].state != TERMINATED );
  if ( i == SIZE(mnRsrc)  // WARNING : error conteo en modifNCount
       || ! (id = addResource(modifNgetReady, modifNblock, modifNkill
                              , & mnRsrc[i])) ) {
    wakeUpThread(mNr->psId, mNr->thId);
    killThread(getActivePsId(), killIfErr);
    return FALSE - 5;
  }
  mNr = & mnRsrc[i];
  mNr->thId = toWakeUpLater;
  modifNCount++;
  mNr->rId = id;
  mNr->f = f;
  mNr->digits = digits;
  mNr->i = original.length() - 1;
  String txt = msg + original;
  printS(0, 0, txt);
  mNr->c = c + (msg).length();
  original.toCharArray(mNr->s, digits + 1);
  mNr->state = READY;
  mNr->listener = listener;
  mNr->psId = getActivePsId();
  mNr = NULL;
  refreshWindow();
  if ( restrictive )
    _consumeK();
  return TRUE;
}

void modifNgetReady(void* r) {
  mNr = (struct ModifNRrsrc *) r;
  ((struct ModifNRrsrc *) r)->state = READY;
  showCursor();
}

void modifNblock(void *r) {
  mNr = NULL;
  ((struct ModifNRrsrc *) r)->state = BLOCKED;
  hideCursor();
}

void modifNkill(void *r) {
  mNr = NULL;
  ((struct ModifNRrsrc *) r)->state = TERMINATED;
  modifNCount--;
}
