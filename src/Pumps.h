#ifndef PUMPS_H
#define PUMPS_H

#include <Arduino.h>

class Pump {
public:
  Pump();
  ~Pump();
  void pumpTime(int time);
  void pumpMl(int ml);
  void backFlush();//Pompuj w odwrotną stronę tak żeby pozbyć się płynu w rurce;
  
};

#endif 
