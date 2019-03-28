#include "PressureSensor.h"

int PressureSensor::getPressurekPa(){
    int sensorValue = analogRead(_pin);
    int pressure = sensorValue*2;//map(sensorValue, 0, 255, 0, 500); //Pressure in kPa
    return pressure;
}
bool PressureSensor::currentlyAtmospherePressure(){
    if(getPressurekPa()<50){
        return true;
    } else {
        return false;
    }
}
