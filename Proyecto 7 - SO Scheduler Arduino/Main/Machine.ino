#include "RTClib.h"
#include "Machine.h"

RTC_DS3231 rtc;
  
#define  _MIN_SPIN     DEFAULT_MIN_SPIN_DURATION
#define  _SPIN_DELAY   DEFAULT_SPIN_DELAY

void turnOnMachine(Machine *m){
  randomSeed(rtc.now().second());
  m->t.end = DEFAULT_PLAY_DURATION;
  int spinningWheels = ( m->t.end - _MIN_SPIN ) / _SPIN_DELAY + 1; // wheels[0].delay = 0
  spinningWheels = ( spinningWheels > m->wCount ) ? m->wCount : spinningWheels; 
  int spinDuration = m->t.end - spinningWheels * _SPIN_DELAY;
    
  for ( int i=0 ; i<spinningWheels ; i++ ) {
    Wheel *w = &m->wheels[i];
    w->value = random(w->valuesPerWheel);
    w->t.start = i * _SPIN_DELAY;
    w->t.end = w->t.start + spinDuration ;
    w->f = DEFAULT_FUNCTION;
    showWheel(m,i); 
  }
  m->wheels[ SIZE(m->wheels) - 1 ].f = DEFAULT_LAST_FUNCTION;
}

void spin (Machine *m) {
  play(&m->t);
  for(int w=0 ; w<m->wCount ; w++)
    m->wheels[w].randSpeed = random(m->randSpeedFactor);
}

#define MUST_SPIN(w,snapShot) clockSignal(w, snapShot, w->maxPeriod)
#define SPIN(w) ( w->value = (w->value + 1) % w->valuesPerWheel )

int continueSpinning(Machine *m) {
  int snapShot = elapsed(&m->t);
  for ( int w=0 ; w < SIZE(m->wheels) ; w++ ) {
    Wheel *wheel = &m->wheels[w];
    if ( MUST_SPIN(wheel,snapShot) ) {
      SPIN(wheel);
      showWheel(m,w);
    }
  }
  return snapShot;
}

int clockSignal(Wheel *w, int snapShot, int maxPeriod) {
  if ( isOutside(&w->t, snapShot) )
    return FALSE;
  double x = snapShot - w->t.start + w->randSpeed;
  double e = 2.71828;
  int period = 1;
  if ( w->f == POLYNOMIAL )
    period = w->A * (x*x) + w->B * x + w->C; 
  else if ( w->f == EXPONENTIAL )
    period = w->A * pow(e, w->B * x + w->C)+ w->D; 
  period = ( maxPeriod > period ) ? period : maxPeriod;
  return isMultiple(snapShot, period);
}

BOOL isMultiple(int a, int b) {
   if ( b > 2 )
     return ! (a % b);
  return TRUE;
}

void setWheelPeriod(Machine *m, int w, Function f) {
  Wheel *wheel = &m->wheels[w];
  wheel->f = f;
}

Function getWheelPeriod(Machine *m, int w) {
  return m->wheels[w].f;
}


/* ------------------------------------------------------------------------*/
/* - Por completitud. Leer comenario en la definicion de "struct Machine" -*/
/* ------------------------------------------------------------------------*/
  
void setType(Machine *m, MachineType type) {
  m->type = type;
}

void setPlayDuration(Machine *m, int duration) {
  m->t.end = duration;
}

int getPlayDuration(Machine *m) {
  return m->t.end;
}

  /*
#define FUNCTION(name) int get##name(Machine *m) { return m->##name; }
FUNCTION(wCount)
*/
