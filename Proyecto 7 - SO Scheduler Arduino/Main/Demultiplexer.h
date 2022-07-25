#ifndef DEMULTIPLEXOR_H
#define DEMULTIPLEXOR_H

#define DEMULTIPLEXOR_A   PIN(13)
#define DEMULTIPLEXOR_B   PIN(3)

  void initDemultiplexer(void);
  
  // selector = 2 => 10b => A=1,B=0
  int addDemultiplexer(int psId, int selector);
  
  void selectOutput(int code);

#endif
