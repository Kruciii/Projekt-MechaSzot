
// Klasa Holder
// Steruje silnikiem krokowym dla podstawki z 5 pozycjami (co 72°)
#include <Holder.h>
#include <Arduino.h>

Holder::Holder(int stepPin, int dirPin, MicrostepMode mode, int nativeSteps)
    : _stepPin(stepPin), _dirPin(dirPin), _currentPositionIndex(0) {
    
    // Obliczamy całkowitą liczbę kroków: kroki silnika * mnożnik mikrokroków
    _totalStepsPerRev = (long)nativeSteps * (int)mode;
    
    // Obliczamy dystans dla 72 stopni (1/5 obrotu)
    _stepsFor72Degrees = _totalStepsPerRev / 5;
}

void Holder::begin() {
    pinMode(_stepPin, OUTPUT);
    pinMode(_dirPin, OUTPUT);
    digitalWrite(_stepPin, LOW);
    digitalWrite(_dirPin, LOW);
}

void Holder::stepOnce(bool dir) {
    digitalWrite(_dirPin, dir ? HIGH : LOW);
    digitalWrite(_stepPin, HIGH);
    // Przy wysokich mikrokrokach (np. 64) można skrócić te czasy
    delayMicroseconds(500); 
    digitalWrite(_stepPin, LOW);
    delayMicroseconds(500); 
}

void Holder::moveToPosition(int targetIndex) {
    if (targetIndex < 0 || targetIndex > 4) return;

    int diff = targetIndex - _currentPositionIndex;
    if (diff == 0) return;

    bool direction = (diff > 0);
    long stepsToDo = abs(diff) * _stepsFor72Degrees;

    for (long i = 0; i < stepsToDo; i++) {
        stepOnce(direction);
    }

    _currentPositionIndex = targetIndex;
}

void Holder::nextPosition() {
    int nextIndex = (_currentPositionIndex + 1) % 5;
    moveToPosition(nextIndex);
}

void Holder::setHome() {
    _currentPositionIndex = 0;
}