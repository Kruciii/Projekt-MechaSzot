#include <Arduino.h>
#include <PCF8574.h>
#include <LiquidCrystal_I2C.h>
#include <InOut.h>
volatile bool ButtonManager::_eventOccurred = false;

// Konstruktor zapisuje wskaźnik do PCF w prywatnym polu
ButtonManager::ButtonManager(PCF8574* pcf, uint8_t interruptPin) {
    this->_pcf = pcf;
    this->_intPin = interruptPin;
}

void IRAM_ATTR ButtonManager::isr() {
    _eventOccurred = true;
}

void ButtonManager::begin() {
    pinMode(_intPin, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(_intPin), ButtonManager::isr, FALLING);
    
    if (_pcf != nullptr) {
        for(int i = 0; i < 8; i++) {
            _pcf->pinMode(i, INPUT_PULLUP);
        }
        _pcf->begin(); // Biblioteka Renzo wymaga begin()

        // Odczytujemy stan początkowy
        PCF8574::DigitalInput di = _pcf->digitalReadAll();
        _lastStates = packDigitalInput(di); 
    }
}
void ButtonManager::loop() {
    if (_eventOccurred && _pcf != nullptr) {
        _eventOccurred = false;
        
        // digitalReadAll() to odpowiednik "read8" w tej bibliotece
        PCF8574::DigitalInput currentStatesStruct = _pcf->digitalReadAll();
        uint8_t currentStates = packDigitalInput(currentStatesStruct);
        
        for (int i = 0; i < 8; i++) {
            bool lastBit = bitRead(_lastStates, i);
            bool currentBit = bitRead(currentStates, i);
            
            if (lastBit == HIGH && currentBit == LOW) {
                _buttons[i].press(); 
            }
        }
        _lastStates = currentStates;
    }
}

// Pomocnicza funkcja (dodaj ją do klasy jako private lub w pliku .cpp)
uint8_t ButtonManager::packDigitalInput(PCF8574::DigitalInput di) {
    uint8_t res = 0;
    if (di.p0 == HIGH) res |= (1 << 0);
    if (di.p1 == HIGH) res |= (1 << 1);
    if (di.p2 == HIGH) res |= (1 << 2);
    if (di.p3 == HIGH) res |= (1 << 3);
    if (di.p4 == HIGH) res |= (1 << 4);
    if (di.p5 == HIGH) res |= (1 << 5);
    if (di.p6 == HIGH) res |= (1 << 6);
    if (di.p7 == HIGH) res |= (1 << 7);
    return res;
}
void ButtonManager::attach(uint8_t pin, Button::button_callback_t cb) {
    if (pin < 8) {
        _buttons[pin].setCallback(cb);
    }
}