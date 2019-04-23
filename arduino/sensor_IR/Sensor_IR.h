#ifndef SENSOR_IR_h
#define SENSOR_IR_h

#include <Arduino.h>
#include "../common/VL53L0X.h"
#include "../common/VL6180X.h"
#include "Sensor_base.h"

/*
  Class permettant l'utilisation en parallèle des deux capteurs
*/

class Sensor_IR : public Sensor_base
{

  public:
    Sensor_IR():XSHUT_pin1(14), Adresse1(42){};
    Sensor_IR(const Sensor_IR& AutreC):is_continuous(AutreC.is_continuous),XSHUT_pin1(AutreC.XSHUT_pin1),XSHUT_pin2(AutreC.XSHUT_pin2),Adresse1(AutreC.Adresse1),Adresse2(AutreC.Adresse2){};
    Sensor_IR(int pin1, int pin2, int _Adresse1, int _Adresse2):  XSHUT_pin1(pin1),XSHUT_pin2(pin2),Adresse1(_Adresse1),Adresse2(_Adresse2){};
    Sensor_IR(int pin1, int _Adresse2):   XSHUT_pin1(pin1),Adresse2(_Adresse2){};
    ~Sensor_IR(void){if(is_continuous) stopContinuous(); is_continuous=false;};
    void begin(void);
    void bind(void);
    void init(void);
    void configureDefault(void);
    void startContinuous(uint32_t period_ms =0); // Par défaut : Aussi rapide que possible
    uint16_t readRangeContinuousMillimeters(void);
    void stopContinuous(void);
    uint16_t readRangeSingleMillimeters(void);
    void setTimeout(uint16_t timeout);
    bool timeoutOccurred(void);
    Sensor_IR& operator=(const Sensor_IR& AutreC){
      this->is_continuous=AutreC.is_continuous;
      this->XSHUT_pin1=AutreC.XSHUT_pin1;
      this->XSHUT_pin2=AutreC.XSHUT_pin2;
      this->Adresse1=AutreC.Adresse1;
      this->Adresse2=AutreC.Adresse2;
      return *this;
    };
    //void setNbEchantillonsMG(int nb){reset((uint8_t)nb);};
    //void update(){ if (getNbEchantillonsMG()>0) AddElement(readRangeContinuousMillimeters());};

  private:
    VL53L0X A;
    VL6180X B;
    bool is_continuous = false;
    int XSHUT_pin1;
    int XSHUT_pin2 = -1;
    int Adresse1;
    int Adresse2 = -1;
    float dist_switch = 150;
};


#endif
