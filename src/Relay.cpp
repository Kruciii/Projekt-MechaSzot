#include <Arduino.h>
#include <PCF8574.h>
#include <Relay.h>
#include "Relay.h"

Relay::Relay(uint8_t addr, bool activeLow)
{
    _addr = addr;
    _activeLow = activeLow;
    pcf = new PCF8574(_addr);

    // Ustalenie logiki sterowania
    if (_activeLow)
    {
        ON = LOW;
        OFF = HIGH;
    }
    else
    {
        ON = HIGH;
        OFF = LOW;
    }
    // UWAGA: Nie wywołujemy tu pinMode ani Wire!
}
Relay::Relay(uint8_t addr, PCF8574 *expander, bool activeLow)
{
    _addr = addr;
    _activeLow = activeLow;
    pcf = expander;

    // Ustalenie logiki sterowania
    if (_activeLow)
    {
        ON = LOW;
        OFF = HIGH;
    }
    else
    {
        ON = HIGH;
        OFF = LOW;
    }
    // UWAGA: Nie wywołujemy tu pinMode ani Wire!
}
bool Relay::begin()
{
    for (int i = 0; i < 8; i++)
    {
        pcf->pinMode(i, OUTPUT);
    }
    // Najpierw inicjalizujemy PCF
    if (pcf->begin())
    {
        Serial.println("OK");
    }
    else
    {
        Serial.println("KO");
        return false;
    }

    Serial.println("PCF8574 init ok");

    // Najpierw ustawiamy stan WYŁĄCZONY na wszystkich pinach,
    // aby uniknąć przypadkowego włączenia przekaźników przy ustawianiu pinMode.
    for (int i = 0; i < 8; ++i)
    {
        pcf->digitalWrite(i, ON);
    }

    return true;
}

void Relay::on(uint8_t pin)
{
    if (pcf != nullptr && pin < 8)
        pcf->digitalWrite(pin, ON);
}

void Relay::off(uint8_t pin)
{
    if (pcf != nullptr && pin < 8)
        pcf->digitalWrite(pin, OFF);
}

void Relay::toggle(uint8_t pin)
{
    if (!pcf || pin >= 8)
        return;

    uint8_t currentState = pcf->digitalRead(pin);
    // Jeśli obecny stan to ON, ustawiamy OFF i odwrotnie
    pcf->digitalWrite(pin, (currentState == ON) ? OFF : ON);
}

void Relay::allOff()
{
    if (!pcf)
        return;
    for (int i = 0; i < 8; i++)
        off(i);
}

void Relay::scan()
{
    byte err, addr;
    int n = 0;
    Serial.println("I2C scan...");
    // Wire.begin() powinno być wywołane w setup() głównego programu,
    // ale dla pewności skaner działa na istniejącej konfiguracji.
    for (addr = 1; addr < 127; ++addr)
    {
        Wire.beginTransmission(addr);
        err = Wire.endTransmission();
        if (err == 0)
        {
            Serial.print("Found @0x");
            Serial.println(addr, HEX);
            ++n;
        }
    }
    Serial.printf("%d device(s)\n", n);
}

void Relay::test()
{
    Serial.println("Test sekwencyjny...");
    for (uint8_t i = 0; i < 8; ++i)
    {
        on(i);
        delay(500); // Skróciłem czas dla szybszego testu
        off(i);
        delay(200);
    }
}