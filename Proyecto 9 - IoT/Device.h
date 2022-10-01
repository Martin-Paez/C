#ifndef _DEVICE_H_
#define _DEVICE_H_

   
  // Ejemplo de uso :
  //
  //    .h:
  //      struct Device botonera = { ...
  //      #define BOTON_ROJO    CREATE_MACRO(botonera, 0, val)  // vars[0] de boton
  //      #define BOTON_VERDE   CREATE_MACRO(botonera, 1, val)  
  //
  //    loop:
  //      BOTON_VERDE(FALSE);                 // setea en FALSE el boton verde
  //      ubiPublish( BOTON_ROJO(TRUE) );     // setea el boton rojo y publica todas las variables que tenga el boton
  //
  //    loop2:
  //      ubiAdd( BOTON_VERDE(FALSE) );       // se setea y agrega al JSON, pero todavia no se publica
  //      ubiCommit( &botonera );             // publica solo el boton verde
  //
  #define CREATE_MACRO(device, var, val)    ([&device](int i) -> struct Device* { \
                                                VAL(&device,var) = i; \
                                                return &device;\
                                              })(val)
                                              
  struct Var
  {
    int   timer;
    int   frequency;
    char* varName;
    int   value;
  };
  
  struct Device 
  {
    char* device;
    int   vSize;
    Var   vars[];
  };
  
  #define DEVICE(d)               (d)->device
  #define TOPIC(d,v)              DEVICE(d), NAME(d,v)
  
  #define VAL(d,v)                (d)->vars[v].value
  #define NAME(d,v)               (d)->vars[v].varName
  #define TOPIC_VAL(d,v)          NAME(d,v), VAL(d,v)
  
  #define TIME(d,v)               (d)->vars[v].timer
  #define ELAPSED(d,v)            ( millis() - TIME(d,v) )
  #define FREQUENCY(d,v)          (d)->vars[v].frequency
  #define NOT_PUBLISH_TIME(d,v)   ( FREQUENCY(d,v) == STOP || ELAPSED(d,v) < FREQUENCY(d,v) )
                                          
#endif
