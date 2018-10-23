#ifndef CAPTEUR_IR_h
#define CAPTEUR_IR_h

#include <Arduino.h>
#include "VL53L0X.h"
#include "VL6180X.h"

class MyCapteur
{

  public:

    MyCapteur(int pin1, int pin2, int _Adresse1, int _Adresse2);
    MyCapteur(int pin1, int _Adresse2);
    //~MyCapteur();
    void begin(void);
    void bind(void);
    bool init(void);
    void configureDefault(void);
    void startContinuous(uint32_t period_ms =0);
    uint16_t readRangeContinuousMillimeters(void);
    void stopContinuous(void);
    uint16_t readRangeSingleMillimeters(void);
    void setTimeout(uint16_t timeout);
    bool timeoutOccurred(void);
    VL53L0X A;
    VL6180X B;

  private:
    int XSHUT_pin1;
    int XSHUT_pin2 = -1;
    int Adresse1;
    int Adresse2 = -1;
};


#endif
