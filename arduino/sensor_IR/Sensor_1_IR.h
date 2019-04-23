#ifndef SENSOR_1_IR_h
#define SENSOR_1_IR_h

#include <Arduino.h>

#include "Sensor_base.h"
#include "../common/VL53L0X.h"

class Sensor_1_IR : public Sensor_base, public VL53L0X
{
  public :
    Sensor_1_IR(int pin, int adresse): XSHUT_pin(pin), Adresse(adresse){};
    void begin(void){pinMode(XSHUT_pin, OUTPUT);};
    void bind(void){pinMode(XSHUT_pin, INPUT);delay(10);VL53L0X::setAddress(Adresse);};
    void init(void){VL53L0X::init();}
    void configureDefault(void){VL53L0X::setMeasurementTimingBudget(200000);}; // Précision max (au détriment du temps de calcul)
    void startContinuous(uint32_t period_ms =0){VL53L0X::setTimeout(500);VL53L0X::startContinuous();};
    uint16_t readRangeContinuousMillimeters(void){return VL53L0X::readRangeContinuousMillimeters();};
    void stopContinuous(void){VL53L0X::stopContinuous();};
    uint16_t readRangeSingleMillimeters(void){return VL53L0X::readRangeSingleMillimeters();};
    void setTimeout(uint16_t timeout){VL53L0X::setTimeout(timeout);};
    bool timeoutOccurred(void){return VL53L0X::timeoutOccurred();};

  private :
    int XSHUT_pin;
    int Adresse;
};

#endif
