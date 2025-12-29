#ifndef CONFIG_H
#define CONFIG_H

// --- ADRESY I2C ---
#define ADDR_RELAYS  0x20  // Adres modułu przekaźników
#define ADDR_BUTTONS 0x21  // Adres modułu przycisków (jeśli inny) lub ten sam
#define ADDR_LCD     0x27  // Adres ekranu LCD

// --- PINY NA EKSPANDERZE PRZEKAŹNIKÓW (Relay) ---
// Piny 0-7 na module PCF8574 sterującym przekaźnikami
#define PIN_PUMP_1   0  // Składnik 1
#define PIN_PUMP_2   1  // Składnik 2
#define PIN_PUMP_3   2  // Składnik 3
#define PIN_PUMP_4   3  // Składnik 4
#define PIN_PUMP_OUT 4  // Pompa nalewająca do kieliszków
#define PIN_MIXER    5  // Silnik mieszadła

// --- PINY NA EKSPANDERZE PRZYCISKÓW (ButtonManager) ---
// Piny 0-7 na module PCF8574 sterującym przyciskami
#define BTN_NEXT_PIN  0
#define BTN_PREV_PIN  1
#define BTN_OK_PIN    2
#define BTN_BACK_PIN  3
#define INT_PIN       14 // Pin przerwania na Arduino/ESP (np. D5)

// --- PINY SILNIKA KROKOWEGO (Holder) ---
// Podłączone bezpośrednio do mikrokontrolera
#define PIN_STEP     12 // Przykład D6
#define PIN_DIR      13 // Przykład D7
#define HOLDER_STEPS 200 // Kroki na pełny obrót silnika

// --- KALIBRACJA (Ile milisekund na 1 ml płynu) ---
// Zmierz czas potrzebny na nalanie 100ml i podziel przez 100.
#define CALIB_PUMP_1   80.0f 
#define CALIB_PUMP_2   80.0f
#define CALIB_PUMP_3   80.0f
#define CALIB_PUMP_4   80.0f
#define CALIB_PUMP_OUT 60.0f 

// --- USTAWIENIA OGÓLNE ---
#define SHOT_SIZE_ML   40   // Pojemność jednego kieliszka
#define CUPS_COUNT     5    // Ilość kieliszków w karuzeli

#endif