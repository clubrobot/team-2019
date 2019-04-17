#include "PressureSensor.h"

int PressureSensor::getPressurekPa()
{
    int sensorValue = analogRead(_pin);
    int pressure = sensorValue * 2;
    return pressure;
}
bool PressureSensor::currentlyAtmospherePressure()
{
    if(getPressurekPa() < _threshold)
    {
        return true;
    } 
    else 
    {
        return false;
    }
}
