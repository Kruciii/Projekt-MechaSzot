#include <Arduino.h>
#include <PCF8574.h>
#include <Pumps.h>

Pump::Pump(int index,int pin, Relay* relay) //Domyślny konstruktor
  {
    this->relay = relay; 
    this->index = index;
    this->pin = pin;
    this->_isOn = false;
    this-> duration =0;
    this-> startTime = 0;
  }
int Pump::getIndex() //Zwraca index danej pompy
  {
    return index;
  }
void Pump::pumpByTime(int time) //Podaj czas w MILISEKUDNACH (1/1000 sekundy)
{
    this->duration = time;
    this->_isOn = true;
    startTime = millis(); //wpisuje aktualnyu czas
    relay ->on(pin);
    this->_isOn  = true;
}
void Pump::pumpByVolume(int ml)
{
    // mlMsConversion stores milliseconds per milliliter, so duration should be ml * ms/ml
    int calculatedMs = (int)(ml * this->mlMsConversion);
    if (calculatedMs <= 0) calculatedMs = 1; // ensure at least 1 ms
    this->pumpByTime(calculatedMs);
}
bool Pump::update() //Zwraca true jeśli wciąż działa
{
    
    if((this->_isOn))
    {
        unsigned long currentTime = millis();
        if(startTime+duration<currentTime)
        {
            relay->off(pin);
            this->_isOn = false;
            this->startTime = 0; 
            this->duration = 0;
            return false;
        }
        return true;
    }
    return false;
}

bool Pump::isOn()
{
    return _isOn;
}