#include "ZLib.h"
#include "ZKernel.h"

/* ------------------------------------------------------------------------*/
/* -------------------------------- Timer ---------------------------------*/
/* ------------------------------------------------------------------------*/

BOOL isRunning(struct Timer *t) {
  return t->start != STOPPED;
}

void play(struct Timer *t) {
  t->start = millis();
}

int elapsed(struct Timer *t) {
  unsigned int lapse = millis() - t->start;
  if ( lapse >= t->end) {
    t->start = STOPPED;
    return t->end;   // importante devolver LIMIT y no STOPPED
  }
  return lapse;
}

BOOL isOutside(Lapse *lapse, unsigned int snapShot) {
  return snapShot < lapse->start || snapShot > lapse->end;
}
