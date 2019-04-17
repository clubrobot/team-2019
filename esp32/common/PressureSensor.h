#ifndef __PRESSURE_SENSOR_H__
#define __PRESSURE_SENSOR_H__

#include <Arduino.h>

class PressureSensor 
{
    public:
        PressureSensor(int analogPin) : _pin(analogPin), _threshold(100.0){};
        int getPressurekPa();
        bool currentlyAtmospherePressure();
        void change_pressure_threshold(float threshold) {_threshold = threshold;}
        float get_pressure_threshold() {return _threshold;}
    private:
        int _pin;
        float _threshold;
};
#endif //__PRESSURE_SENSOR_H__
