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
  //      BOTON_VERDE(FALSE);              // setea en FALSE el boton verde
  //      ubiPush( BOTON_ROJO(TRUE) );     // setea el boton rojo y publica todas las variables que tenga el boton
  //
  #define CREATE_MACRO(device, var, val)    ([&device](int i) -> struct Device* { \
                                                SET(&device,var,i); \
                                                return &device;\
                                              })(val)
  struct Var
  {
    int       timer;
    int       frequency;
    bool      paused;
    char*     varName;
    bool      eEsInt;
    union V
      {
        int   value;
        int   (*f)();
      }       e;
  };
  
  struct Device 
  {
    char* device;
    int   vSize;
    Var   vars[];
  };
  
  #define DEVICE(d)               (d)->device
  #define TOPIC(d,v)              DEVICE(d), VARNAME(d,v)

  #define V(d,v)                  (d)->vars[v].e
  #define SET(d,v,i)              ( V(d,v).value = i )
  #define GET(d,v)                ( ((d)->vars[v].eEsInt) ? V(d,v).value : (V(d,v).f)() )
  #define VARNAME(d,v)            (d)->vars[v].varName
  #define TOPIC_VAL(d,v)          VARNAME(d,v), GET(d,v)
  
  #define TIME(d,v)               (d)->vars[v].timer
  #define ELAPSED(d,v)            ( millis() - TIME(d,v) )
  #define FREQUENCY(d,v)          (d)->vars[v].frequency
  #define RESET(d,v)              ((d)->vars[v].timer = millis() )
  #define DISABLE_PUBLISH(d,v)    ((d)->vars[v].paused = true )
  #define ENABLE_PUBLISH(d,v)     RESET(d,v); (d)->vars[v].paused = false
  #define NOT_PUBLISH_TIME(d,v)   ((d)->vars[v].paused || ELAPSED(d,v) < FREQUENCY(d,v) )
                                          
#endif
