#include <Arduino.h>
#include "instructions.h"
#include "../common/SerialTalks.h"
#include "../common/LSM303.h"
#include "../common/MagneticCompas.h"
#include "AccelerationController.h"
#include "PIN.h"

Lsm303 acmaSensor = Lsm303();
MagneticCompas cmp = MagneticCompas();
AccelerationController acc = AccelerationController();

void setup(){
  Serial.begin(SERIALTALKS_BAUDRATE);
  talks.begin(Serial);
  talks.bind(GET_ROBOT_ACC_OPCODE, GET_ROBOT_ACC);
  talks.bind(CALIBRATE_COMPAS_OPCODE, CALIBRATE_COMPAS);
  talks.bind(GET_ORIENTATION_OPCODE, GET_ORIENTATION);
  talks.bind(GET_MEASURED_ORIENTATION_OPCODE, GET_MEASURED_ORIENTATION);
  talks.bind(GET_SENSOR_VALUE_OPCODE, GET_SENSOR_VALUE);
  talks.bind(GET_SENSOR_STATE_OPCODE, GET_SENSOR_STATE);
  acmaSensor.setTimestep(0.01);
  acmaSensor.enable();
}

void loop() {
  talks.execute();
  acmaSensor.update();
  if(acmaSensor.dataAvailable()){
    acc.computeProjection(acmaSensor.getAccX(), acmaSensor.getAccY(), cmp.computeOrientation(acmaSensor.getMagX(), acmaSensor.getMagY()));
    acmaSensor.setDataOld();
  }
}