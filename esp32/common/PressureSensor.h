#ifndef __PRESSURE_SENSOR_H__
#define __PRESSURE_SENSOR_H__

#include <Arduino.h>

class PressureSensor {
public:
    PressureSensor(int analogPin) : _pin(analogPin){};
    int getPressurekPa();
    bool currentlyAtmospherePressure();
private:
    int _pin;
};
#endif //__PRESSURE_SENSOR_H__
