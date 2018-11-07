#include <Arduino.h>
#include <Wire.h>

#include "../common/SerialTalks.h"

#include "Sensor_IR.h"
#include "instructions.h"
#include "Vect_Sensor.h"



MyCapteur Sensor1 = MyCapteur(14, 42); // Objet permettant d'utiliser les deux capteurs en même temps
Vect_Sensor<MyCapteur*> Sensors = {&Sensor1}; // On créé une liste particulière pour éviter les boucles for dans le .ino ?

void setup() {

  Sensors.begin();
  Serial.begin(SERIALTALKS_BAUDRATE);
  talks.begin(Serial);
  Wire.begin();

  talks.bind(GET_SINGLE_MESURE_OPCODE, GET_SINGLE_MESURE);
  talks.bind(GET_CONTINUOUS_MESURE_OPCODE,GET_CONTINUOUS_MESURE);
  talks.bind(START_CONTINUOUS_OPCODE, START_CONTINUOUS);
  talks.bind(STOP_CONTINUOUS_OPCODE, STOP_CONTINUOUS);

  Sensors.bind(); // Permet d'ajuster les différentes adresses pour l'I2C
  Sensors.init();

  Sensors.configureDefault();

}

void loop() {
   talks.execute();
   if (Sensors.timeoutOccurred()) { Serial.println(" TIMEOUT"); }
   Sensors.update();
}
