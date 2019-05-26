#include "pin.h"
#include <SPI.h>
#include "../common/SerialTalks.h"
#include "instructions.h"
#include "../common/BNO055.h"
#include "AccelerationController.h"

BNO055 bno = BNO055();
bool connected = false;
AccelerationController meanAcc = AccelerationController();

void setup() {
  
  Serial.begin(SERIALTALKS_BAUDRATE);
  talks.begin(Serial);
  talks.bind(GET_ACCELERATION_OPCODE, GET_ACCELERATION);
  talks.bind(GET_MAGNETIC_OPCODE,     GET_MAGNETIC);            
  talks.bind(GET_ORIENTATION_OPCODE , GET_ORIENTATION);           
  talks.bind(GET_GYROSCOPE_OPCODE,    GET_GYROSCOPE);       
  talks.bind(GET_LINEAR_ACCELERATION_OPCODE, GET_LINEAR_ACCELERATION);  
  talks.bind(GET_TEMPERATURE_OPCODE,  GET_TEMPERATURE);
  talks.bind(GET_STATE_OPCODE,        GET_STATE);
  talks.bind(READ_BYTE_OPCODE ,       READ_BYTE);
  talks.bind(IS_CONNECTED_OPCODE,     IS_CONNECTED);
  talks.bind(BEGIN_COM_OPCODE,        BEGIN_COM);
  talks.bind(READ_LEN_OPCODE,         READ_LEN);
  talks.bind(GET_ABSOLUTE_ACCELERATION_OPCODE, GET_ABSOLUTE_ACCELERATION);
  talks.bind(SET_ORIENTATION_OPCODE, SET_ORIENTATION);

  bno.activateAll(false);
  bno.activateAbsO(true);
  bno.activateLinAcc(true);
  bno.setTimestep(0.01);

  connected = bno.begin(PIN_SDA, PIN_SCL);
  bno.enable();
}

void loop() {
  talks.execute();
  bno.update();
   
  if(bno.absoReady() && bno.linAccReady()){
    meanAcc.computeProjection(bno.getLinAcc().getx(), bno.getLinAcc().gety(),bno.getLinAcc().getz(), bno.getAbsO().getx(), bno.getAbsO().gety(), bno.getAbsO().getz());
  }
}
