#ifndef _UBI_NEYEN_MARTIN_H_
#define _UBI_NEYEN_MARTIN_H_

  #include "Device.h"
  #include  "Macros.h"

/* ---------------------------------  BOTON ------------------------------- */

  #define BOTON_FREQ  5000
  #define PRESIONADO  0
  #define BOTON(val)  CREATE_MACRO(boton, PRESIONADO , val)
  
  struct Device boton = {"boton", 1, { 0, BOTON_FREQ, true , "presionado", true ,FALSE }};

/* ----------------------------------  LDR -------------------------------- */

  #define LDR_FREQ    5000
  #define INTENSIDAD  0
  
  #ifdef COMPILE_ESP32
    #define RESOLUTION  12 
    #define LDR_PIN     33
  #else
    #define RESOLUTION  10 // Compatible con Arduino Uno
    #define LDR_PIN     A0
  #endif
  #define READ_LDR analogRead(LDR_PIN) * (100.0 / pow(2,RESOLUTION))

  struct Device ldr = {"ldr", 1, { 0, LDR_FREQ, false, "intensidad", false, {.f=[]()->int{return READ_LDR;}} }};

#endif
