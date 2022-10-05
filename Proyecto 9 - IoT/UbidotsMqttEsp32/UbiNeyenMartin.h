#ifndef _UBI_NEYEN_MARTIN_H_
#define _UBI_NEYEN_MARTIN_H_

  #include "Device.h"
  #include  "Macros.h"

/* ---------------------------------  BOTON ------------------------------- */

  #define BOTON_FREQ  1000
  #define PRESIONADO  0

  char a[] = "presionado";
  struct Var presionado = 
  { 0 /*timer = 0 segundos*/,
    BOTON_FREQ, 
    true /*pausado, no publica*/, 
    a, 
    true ,/*el valor es un entero almacenado en la estructura*/
    FALSE /*el boton se encuentra sin precionar*/,
  };
  
  #define BOTON(val)  CREATE_MACRO(boton, PRESIONADO , val)

  
/* ----------------------------------  LDR -------------------------------- */

  #define LDR_FREQ    5000
  #define INTENSIDAD  0
  
  #ifdef COMPILE_ESP32
    #define RESOLUTION  12 
    #define LDR_PIN     13
  #else
    #define RESOLUTION  10 // Compatible con Arduino Uno
    #define LDR_PIN     A0
  #endif
  #define READ_LDR analogRead(LDR_PIN) * (100.0 / pow(2,RESOLUTION))

  char aa[]="intensidad";
  int ff() {return READ_LDR;}
  struct Var intensidad = 
  {
    0 /*timer = 0 segundos*/,
    LDR_FREQ, 
    false /*listo para publicar*/, 
    aa, 
    false, /*el valor se lee desde una funcion*/
    {.f=ff/*[]()->int{{return READ_LDR;}}*/} /*los valores se obtienen de un analogRead*/
  };
  struct Var ldrVars[] = {intensidad};
  char s[] = "ldr";
  struct Device ldr = {s, 1, ldrVars};

  struct Var botonVars[] = {presionado};
  char ss[] = "boton";
  struct Device boton = {ss, 1, botonVars };
#endif
