#ifndef RELAY_H
#define RELAY_H

#include <Arduino.h>
#include <PCF8574.h>

class Relay {
public:
  Relay(uint8_t i2cAddress);
  bool begin();               
  void on(uint8_t pin);
  void off(uint8_t pin);
  void toggle(uint8_t pin);
  void scan();                
  void test();                
  
private:
  PCF8574* pcf;
  static constexpr uint8_t onState  = LOW;
  static constexpr uint8_t offState = HIGH;
};


#endif 