#include <Arduino.h>
#include <AccelStepper.h>
#include "Holder.h"

class Holder {
private:
    AccelStepper* stepper; 
public:
    Holder() {
        stepper = new AccelStepper(AccelStepper::FULL4WIRE, 5, 4, 14, 12);
    }

    ~Holder() {
        delete stepper;
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