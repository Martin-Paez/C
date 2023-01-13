#ifndef KERNEL_H
#define KERNEL_H
#include <LiquidCrystal.h>
#include <Keypad.h> 
#include "Macros.h"
  
/***************************************************************************/
/* ------------------------------------------------------------------------*/
/* ------------------------------ ZKernel.h -------------------------------*/
/* ------------------------------------------------------------------------*/
/***************************************************************************/

/* ------ Por el momento el kernel trabaja con un keyPad si o si ----------*/

  const byte ROWS = 4;
  const byte COLS = 4;
  
  char keys[ROWS][COLS] = {
   {'7','8','9','/'},
   {'4','5','6','*'},
   {'1','2','3','-'},
   {'C', '0' ,'=','+'}
  };

  byte rowPins[ROWS] = {PIN(12),PIN(11),PIN(8),PIN(7)};
  byte colPins[COLS] = {PIN(6),PIN(5),PIN(4),PIN(2)}; 
  
  Keypad keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
  

/* ------------------------------ Kernel ----------------------------------*/

  #define RESOURCES_PER_PS  4
  #define THREADS_PER_PS    4
  #define MAX_PS            4

  enum State{TERMINATED, BLOCKED, READY};
    
  void kernelLoop();
  void printPsList(LiquidCrystal *lcd);
  void _consumeK();
  
  
#endif
