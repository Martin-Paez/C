#ifndef _UBI_NEYEN_MARTIN_H_
#define _UBI_NEYEN_MARTIN_H_

  #include "Device.h"
  #include "Macros.h"

  #define STOP        -1
  #define LDR_FREQ    5000
  #define BOTON_FREQ  STOP
  
  struct Device boton = {"boton", 1, {{0, BOTON_FREQ, "presionado", FALSE}}};
  struct Device ldr = {"ldr", 1, {{0, LDR_FREQ, "intensidad", 0}}};
                                          
  #define LDR(val)    CREATE_MACRO(ldr, 0, val)
  #define BOTON(val)  CREATE_MACRO(boton, 0, val)
                                          
#endif
