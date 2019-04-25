#include <Arduino.h>
#include <Wire.h>

#include "../common/SerialTalks.h"

#include "Sensor_base.h"
#include "Sensor_IR.h"
#include "Sensor_1_IR.h"
#include "Sensor_2_IR.h"
#include "instructions.h"
#include "Vect_Sensor.h"



Sensor_1_IR Sensor1 = Sensor_1_IR(14, 42);
Sensor_2_IR Sensor2 = Sensor_2_IR(15, 43);
Vect_Sensor<Sensor_base*> Sensors = {&Sensor1, &Sensor2}; // On créé une liste particulière pour éviter les boucles for dans le .ino

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
