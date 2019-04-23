#include <Arduino.h>
#include "instructions.h"
#include "../common/SerialTalks.h"
#include "PIN.h"
#include "VLSensors.h"

VLSensors sensors_IR = VLSensors(SHUT_VL53_1, SHUT_VL61_1);

void setup(){
  Serial.begin(SERIALTALKS_BAUDRATE);
  talks.begin(Serial);
  talks.bind(GET_DIST_OPCODE, GET_DIST);
  talks.bind(GET_STATE_OPCODE, GET_STATE);
  talks.bind(GET_VL61_OPCODE, GET_VL61);
  talks.bind(GET_VL53_OPCODE, GET_VL53);
  Wire.begin();
  sensors_IR.shutBothVL();
  //sensors_IR.setVL53Address(53);
  //sensors_IR.setVL61Address(61);
  sensors_IR.init();
  sensors_IR.setTimestep(0.01);
  sensors_IR.enable();
  

  pinMode(SHUT_VL61_1, OUTPUT);
  digitalWrite(SHUT_VL61_1, LOW);

}

void loop() {
  talks.execute();
  sensors_IR.update();
}