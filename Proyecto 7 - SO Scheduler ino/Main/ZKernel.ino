#include "ZKernel.h"
#include "ZLib.h"

#define UNASIGNED  -1

struct Resource {
  void (*getReady)(void*);
  void (*block)(void*);
  void (*kill)(void*);
  State state = TERMINATED;
  void *data;
};
  
struct Thread {
  void (*run)(char keyPressed);
  State state = TERMINATED;
  Thread *parent;
};

struct Pcb {                               // Process Control Block
  struct Thread threads[THREADS_PER_PS];
  int thCount = 0;                         // Threads list size
  int currTh = UNASIGNED;                  // Current thread , running
  Resource rsrc[RESOURCES_PER_PS];
  int rCount = 0;
  State state = TERMINATED;
  int psId;
  Pcb *parent;
};

struct {
  Pcb ps[MAX_PS];                               // Process list
  int psCount=0;                           // Process list size
  int currPs = UNASIGNED;                  // Current process , running
} psTable;                                 // Process table



int addResource(void (*getReady)(void*), void (*block)(void *)
                  ,void (*kill)(void *), void *data) {  
  if ( psTable.currPs == UNASIGNED || getReady == NULL || kill == NULL || block == NULL )
    return FALSE;
  Pcb *pcb = &psTable.ps[ psTable.currPs ];
  if ( pcb->rCount == SIZE(pcb->rsrc) )
    return FALSE; 
  int i = -1 ;
  while( ++i < SIZE(pcb->rsrc) && pcb->rsrc[i].state != TERMINATED );
  if ( i == SIZE(pcb->rsrc) ) // si se procude error seria un warning
    return FALSE;
  pcb->rsrc[i].getReady = getReady;
  pcb->rsrc[i].block = block; 
  pcb->rsrc[i].kill = kill;
  pcb->rsrc[i].data = data;
  pcb->rsrc[i].state = READY;
  pcb->rCount++;
  
  return pcb->rCount;
}

int addResourceTo(int psId, void (*getReady)(void*), void (*block)(void *)
                                            ,void (*kill)(void *), void *data) {  
  if ( ! psTable.psCount )
    return FALSE;
  int i = -1 ;
  while( ++i < SIZE(psTable.ps) && psTable.ps[i].psId != psId );
  if ( i == SIZE(psTable.ps) ) // WARNING : error conteo en thCount
    return FALSE;           
  int backUp = psTable.currPs;
  psTable.currPs = psId - 1;
  int rId = addResource(getReady, block, kill, data);
  psTable.currPs = backUp;
  return rId;
}

int addThread(void (*run)(char key), BOOL saveParent) {
  if ( psTable.currPs == UNASIGNED || run == NULL )
    return FALSE;
  Pcb *pcb = &psTable.ps[ psTable.currPs ];
  if ( pcb->thCount == SIZE(pcb->threads) )
    return FALSE;
  int i = -1 ;
  while( ++i < SIZE(pcb->threads) && pcb->threads[i].state != TERMINATED );
  if ( i == SIZE(pcb->threads) ) // WARNING : error conteo en thCount
    return FALSE; 
  if ( saveParent && pcb->currTh != UNASIGNED )
    pcb->threads[i].parent = & pcb->threads[pcb->currTh];
  else
    pcb->threads[i].parent = NULL;
  pcb->threads[i].run = run;
  pcb->threads[i].state = READY;
  pcb->thCount++;
 
  return i+1;
}

int createProcess(void (*ps)(char k), BOOL saveParent) {
  int i = -1 ;
  while( ++i < SIZE(psTable.ps) && psTable.ps[i].thCount > 0 );
  if ( i == SIZE(psTable.ps) )
    return FALSE;

  Pcb *pcb = &psTable.ps[i];
  if ( saveParent && psTable.currPs != UNASIGNED)
    pcb->parent = & psTable.ps[psTable.currPs];
  else
    pcb->parent = NULL;
  pcb->rCount = 0;
  psTable.psCount++;
  psTable.currPs = i;
  pcb->thCount = 1;
  pcb->threads[0].run = ps;
  pcb->threads[0].state = READY;
  pcb->state = READY;
  pcb->psId = i+1;
  return pcb->psId;
}

int removeActivePsResource(int id) {
  if ( psTable.currPs == UNASIGNED || id < 1 )
    return FALSE;
  Pcb *pcb = &psTable.ps[psTable.currPs];  
  if ( pcb->rCount == 0 || id > SIZE(pcb->rsrc) )
    return FALSE;
  pcb->rsrc[id-1].state = TERMINATED;
  pcb->rsrc[id-1].kill(pcb->rsrc[id-1].data);
  pcb->rCount--;
  return TRUE;
}

BOOL killActiveThread() { 
  if ( psTable.currPs == UNASIGNED )
    return FALSE;
  Pcb *pcb = &psTable.ps[psTable.currPs];
  if ( pcb->currTh == UNASIGNED || pcb->thCount == 0 )
    return FALSE; 
  pcb->threads[pcb->currTh].state = TERMINATED;
  if ( pcb->threads[pcb->currTh].parent != NULL )
    pcb->threads[pcb->currTh].parent->state = READY;
  if ( --pcb->thCount == 0 )
    killEmptyPs();
  return TRUE;
}

BOOL killThread(int psId, int thId) {
  if ( psId < 1 || psId > SIZE(psTable.ps) )
    return FALSE; 
  if ( thId < 1 || thId > SIZE(psTable.ps[psId-1].threads) )
    return FALSE; 
  int backUp = psTable.ps[psId-1].currTh;
  psTable.ps[psId-1].currTh = thId - 1;
  killActiveThread();
  psTable.ps[psId-1].currTh = backUp;
  return TRUE;
}

void killPs(int psId) {
  if ( psId < 1 || psId > SIZE(psTable.ps) )
    return; 
  int backUp = psTable.currPs;
  psTable.currPs = psId - 1;
  Pcb *pcb = &psTable.ps[ psId - 1 ];
  for( int i = 1 ; i <= pcb->rCount ; i++)
    removeActivePsResource(i);
  for( int i = 0 ; i < pcb->thCount ; i++) {
    pcb->currTh = i;
    killActiveThread();
  }
  psTable.currPs = backUp;
}

void killEmptyPs() {
  if ( psTable.currPs == UNASIGNED )
    return;
  Pcb *pcb = &psTable.ps[psTable.currPs]; 
  if ( pcb->thCount > 0 )
    return;
  for ( int i=0 ; i < pcb->rCount ; i++ ) {
    Resource r = pcb->rsrc[i];
    r.kill(r.data);
  }
  pcb->rCount = 0;
  pcb->state = TERMINATED;
  if ( pcb->parent != NULL )
    pcb->parent->state = READY;
  psTable.psCount--;
  psTable.currPs = UNASIGNED;
}

int blockActiveThread() {
  if ( psTable.currPs == UNASIGNED )
    return FALSE -1;
  Pcb *pcb = &psTable.ps[psTable.currPs];  
  if ( pcb->currTh == UNASIGNED )
    return FALSE - 2;
  pcb->threads[pcb->currTh].state = BLOCKED;
  return pcb->currTh+1;
}

int getActivePsId() {
  if ( psTable.currPs == UNASIGNED )
    return FALSE;
  return psTable.ps[psTable.currPs].psId;
}

BOOL wakeUpThread(int psId, int thId) {
  if ( psId < 1 || psId > SIZE(psTable.ps) )
    return -1; 
  if ( thId < 1 || thId > SIZE(psTable.ps[psId-1].threads) )
    return -2; 
  Thread *th = & psTable.ps[psId-1].threads[thId-1];
  th->state = READY;
  return TRUE;
}

BOOL hasParent() {
  return psTable.ps[psTable.currPs].parent != NULL;
}

char k;

void _consumeK() {
  k = keypad.getKey();   // Para simplificar el kernel funciona si o si con teclado
}

void runProcess() {  
  if ( psTable.currPs == UNASIGNED )
    return;
  Pcb *pcb = &psTable.ps[psTable.currPs];
  //if ( pcb->rCount > 0 ) //IMPORTANTE DECICION, DEL ULTIMO AL PRIMERO
    for( int i = 0 ; i < pcb->rCount ; i++ )  
      if ( pcb->rsrc[i].state == READY )
        pcb->rsrc[i].getReady( pcb->rsrc[i].data );
  for( int i = 0 ; i < pcb->thCount ; i++ ) {
    if( pcb->threads[i].state == READY ) {
      pcb->currTh = i;
      pcb->threads[i].run(k);
    }
  }
  for( int i = 0 ; i < pcb->rCount ; i++ )
    if ( pcb->rsrc[i].state != TERMINATED )
      pcb->rsrc[i].block( pcb->rsrc[i].data );
  pcb->currTh = UNASIGNED;
}

void printPsList(LiquidCrystal *lcd) {
  for ( int i=0 ; i < psTable.psCount ; i++ ) {
    lcd->print("ps:" + String(i+1) + " | ");
    Pcb *pcb = &psTable.ps[i];
    lcd->print(String(pcb->rCount) + " | ");
    lcd->print(String(pcb->thCount) + " | ");
  }
}

void kernelLoop() {
  int sensorValue = analogRead(A0);
  for ( int i=0 ; i < psTable.psCount ; i++ ) {
    k =' ';
    if ( (sensorValue > 700 && i ==0)
        || (sensorValue < 700 && i == 1)  ) // Habria que buscar un modo mas acorde
      k = keypad.getKey();
    psTable.currPs = i;
    runProcess();
  }
}

/*void randomPeriods(int* period, int size) {
  for ( int i=0 ; i<size ; i++ ){
    int exist = i + 1;
    while (exist) {
      period[i] = rand() % MAX_PERIOD + 2; // periodos no validos: {0;1}
      while( --exist && period[exist-1] != period[i] );
    }
  }
}*/


/*
 * // Implementación simple con vectores
Function function[ N ];
int f = 0;

void loop() {
      while ( 1 ) {
              scheduler();
      }
}

// Se atienden las Listas y la última bloqueada. Las demás esperan 
void scheduler(Function ready) {
       for(int i = f ; i<SIZE(ready) ; i++) {
              f = i;
              playTimer();
              ready[ i ].refresh();
        }
}

void rtcListener() {
    if ( functionOverTime() ) {
        f++;
        scheduler();
    }
}

//TODO
// Algunas son reiniciables
// Marcarlas como tal y swapear dentro del vector de funciones
// Usar copias de machine y de screen. Pisar el original despues de la funcion
 * 
 */
