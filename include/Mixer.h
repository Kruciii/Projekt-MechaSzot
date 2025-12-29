#ifndef MIXER_H
#define MIXER_H

#include <Arduino.h>
#include <Relay.h>
#include "PCF8574.h" 
class Mixer {
private:
    Relay *_relayController; // Wskaźnik do głównego kontrolera przekaźników
    uint8_t _pin;            // Numer pinu (0-7) pod którym jest mieszadło
    
    unsigned long _startTime; // Czas startu
    unsigned long _duration;  // Czas trwania
    bool _isTimed;           // Czy tryb jest czasowy?
    bool _isRunning;         // Czy mieszadło fizycznie działa?

public:
    // Konstruktor przyjmuje wskaźnik do obiektu Relay i numer pinu
    Mixer(Relay *relay, uint8_t pin);

    void on();                  // Włącz na stałe
    void off();                 // Wyłącz natychmiast
    void onTime(unsigned long ms); // Włącz na określony czas (nieblokujące)
    void update();              // Wrzuć to do void loop()
    
    bool isRunning();           // Pomocnicza: sprawdza czy działa
};
#endif