#ifndef SENSOR_2_IR_h
#define SENSOR_2_IR_h

#include <Arduino.h>

#include "Sensor_base.h"
#include "../common/VL6180X.h"

class Sensor_2_IR : public Sensor_base, public VL6180X
{
  public :
    Sensor_2_IR(int pin, int adresse): XSHUT_pin(pin), Adresse(adresse){};
    void begin(void){pinMode(XSHUT_pin, OUTPUT);};
    void bind(void){pinMode(XSHUT_pin, INPUT);delay(10);VL6180X::setAddress(Adresse);};
    void init(void){VL6180X::init();}
    void configureDefault(void){VL6180X::configureDefault();VL6180X::setScaling(1);}; // Précision max
    void startContinuous(uint32_t period_ms =0){VL6180X::setTimeout(500);};
    uint16_t readRangeContinuousMillimeters(void){return VL6180X::readRangeSingleMillimeters();};
    void stopContinuous(void){};
    uint16_t readRangeSingleMillimeters(void){return VL6180X::readRangeSingleMillimeters();};
    void setTimeout(uint16_t timeout){VL6180X::setTimeout(timeout);};
    bool timeoutOccurred(void){return VL6180X::timeoutOccurred();};

  private :
    int XSHUT_pin;
    int Adresse;
};

#endif
