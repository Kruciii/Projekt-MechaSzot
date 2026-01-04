🤖 MechaSzot – Zautomatyzowany Barman
MechaSzot to projekt zautomatyzowanego robota barmańskiego opartego na platformie Arduino. System pobiera składniki z butelek, miesza drinki i rozlewa je do 5 kieliszków umieszczonych na obrotowej karuzeli.

Projekt został stworzony z myślą o edukacji w zakresie systemów wbudowanych, wykorzystując sterowanie magistralą I2C, maszynę stanów (FSM) oraz obsługę przerwań.

📋 Spis Treści
Funkcjonalności

Architektura Sprzętowa

Architektura Oprogramowania

Maszyna Stanów (Logika)

Konfiguracja i Kalibracja

Bezpieczeństwo

🌟 Funkcjonalności
Dozowanie: Obsługa 4 pomp perystaltycznych do składników bazowych.

Mieszanie: Centralny zbiornik z mieszadłem i pompą dozującą (output).

Dystrybucja: Karuzela napędzana silnikiem krokowym obsługująca 5 kieliszków (Shotów).

Interfejs: Menu na wyświetlaczu LCD (I2C) sterowane przyciskami fizycznymi.

Wielozadaniowość: Nieblokujące sterowanie pompami (bez delay).

🛠 Architektura Sprzętowa
System został zaprojektowany tak, aby zminimalizować zużycie pinów mikrokontrolera poprzez wykorzystanie ekspanderów portów I2C.

MCU: Arduino (Uno/Nano/Mega).

Ekspander 1 (PCF8574): Sterowanie przekaźnikami (Pompy 1-4, Mieszadło, Pompa Główna).

Ekspander 2 (PCF8574): Obsługa wejść przycisków (Menu, Start, Stop).

Silnik Krokowy: Obrót tacy z kieliszkami (sterownik np. A4988/DRV8825).

Wyświetlacz: LCD 16x2 / 20x4 po I2C.

Fragment kodu

graph TD
    MCU[Arduino] -->|I2C SDA/SCL| LCD[Wyświetlacz LCD]
    MCU -->|I2C SDA/SCL| PCF1[PCF8574 - Przekaźniki]
    MCU -->|I2C SDA/SCL| PCF2[PCF8574 - Przyciski]
    MCU -->|Digital Pins| STEP[Sterownik Silnika]
    MCU -->|Interrupt Pin| PCF2

    PCF1 -->|Relay| P1[Pompa 1]
    PCF1 -->|Relay| P2[Pompa 2]
    PCF1 -->|Relay| P3[Pompa 3]
    PCF1 -->|Relay| P4[Pompa 4]
    PCF1 -->|Relay| MIX[Mieszadło]
    PCF1 -->|Relay| POUT[Pompa Wylewająca]
    
    STEP --> MOTOR[Silnik Karuzeli]
💻 Architektura Oprogramowania
Kod jest zorganizowany obiektowo. Główne klasy systemu:

1. Relay
Klasa typu "wrapper" na pojedynczy pin ekspandera.

Obsługuje logikę Active Low (stan NISKI włącza urządzenie).

Zawiera metody bezpieczeństwa allOff().

2. Pump
Inteligentna obsługa dozowania płynów.

Zasada działania: Przelicza mililitry na czas pracy pompy.

Non-blocking: Wykorzystuje millis() w metodzie update(), co pozwala na równoległą obsługę przycisków podczas nalewania.

3. Holder
Sterownik mechaniki karuzeli.

Odpowiada za precyzyjne sterowanie silnikiem krokowym.

Przelicza kroki silnika na pozycje kieliszków (obrót o 72 stopnie).

4. ButtonManager
Zaawansowana obsługa wejścia.

Działa w oparciu o przerwania sprzętowe (Interrupts), a nie polling.

Oszczędza czas procesora – sprawdza stan przycisków przez I2C tylko wtedy, gdy zostanie zgłoszone przerwanie na pinie INT_PIN.

🧠 Maszyna Stanów (Logika)
Sercem systemu jest klasa MechaSzot realizująca wzorzec Finite State Machine. Robot zawsze znajduje się w jednym z poniższych stanów:

Fragment kodu

stateDiagram-v2
    [*] --> STATE_MENU
    STATE_MENU --> STATE_POURING : Start
    STATE_POURING --> STATE_MIXING : Składniki nalane
    STATE_MIXING --> STATE_DISPENSING : Wymieszane
    
    state Pętla_Nalewania {
        STATE_DISPENSING --> STATE_ROTATING : Kieliszek pełny
        STATE_ROTATING --> STATE_DISPENSING : Następna pozycja
    }
    
    STATE_DISPENSING --> STATE_DONE : Wszystkie kieliszki (5/5)
    STATE_DONE --> STATE_MENU : Reset
MENU: Wybór drinka.

POURING: Pobieranie składników z butelek do zbiornika głównego.

MIXING: Mieszanie składników.

DISPENSING: Nalewanie pojedynczej porcji (Shot) do kieliszka.

ROTATING: Obrót karuzeli do następnego, pustego kieliszka.

DONE: Zakończenie pracy.

⚙️ Konfiguracja i Kalibracja
Wszystkie ustawienia fizyczne znajdują się w pliku Config.h. Nie należy zmieniać logiki w plikach .cpp, jedynie parametry tutaj.

Adresacja I2C
Upewnij się, że adresy zgadzają się z ustawieniem zworek na modułach:

C++

#define ADDR_RELAYS  0x20
#define ADDR_BUTTONS 0x21
Kalibracja Pomp
Kluczowy parametr dla precyzji drinków. Określa, ile milisekund pompa musi pracować, aby nalać 1 ml płynu.

C++

// Przykład: 80ms pracy = 1ml płynu
#define CALIB_PUMP_1 80.0f 
#define CALIB_PUMP_2 85.0f 
Wskazówka: Jeśli pompa nalewa za mało, zwiększ tę wartość.

⚠️ Bezpieczeństwo
Emergency Stop: Przycisk BACK pełni funkcję wyłącznika bezpieczeństwa. Jego naciśnięcie wywołuje stopAll(), natychmiast odcinając zasilanie wszystkich przekaźników.

Blokowanie: Funkcja obrotu karuzeli (holder->nextPosition()) jest blokująca. W trakcie obrotu interfejs może mieć minimalne opóźnienie.

Dokumentacja przygotowana dla projektu studenckiego MechaSzot.
