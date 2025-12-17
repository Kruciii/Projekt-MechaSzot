#include <Arduino.h>
#include <AccelStepper.h>
const int sway = 512;
// Definicja pinów (zakładam ESP8266/ESP32 lub Arduino z takimi pinami)
AccelStepper stepper2(AccelStepper::FULL4WIRE, 5, 14, 4, 12);

void setup()
{
  stepper2.setMaxSpeed(300.0);
  stepper2.setAcceleration(100.0);

  // Pierwszy ruch
  stepper2.moveTo(sway);
}

void loop()
{
  // Sprawdzamy, czy silnik dojechał do celu
  if (stepper2.distanceToGo() == 0)
  {
    // Jeśli jest na pozycji 200, jedź do -200
    if (stepper2.currentPosition() == sway)
    {
      stepper2.moveTo(-sway);
      Serial.println("Zmiana kierunku: cel -200");
    }
    // Jeśli jest na pozycji -200 (lub innej, np. 0 na starcie), jedź do 200
    else
    {
      stepper2.moveTo(sway);
      Serial.println("Zmiana kierunku: cel 200");
    }
  }

  // Wywołanie run() - najważniejsza część!
  // Bez Serial.print w każdym obiegu, silnik będzie działał płynnie
  stepper2.run();
}