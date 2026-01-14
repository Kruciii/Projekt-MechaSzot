
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

// Perform a single step and yield to keep the watchdog happy
void Holder::stepOnce(bool dir) {
    digitalWrite(_dirPin, dir ? HIGH : LOW);
    digitalWrite(_stepPin, HIGH);
    delayMicroseconds(500);
    digitalWrite(_stepPin, LOW);
    delayMicroseconds(500);
    // Let OS do background tasks and feed the watchdog
    yield();
}

// Blocking (compatibility) implementation reused via non-blocking API
void Holder::moveToPosition(int targetIndex) {
    startMoveToPosition(targetIndex);
    // Busy-wait but call update() each iteration so we don't trigger WDT
    while (isMoving()) {
        update();
    }
}

// Start an asynchronous move to a target index
void Holder::startMoveToPosition(int targetIndex) {
    if (targetIndex < 0 || targetIndex > 4) return;
    if (targetIndex == _currentPositionIndex) return;

    int diff = targetIndex - _currentPositionIndex;
    _direction = (diff > 0);
    _stepsRemaining = (long)abs(diff) * _stepsFor72Degrees;
    _targetIndex = targetIndex;
    _isMoving = true;
} 

// Call frequently from the main loop to perform the asynchronous move
void Holder::update() {
    if (!_isMoving) return;

    if (_stepsRemaining > 0) {
        stepOnce(_direction);
        _stepsRemaining--;
    }

    if (_stepsRemaining <= 0) {
        _isMoving = false;
        _currentPositionIndex = _targetIndex;
    }
}

void Holder::nextPosition() {
    int nextIndex = (_currentPositionIndex + 1) % 5;
    moveToPosition(nextIndex);
}

void Holder::setHome() {
    _currentPositionIndex = 0;
}