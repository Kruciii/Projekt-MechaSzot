#include <Arduino.h>
#include <PCF8574.h>
#include <Relay.h>
Relay::Relay(uint8_t addr)
{
    pcf = new PCF8574(addr);
    for (int i = 0; i < 8; ++i)
        pcf->pinMode(i, OUTPUT);
    
}

bool Relay::begin()
{
    if (pcf->begin())
    {
        Serial.println("PCF8574 init ok");
        return true;
    }
    delete pcf;
    Serial.println("PCF8574 init failed");
    pcf = nullptr;
    return false;
}

void Relay::on(uint8_t pin)
{
    if (pcf != nullptr && pin < 8)
        pcf->digitalWrite(pin, onState);
}
void Relay::off(uint8_t pin)
{
    if (pcf != nullptr && pin < 8)
        pcf->digitalWrite(pin, offState);
}
void Relay::toggle(uint8_t pin)
{
    if (!pcf)
        return;
    uint8_t s = pcf->digitalRead(pin);
    pcf->digitalWrite(pin, !s);
}
void Relay::scan()
{
    byte err, addr;
    int n = 0;
    Serial.println("I2C scan...");
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
    delay(5000);
}
void Relay::test()
{
    for (uint8_t i = 0; i < 8; ++i)
    {

        on(i);
        delay(2000);
        off(i);
    }
}