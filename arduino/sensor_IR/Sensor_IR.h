#ifndef CAPTEUR_IR_h
#define CAPTEUR_IR_h

#include <Arduino.h>
#include "VL53L0X.h"
#include "VL6180X.h"
#include "Traitements.h"

/*
  Class permettant l'utilisation en parallèle des deux capteurs
*/

class MyCapteur
{

  public:
    MyCapteur():XSHUT_pin1(14), Adresse1(42){};
    MyCapteur(const MyCapteur& AutreC):is_continuous(AutreC.is_continuous),XSHUT_pin1(AutreC.XSHUT_pin1),XSHUT_pin2(AutreC.XSHUT_pin2),Adresse1(AutreC.Adresse1),Adresse2(AutreC.Adresse2){};
    MyCapteur(int pin1, int pin2, int _Adresse1, int _Adresse2):  XSHUT_pin1(pin1),XSHUT_pin2(pin2),Adresse1(_Adresse1),Adresse2(_Adresse2){};
    MyCapteur(int pin1, int _Adresse2):   XSHUT_pin1(pin1),Adresse2(_Adresse2){};
    ~MyCapteur(void){if(is_continuous) stopContinuous(); is_continuous=false;};
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
    MyCapteur& operator=(const MyCapteur& AutreC){
      this->is_continuous=AutreC.is_continuous;
      this->XSHUT_pin1=AutreC.XSHUT_pin1;
      this->XSHUT_pin2=AutreC.XSHUT_pin2;
      this->Adresse1=AutreC.Adresse1;
      this->Adresse2=AutreC.Adresse2;
      return *this;
    };
    void setNbEchantillonsMG(int nb){ nb_echantillon_MG = nb; MG.reset((uint8_t)nb);};
    void update(){ if (nb_echantillon_MG>0) MG.AddElement(readRangeContinuousMillimeters());}
    float getAverage(){ return MG.getAverage();};
    int getNbEchantillonsMG(void){return nb_echantillon_MG;}

  private:
    MoyenneGlissante MG;
    int nb_echantillon_MG = 0;
    VL53L0X A;
    VL6180X B;
    bool is_continuous = false;
    int XSHUT_pin1;
    int XSHUT_pin2 = -1;
    int Adresse1;
    int Adresse2 = -1;
    float dist_switch = 50;
};


#endif
