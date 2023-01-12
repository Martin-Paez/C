#line 1 "c:\\Users\\54911\\Documents\\GitHub\\C\\Proyecto 9 - IoT\\Esp\\RunT\\RunT.h"
#ifndef _RUN_T
#define _RUN_T

class RunT {

  private:

    int period;
    int last;
    void (*f)();
    bool running;
  
  public:
  
    RunT(int period, void func(), bool paused=true);
    void loop();
    void pause() ;
    void play();

};

#include "RunT.cpp"       // En Arduino es necesario aca

#endif