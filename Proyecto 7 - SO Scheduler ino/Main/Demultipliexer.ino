#include "Demultiplexer.h"
#include "ZLib.h"


    struct DemultiplexRsrc {
      char pinSelector;
      State state = TERMINATED;
    };

    struct DemultiplexRsrc demultiplexRsrc[4];
    int demultiplexCount = 0;



void selectOutput(int code) {
  // IMPORTANTE: NO CAMBIAR EL ORDEN DE SETEO
  // PRIMERO B PARA 3 Y 2, PRIMERO A PARA 1 Y 0
  // PUEDEN HABER OTRAS QUE FUNCIONEN , PERO NO TODAS

  switch(code) {
    case 3:
      digitalWrite(DEMULTIPLEXOR_B,HIGH);
      digitalWrite(DEMULTIPLEXOR_A,HIGH);
      break;
   case 2:
      digitalWrite(DEMULTIPLEXOR_B,LOW);
      digitalWrite(DEMULTIPLEXOR_A,HIGH);
      break;
   case 1:
      digitalWrite(DEMULTIPLEXOR_A,LOW);
      digitalWrite(DEMULTIPLEXOR_B,HIGH);
      break;
   case 0:
      digitalWrite(DEMULTIPLEXOR_A,LOW);
      digitalWrite(DEMULTIPLEXOR_B,LOW);
      break;
  }
}

void initDemultiplexer(void) {
  pinMode(DEMULTIPLEXOR_A, OUTPUT);
  pinMode(DEMULTIPLEXOR_B, OUTPUT);
}

void getReadyDemultiplex(void *r) {  
  char pin = ( ( struct DemultiplexRsrc * ) r )->pinSelector;
  selectOutput(pin);
}

void blockDemultiplex(void *r) {
  r=r;
}

void killDemultiplex(void *r) {
  ( ( struct DemultiplexRsrc * ) r )->state = TERMINATED;
  demultiplexCount--;
}
   
int addDemultiplexer(int psId, int selector) {
  initDemultiplexer();        //Por las dudas
  if ( demultiplexCount == 4 )
    return FALSE;
  int i=-1;
  while( ++i < SIZE(demultiplexRsrc) && demultiplexRsrc[i].state != TERMINATED );
  if ( i == SIZE(demultiplexRsrc) ) // WARNING : error conteo en multiplexCount
    return FALSE; 
  int rsrcId = addResourceTo( psId, getReadyDemultiplex, blockDemultiplex, killDemultiplex
                        , (void *) &demultiplexRsrc[i] ) ;
  if ( ! rsrcId )
    return FALSE;
  demultiplexRsrc[i].pinSelector = selector;  
  demultiplexRsrc[i].state = READY;
  demultiplexCount++;
  return rsrcId;
}
