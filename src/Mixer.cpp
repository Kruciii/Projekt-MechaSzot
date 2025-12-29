#include "Mixer.h"

Mixer::Mixer(Relay *relay, uint8_t pin) {
    _relayController = relay;
    _pin = pin;
    _isTimed = false;
    _isRunning = false;
}

void Mixer::on() {
    _isTimed = false; // Tryb ciągły (manualny)
    _isRunning = true;
    _relayController->on(_pin);
}

void Mixer::off() {
    _isTimed = false;
    _isRunning = false;
    _relayController->off(_pin);
}

void Mixer::onTime(unsigned long ms) {
    _duration = ms;
    _startTime = millis();
    _isTimed = true;  // Włączamy tryb czasowy
    _isRunning = true;
    _relayController->on(_pin);
}

void Mixer::update() {
    // Jeśli nie działa lub nie jest w trybie czasowym -> nic nie rób
    if (!_isRunning || !_isTimed) return;

    // Sprawdź czy czas minął (bezpieczne odejmowanie unsigned long)
    if (millis() - _startTime >= _duration) {
        off(); // Wyłącz mieszadło
        Serial.println("Mixer: Czas minął. Auto-Stop.");
    }
}

bool Mixer::isRunning() {
    return _isRunning;
}