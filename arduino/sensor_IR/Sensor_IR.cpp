#include "Sensor_IR.h"
#include "VL53L0X.h"
#include "VL6180X.h"
#include <Wire.h>

void MyCapteur::begin(void){
  /**
    !!! A placer AVANT le Serial.begin() !!!
    Utile pour pouvoir changer les adresses I2C
  */
  pinMode(XSHUT_pin1, OUTPUT);
  if (XSHUT_pin2!=-1){
    pinMode(XSHUT_pin2, OUTPUT);
  }
}


void MyCapteur::bind(void){
  /**
    Change les Adressses I2C
  */
  // Peut être qu'il peut y avoir une erreur si Adresse est égale à 41
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

void MyCapteur::init(void){
  /**
    On initialise les capteurs
  */
  A.init();
  B.init();
}

void MyCapteur::setTimeout(uint16_t timeout=500){
  /**
    Pour la lecture en continue, on initialise le Timeout
  */
  A.setTimeout(timeout);
  //B.setTimeout(1000); // bug donc on utilise la  fonction RangeSingle (reste la plus rapide)
}

void MyCapteur::configureDefault(void){
  /**
    On effectue une configuration par défaut : La plus rapide possible
  */
  B.configureDefault();
  B.setScaling(1); // Précision max
  A.setMeasurementTimingBudget(200000); // Précision max (au détriment du temps de calcul)
}

void MyCapteur::startContinuous(uint32_t period_ms){
  /**
    Commence le mode continu (Pour le capteur A) # Le B étant plus rapide que le A en restant en Single Range
  */
  setTimeout(500);
  is_continuous = true;
  A.startContinuous();
  //B.startRangeContinuous(); // bug donc on utilise la  fonction RangeSingle (reste la plus rapide)
}

uint16_t MyCapteur::readRangeContinuousMillimeters(void){
  /**
    Choix de la valeur à retourner en fonction de la distance lu par le capteur B (le plus précis)
  */
  if (B.readRangeSingleMillimeters()<dist_switch){
    //return B.readRangeContinuousMillimeters(); // bug donc on utilise la  fonction RangeSingle (reste la plus rapide)
    return B.readRangeSingleMillimeters();
  }
  else {
    return A.readRangeContinuousMillimeters();
  }
}

void MyCapteur::stopContinuous(void){
  /**
    Arrête le Mode continu
  */
  is_continuous = false;
  A.stopContinuous();
  B.stopContinuous();
}

uint16_t MyCapteur::readRangeSingleMillimeters(void){
  /**
    Permet de lire une valeur sans avoir activé le mode continu (Prend donc plus de temps)
  */
  if (B.readRangeSingleMillimeters()<dist_switch){
    return B.readRangeSingleMillimeters();
  }
  else {
    return A.readRangeSingleMillimeters();
  }
}

bool MyCapteur::timeoutOccurred(void){
  /**
    Dans le cas du mode continu, indique si un Timeout a été détecté
  */
  return (A.timeoutOccurred() || B.timeoutOccurred());
}
