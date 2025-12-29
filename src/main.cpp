#include <Arduino.h>
#include "MechaSzot.h"

MechaSzot robot;

void setup() {
    Serial.begin(115200);
    robot.begin();
}

void loop() {
    robot.update();
}