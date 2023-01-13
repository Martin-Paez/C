#ifndef MACHINE_H
#define MACHINE_H

#include "ZLib.h"
#include "Macros.h"

    enum MachineType {LCD=0};

    enum Function {POLYNOMIAL, EXPONENTIAL, ONE, F_SIZE};
    String FunctionNames[] = {"POL", "EXP", "ONE"};
    
/* ------------------------------------------------------------------------*/
/* ------------------------- Valores POR DEFECTO --------------------------*/
/* ------------------------------------------------------------------------*/

  // Con alocacion dinamica tendria mas sentido hacerlo variable
  #define WHEELS_PER_MACHINE  3
  
  #define DEFAULT_PLAY_DURATION       10000
  #define DEFAULT_VALUES_PER_WHEEL    7  
  #define DEFAULT_SPIN_DELAY          1000
  #define DEFAULT_MIN_SPIN_DURATION   1000
  #define DEFAULT_MAX_PERIOD          100
  #define DEFAULT_RAND_SPEED_FACTOR   DEFAULT_MIN_SPIN_DURATION / 50
  
  
  #define DEFAULT_FUNCTION            EXPONENTIAL
  #define DEFAULT_LAST_FUNCTION       POLYNOMIAL
  // f(x) = A.x^2 + B.x + C
  #define Ap             -0.02
  #define Bp             2
  #define Cp             1
  // f(x) = A.e^(B.x + C)+ D;
  #define Ae             1.1     // -1
  #define Be             0.0009  //-0.005
  #define Ce             1.4     //22
  #define De             1       //100
      

/* Se sacrifica encapsulamiento para permitir la creacion de vectores.
 * Cuando se implemente alocacion dinamica de memoria deberian de 
 * quitarse del ".h" estas estructuras
 */
/*TODO 
  * Machine.ino
  *    #define _MACHINE_SIZE sizeof(_Machine)               // sizeof = n bytes
  *    struct _Machine {
  *       .... 
  *    }
  * 
  * Machine.h 
  *    #define MACHINE_SIZE  _MACHINE_SIZE
  *    typedef Machine struct _Machine
  * 
  * User.ino
  *    struct MachineLcdRsrc {
  *      char  m[MACHINE_SIZE];                             // char = byte
  *    }
  *    struct MachineLcdRsrc machines[LCD_MACHINES_COUNT];
  *    Machine *lcdM;
  * 
  *    void getReadyMachineLCD(void *m) {
  *      lcdMR = ((struct MachineLcdRsrc *) m);
  *      lcdMR->state = READY;
  *      lcdM = &(Machine)lcdMR->m;                         // Machine
  *    }
  */
    struct Wheel{
      int value;
      Lapse t;
      Function f = DEFAULT_FUNCTION;
      int A = Ae;
      int B = Be;
      int C = Ce;
      int D = De;
      int valuesPerWheel = DEFAULT_VALUES_PER_WHEEL;
      int maxPeriod = DEFAULT_MAX_PERIOD;
      int randSpeed;
    };

  // Para cuado se implemente alocacion dinamica
  typedef struct _Machine Machine;
  
  struct _Machine {
    struct Wheel wheels[WHEELS_PER_MACHINE];
    int wCount = WHEELS_PER_MACHINE; //Por futura alocacion dinamica
    struct Timer t;
    int randSpeedFactor = DEFAULT_RAND_SPEED_FACTOR;
    MachineType type; 
  };
  

/* ------------------------------------------------------------------------*/
/* ----------------------- NECESITAN ser implementadas---------------------*/
/* ------------------------------------------------------------------------*/
   
  void showWheel(Machine *m, int w);
  

/* ------------------------------------------------------------------------*/
/* --------------------------- Funciones Basicas --------------------------*/
/* ------------------------------------------------------------------------*/
  
  void turnOnMachine(Machine *m);
  void spin (Machine *m);
  int continueSpinning(Machine *m);
  void setType(Machine *m, MachineType type);


/* ------------------------------------------------------------------------*/
/* ---------------------- Funciones de Configuracion ----------------------*/
/* ------------------------------------------------------------------------*/

  void setPlayDuration(Machine *m, int duration);
  // Deberian implementarse el resto cuando se utilice asignacion dinamica
  
#endif
