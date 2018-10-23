#include "Sensor_IR.h"
#include "VL53L0X.h"
#include "VL6180X.h"
#include <Wire.h>

MyCapteur::MyCapteur(int pin1, int pin2, int _Adresse1, int _Adresse2){
  XSHUT_pin1 = pin1;
  XSHUT_pin2 = pin2;
  Adresse1 = _Adresse1;
  Adresse2 = _Adresse2;
}

MyCapteur::MyCapteur(int pin1, int _Adresse2){
  XSHUT_pin1 = pin1;
  Adresse2 = _Adresse2;
}

/*MyCapteur::~MyCapteur(){
  //stopContinuous();
}*/

void MyCapteur::begin(void){
  // A placer avant le Serial.begin() !!!
  pinMode(XSHUT_pin1, OUTPUT);
  if (XSHUT_pin2!=-1){
    pinMode(XSHUT_pin2, OUTPUT);
  }
}


void MyCapteur::bind(void){
  // Peut être il peut y avoir une erreur si Adresse est égale à 41
  if (XSHUT_pin2==-1){
    B.setAddress(Adresse2);
    pinMode(XSHUT_pin1, INPUT); // On rallume le premier sans changer son adresse par défaut
    delay(10); //For power-up procedure t-boot max 1.2ms "Datasheet: 2.9 Power sequence"
  }
  else {
    pinMode(XSHUT_pin1, INPUT);
    delay(10);
    A.setAddress(Adresse1);
    pinMode(XSHUT_pin2, INPUT);
    delay(10);
    B.setAddress(Adresse2);
  }
}

bool MyCapteur::init(void){
  A.init();
  B.init();
  return 1;
}

void MyCapteur::setTimeout(uint16_t timeout){
  A.setTimeout(timeout);
  //B.setTimeout(1000); // bug donc on utilise la  fonction RangeSingle (reste la plus rapide)
}

void MyCapteur::configureDefault(void){
  B.configureDefault();
  B.setScaling(1);
  A.setMeasurementTimingBudget(200000);
}

void MyCapteur::startContinuous(uint32_t period_ms){
  setTimeout(500); // Ajouter le Sensor1et2.setScaling à 1 et le Sensor1et2.setTimeout à 500 à HIGH_ACCURACY
  A.startContinuous();
  //B.startRangeContinuous(); // bug donc on utilise la  fonction RangeSingle (reste la plus rapide)
}

uint16_t MyCapteur::readRangeContinuousMillimeters(void){
  if (B.readRangeSingleMillimeters()<50.0){
    //return B.readRangeContinuousMillimeters(); // bug donc on utilise la  fonction RangeSingle (reste la plus rapide)
    return B.readRangeSingleMillimeters();
  }
  else {
    return A.readRangeContinuousMillimeters();
  }
}

void MyCapteur::stopContinuous(void){
  A.stopContinuous();
  B.stopContinuous();
}

uint16_t MyCapteur::readRangeSingleMillimeters(void){

  if (B.readRangeSingleMillimeters()<50.0){
    return B.readRangeSingleMillimeters();
  }
  else {
    return A.readRangeSingleMillimeters();
  }
}

bool MyCapteur::timeoutOccurred(void){
  return (A.timeoutOccurred() || B.timeoutOccurred());
}
