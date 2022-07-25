#ifndef LCD_MACHINE_H
#define LCD_MACHINE_H

#include <LiquidCrystal.h>
#include "Machine.h"
#include "ZKernel.h"
#include "Macros.h"
#include "ZLib.h"

/* ------------------------------------------------------------------------*/
/* ------------------------------- Funciones ------------------------------*/
/* ------------------------------------------------------------------------*/

  /* Esta preparado para instanciar tantas maquinas como lcd de provean,
   * cada una con sus particularidades y seteos de runtime.
   * 
   * Los valores por defecto y las configuraciones posibles estan detalladas
   * en Machine.h. 
   */
  int exeLcdMachine(LiquidCrystal *lcd);

/* ------------------------------------------------------------------------*/
/* ----------------------------- Configuracion ----------------------------*/
/* ------------------------------------------------------------------------*/

  #define LCD_MACHINES_COUNT 2
  
  #define TX        1
  #define RX        0
  #define D4        A1
  #define D5        A2
  #define D6        PIN(10)
  #define D7        PIN(9)


  /* Se opto por no usar alocacion dinamica de memoria. Empleando ese
   * metodo podría mejorarse este apartado.
   */
  /* Se provee un lcd. El usuario podrá optar por no usarlo, o incluso, 
   * crear varios, en caso de instanciar más de una maquina.
   */
  // lcd DE 16X4
  LiquidCrystal machineLcd(TX,RX,D4,D5,D6,D7);
  LiquidCrystal *powerOnErrorLcd = & machineLcd;

#endif
