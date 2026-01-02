#ifndef RELAY_H
#define RELAY_H

#include <Arduino.h>
#include <PCF8574.h>
#include <Wire.h>

class Relay {
private:
    PCF8574 *pcf;
    uint8_t _addr;
    bool _activeLow; // Flaga czy przekaźnik sterowany masą
    uint8_t ON;      // Wartość logiczna dla włączenia
    uint8_t OFF;     // Wartość logiczna dla wyłączenia

public:
    // Domyślnie Active Low (większość modułów chińskich)
    Relay(uint8_t addr, bool activeLow = true); 
    Relay(uint8_t addr,PCF8574* expander, bool activeLow = true); 
    bool begin();
    void on(uint8_t pin);
    void off(uint8_t pin);
    void toggle(uint8_t pin);
    void allOff(); // Przydatna funkcja wyłączająca wszystko
    static void scan();
    void test();
};

#endif