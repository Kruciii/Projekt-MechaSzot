#include <Arduino.h>
#include <AccelStepper.h>
// put function declarations here:

AccelStepper stepper2(AccelStepper::FULL4WIRE, 5, 4, 14, 12);
void setup()
{
  stepper2.setMaxSpeed(300.0);
    stepper2.setAcceleration(100.0);
    stepper2.moveTo(1000000);

}

void loop()
{
  if (stepper2.distanceToGo() == 0) stepper2.moveTo(-stepper2.currentPosition());
    stepper2.run();

}
// p