#ifndef MECHASZOT_H
#define MECHASZOT_H

#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "Config.h"
#include "Relay.h"
#include "Pumps.h"
#include "Mixer.h"
#include "Holder.h"
#include "InOut.h"
struct Recipe {
    String name;      // Nazwa drinka wyświetlana na LCD
    int amounts[4];   // Ilość ml dla każdej z 4 pomp
    int mixTimeMs;    // Czas mieszania w ms
};

enum SystemState { STATE_MENU, STATE_CALIBRATION, STATE_POURING, STATE_MIXING, STATE_DISPENSING, STATE_ROTATING, STATE_DONE };

class MechaSzot {
public:
    MechaSzot();
    void begin();
    void update();
    
    // Metody obsługi przycisków
    void handleBtnNext();
    void handleBtnPrev();
    void handleBtnOk();
    void handleBtnBack();

private:
    PCF8574* pcfRelays;
    PCF8574* pcfButtons;
    Relay* relayBoard;
    ButtonManager* btnManager;
    LiquidCrystal_I2C lcd;
    Holder* holder;
    Mixer* mixer;
    Pump* pumps[4];
    Pump* pumpOut;

    Recipe recipes[3];
    int currentRecipe = 0;
    SystemState state = STATE_MENU;
    int cupsFilled = 0;

    void drawMenu();
    void startRecipe();
    void stopAll();
};

#endif