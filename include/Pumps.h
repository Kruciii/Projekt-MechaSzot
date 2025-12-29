#ifndef PUMPS_H
#define PUMPS_H

#include <Arduino.h>
#include <PCF8574.h>
#include <Relay.h>


class Pump {
public:
   Pump(int index,int pin, Relay* relay);
  ~Pump();
  void pumpByTime(int time);
  void pumpByVolume(int ml);
  void backFlush();//Pompuj w odwrotną stronę tak żeby pozbyć się płynu w rurce;
  int getIndex();
  bool update();
  bool isOn();
  void setFlowRate(float msPerMl) { this->mlMsConversion = msPerMl; }
private:
  Relay* relay;
  int index;
  int pin;
  bool _isOn=false;
  unsigned long startTime = 0;
  unsigned long duration = 0;
  float mlMsConversion = 1;
};

#endif 
