#include <Arduino.h>
#include <AccelStepper.h>
#include "Holder.h"


class Holder {
private:
    AccelStepper* stepper;
    int currentPos = 0;
    int krokowyPin = 5;
    void stepOnce() {
        digitalWrite(krokowyPin, HIGH);
        delayMicroseconds(1000); // Czas trwania impulsami
        digitalWrite(krokowyPin, LOW);
    }
    void moveToPosition(int TargetPosition) {
        int diff=TargetPosition-stepper->currentPosition();
        if (diff>0) {
            for (int i=0;i<diff;i++) {
                stepOnce();
                currentPos++;
            }
        } else if (diff<0) {
            for (int i=0;i<-diff;i++) {
                stepOnce();
                currentPos--;
            }
        }
    }
public:
    int KrokowyStepPin; //krokowyStepPin, krokowyDirPin – numery pinów Arduino podłączonych do drivera silnika (STEP i DIR).
    int KrokowyDirPin;
    int id=0; // identyfikator danego uchwytu
    int currrent_angle=0;
    const int steps_per_revolution = 200; // Liczba kroków na pełny obrót silnika
    const int angle_per_step = 360 / steps_per_revolution; // Kąt na jeden krok
    const int steps_for_72_degrees = steps_per_revolution / 5; // Kroków na 72 stopni
    Holder() {
        stepper = new AccelStepper(AccelStepper::FULL4WIRE, 5, 4, 14, 12);
    }

    ~Holder() {
        delete stepper;
    }
    void nextPosition() {
    int targetPos = (currrent_angle + 1) % 5;
    moveToPosition(targetPos);
    currrent_angle = targetPos;
    }
    void setOnCup(Cup cup) {
    int targetPos = (cup.id % 5);
    moveToPosition(targetPos);
    currrent_angle = targetPos;
    }
    void stepOnce(bool dir) {
    digitalWrite(KrokowyDirPin, dir ? HIGH : LOW);
    digitalWrite(KrokowyStepPin, HIGH);
    delayMicroseconds(500);
    digitalWrite(KrokowyStepPin, LOW);
    delayMicroseconds(500);
}

    void begin() {
        stepper->setMaxSpeed(300.0);
        stepper->setAcceleration(100.0);
        stepper->moveTo(1000000);
    }

        void update() {
            if (stepper->distanceToGo() == 0)
                stepper->moveTo(-stepper->currentPosition());
            stepper->run();
        }
    };
    