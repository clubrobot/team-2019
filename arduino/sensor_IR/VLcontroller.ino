#include <Arduino.h>
#include "instructions.h"
#include "../common/SerialTalks.h"
#include "PIN.h"
#include "VLSensors.h"

VLSensors sensors_IR = VLSensors(SHUT_VL53_1, SHUT_VL61_1);

void setup(){
  Serial.begin(SERIALTALKS_BAUDRATE);
  talks.begin(Serial);
  talks.bind(GET_ROBOT_ACC_OPCODE, GET_ROBOT_ACC);
  Wire.begin();
  sensors_IR.setVL53Address(53);
  sensors_IR.setVL61Address(61);
  sensors_IR.init();
  sensors_IR.setTimestep(0.01);
  sensors_IR.startContinuous();
  sensors_IR.enable();

}

void loop() {
  talks.execute();
  sensors_IR.update();

}