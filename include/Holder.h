// Minimal Holder.h stub to satisfy include
#ifndef HOLDER_H
#define HOLDER_H

#include <Arduino.h>
#include <Arduino.h>
// Struktura Cup
// Reprezentuje pojedynczy kubek z unikalnym id i pozycją na podstawce
struct Cup
{
  int id;       // unikalny numer kubka (0,1,2,...)
  int position; // pozycja na podstawce 0-4 (co 72°)
};
class Holder {
public:
    // Definicja trybów na podstawie przesłanej tabeli
    enum MicrostepMode {
        MS_8  = 8,   // MS2: GND,    MS1: GND
        MS_2  = 2,   // MS2: GND,    MS1: VCC_IO (półkrok)
        MS_64 = 64,  // MS2: VCC_IO, MS1: GND    (ćwierćkrok w tej tabeli oznacza 64)
        MS_16 = 16   // MS2: VCC_IO, MS1: VCC_IO
    };

    /**
     * @param stepPin Pin STEP
     * @param dirPin Pin DIR
     * @param mode Wybrany tryb mikrokroków (z tabeli)
     * @param nativeSteps Pełne kroki silnika (standardowo 200 dla NEMA 17)
     */
    Holder(int stepPin, int dirPin, MicrostepMode mode, int nativeSteps = 200);

    void begin();
    void nextPosition();
    void moveToPosition(int targetIndex);
    void setHome();

    int getCurrentPositionIndex() const { return _currentPositionIndex; }

private:
    int _stepPin;
    int _dirPin;
    long _totalStepsPerRev; // Całkowita liczba mikrokroków na obrót
    long _stepsFor72Degrees;
    int _currentPositionIndex;

    void stepOnce(bool dir);
};

#endif // HOLDER_H
