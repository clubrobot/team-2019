#include <Arduino.h>

#include <Wire.h>
#include "../common/SerialTalks.h"

#include "Sensor_IR.h"
#include "Traitements.h"
#include "instructions.h"



MyCapteur Sensor(14, 42); // Objet permettant d'utiliser les deux capteurs en même temps
MoyenneGlissante MG(0, NULL);
int nb_echantillon_MG = 0;

void setup() {

  Sensor.begin();
  Serial.begin(SERIALTALKS_BAUDRATE);
  talks.begin(Serial);
  Wire.begin();

  talks.bind(GET_SINGLE_MESURE_OPCODE, GET_SINGLE_MESURE);
  talks.bind(GET_CONTINUOUS_MESURE_OPCODE,GET_CONTINUOUS_MESURE);
  talks.bind(START_CONTINUOUS_OPCODE, START_CONTINUOUS);
  talks.bind(STOP_CONTINUOUS_OPCODE, STOP_CONTINUOUS);

  Sensor.bind(); // Permet d'ajuster les différentes adresses pour l'I2C
  Sensor.init();

  Sensor.configureDefault();
  //Sensor.startContinuous();// A enlever si SerialTalks fonctionne !!

}

void loop() {
   //Serial.println(Sensor.readRangeContinuousMillimeters());
   //MG.AddElement(Sensor.readRangeContinuousMillimeters());
   talks.execute();
   if (Sensor.timeoutOccurred()) { Serial.println(" TIMEOUT"); }
   if (nb_echantillon_MG>0) {
     MG.AddElement(Sensor.readRangeContinuousMillimeters());
     //Serial.println(MG.getAverage());
   }
   //Serial.println(Sensor.readRangeContinuousMillimeters());
}
