#ifndef INOUT_H
#define INOUT_H

#include <Arduino.h>
#include <PCF8574.h>
#include <LiquidCrystal_I2C.h>
class Screen
{
    public:
        Screen(uint8_t adres);
        void wyswietl(String a);
        void init();

    private:
        uint8_t adres = 0x20;
        LiquidCrystal_I2C *lcd;
};
class Button {
public:
    typedef void (*button_callback_t)();

    Button() : _callback(nullptr) {} // Pusty konstruktor dla tablicy
    
    void setCallback(button_callback_t cb) { _callback = cb; }
    
    void press() {
        if (_callback) _callback();
    }

private:
    button_callback_t _callback;
};
class ButtonManager {
public:
    // Przekazujemy wskaźnik do istniejącego obiektu PCF
    ButtonManager(PCF8574* pcf, uint8_t interruptPin);
    uint8_t packDigitalInput(PCF8574::DigitalInput di);
    void begin();
    void loop();
    void attach(uint8_t pin, Button::button_callback_t cb);

private:
    PCF8574* _pcf;          // Tutaj zapisujemy wskaźnik do "sprzętu"
    uint8_t _intPin;
    uint8_t _lastStates = 0xFF;
    Button _buttons[8]; 

    static volatile bool _eventOccurred;
    static void IRAM_ATTR isr();
};
#endif 