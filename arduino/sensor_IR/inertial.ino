#include <Arduino.h>
#include "instructions.h"
#include "../common/SerialTalks.h"
#include "../common/LSM303.h"
#include "../common/MagneticCompas.h"
#include "AccelerationController.h"
#include "PIN.h"

Lsm303 acmaSensor = Lsm303();

void setup(){
  Serial.begin(SERIALTALKS_BAUDRATE);
  talks.begin(Serial);
  talks.bind(GET_ROBOT_ACC_OPCODE, GET_ROBOT_ACC);
  acmaSensor.setTimestep(0.01);
  acmaSensor.enable();
}

void loop() {
  talks.execute();
  acmaSensor.update();
}