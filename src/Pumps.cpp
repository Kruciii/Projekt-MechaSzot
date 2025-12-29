#include <Arduino.h>
#include <PCF8574.h>
#include <Pumps.h>


//FUNCKJE PUMP



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
    this->pumpByTime(float(ml)/this->mlMsConversion);
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

int Pump::getIndex()
{
    return index;
}
bool Pump::isOn()
{
    return _isOn;
}