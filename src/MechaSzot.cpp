#include "MechaSzot.h"

// --- Funkcje statyczne do łączenia przycisków ---
MechaSzot* ptr = nullptr;
void s_next() { if(ptr) ptr->handleBtnNext(); }
void s_prev() { if(ptr) ptr->handleBtnPrev(); }
void s_ok()   { if(ptr) ptr->handleBtnOk(); }
void s_back() { if(ptr) ptr->handleBtnBack(); }

MechaSzot::MechaSzot() : lcd(ADDR_LCD, 16, 2) {
    ptr = this;
    pcfRelays = new PCF8574(ADDR_RELAYS);
    pcfButtons = new PCF8574(ADDR_BUTTONS);
    relayBoard = new Relay(ADDR_RELAYS, true);
    
    pumps[0] = new Pump(0, PIN_PUMP_1, relayBoard);
    pumps[1] = new Pump(1, PIN_PUMP_2, relayBoard);
    pumps[2] = new Pump(2, PIN_PUMP_3, relayBoard);
    pumps[3] = new Pump(3, PIN_PUMP_4, relayBoard);
    pumpOut  = new Pump(4, PIN_PUMP_OUT, relayBoard);
    
    pumps[0]->setFlowRate(CALIB_PUMP_1);
    pumps[1]->setFlowRate(CALIB_PUMP_2);
    pumps[2]->setFlowRate(CALIB_PUMP_3);
    pumps[3]->setFlowRate(CALIB_PUMP_4);
    pumpOut->setFlowRate(CALIB_PUMP_OUT);

    mixer = new Mixer(relayBoard, PIN_MIXER);
    holder = new Holder(PIN_STEP, PIN_DIR, Holder::MS_16, HOLDER_STEPS);
    btnManager = new ButtonManager(pcfButtons, INT_PIN);

    // Definicja przepisów
    recipes[0] = {"1. Wsciekly Pies", {15, 0, 25, 0}, 2000};
    recipes[1] = {"2. Blue Lagoon",   {10, 10, 10, 10}, 3000};
    recipes[2] = {"3. Czyszczenie",   {50, 50, 50, 50}, 5000};
}

void MechaSzot::begin() {
    Wire.begin();
    lcd.init(); lcd.backlight();
    relayBoard->begin();
    holder->begin();
    
    btnManager->attach(BTN_NEXT_PIN, s_next);
    btnManager->attach(BTN_PREV_PIN, s_prev);
    btnManager->attach(BTN_OK_PIN, s_ok);
    btnManager->attach(BTN_BACK_PIN, s_back);
    btnManager->begin();

    lcd.print("MechaSzot v1.0");
    delay(2000);
    drawMenu();
}

void MechaSzot::update() {
    btnManager->loop();
    mixer->update();
    for(auto p : pumps) p->update();
    pumpOut->update();

    switch(state) {
        case STATE_POURING: {
            bool running = false;
            for(auto p : pumps) if(p->isOn()) running = true;
            if(!running) {
                lcd.setCursor(0,1); lcd.print("Mieszanie...    ");
                mixer->onTime(recipes[currentRecipe].mixTimeMs);
                state = STATE_MIXING;
            }
            break;
        }
        case STATE_MIXING:
            if(!mixer->isRunning()) {
                cupsFilled = 0;
                lcd.clear(); lcd.print("Nalewanie...");
                pumpOut->pumpByVolume(SHOT_SIZE_ML);
                state = STATE_DISPENSING;
            }
            break;
        case STATE_DISPENSING:
            if(!pumpOut->isOn()) {
                cupsFilled++;
                if(cupsFilled >= CUPS_COUNT) state = STATE_DONE;
                else {
                    lcd.setCursor(0,1); lcd.print("Kieliszek: " + String(cupsFilled+1));
                    holder->nextPosition(); // Blokujące
                    state = STATE_ROTATING;
                }
            }
            break;
        case STATE_ROTATING:
            pumpOut->pumpByVolume(SHOT_SIZE_ML);
            state = STATE_DISPENSING;
            break;
        case STATE_DONE:
            lcd.clear(); lcd.print("Gotowe!");
            delay(2000);
            state = STATE_MENU;
            drawMenu();
            break;
    }
}

void MechaSzot::startRecipe() {
    lcd.clear(); lcd.print("Przygotowanie...");
    Recipe r = recipes[currentRecipe];
    for(int i=0; i<4; i++) if(r.amounts[i]>0) pumps[i]->pumpByVolume(r.amounts[i]);
    mixer->on(); 
    state = STATE_POURING;
}

void MechaSzot::drawMenu() {
    lcd.clear(); lcd.print("Wybierz:");
    lcd.setCursor(0,1); lcd.print(recipes[currentRecipe].name);
}
void MechaSzot::stopAll() {
    relayBoard->allOff(); mixer->off();
}

void MechaSzot::handleBtnNext() {
    if(state == STATE_MENU) {
        currentRecipe = (currentRecipe + 1) % 3;
        drawMenu();
    } else if(state == STATE_CALIBRATION) holder->nextPosition();
}
void MechaSzot::handleBtnPrev() {
    if(state == STATE_MENU) {
        currentRecipe = (currentRecipe - 1 + 3) % 3;
        drawMenu();
    }
}
void MechaSzot::handleBtnOk() {
    if(state == STATE_MENU) startRecipe();
    else if(state == STATE_CALIBRATION) {
        holder->setHome();
        state = STATE_MENU;
        drawMenu();
    }
}
void MechaSzot::handleBtnBack() {
    if(state == STATE_MENU) {
        state = STATE_CALIBRATION;
        lcd.clear(); lcd.print("Kalibracja Poz.");
    } else {
        stopAll();
        state = STATE_MENU;
        drawMenu();
    }
}