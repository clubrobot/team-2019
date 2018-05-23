#include <Arduino.h>

#include "../common/UltrasonicSensor.h"
#include "../common/SensorListener.h"
#include "../common/SerialTalks.h"
#include "instructions.h"
#include "PIN.h"
 

bool activated = true;

UltrasonicSensor SensorAv;
UltrasonicSensor SensorAr;
SensorListener   ListenerAv;
SensorListener   ListenerAr;

void setup() {
    Serial.begin(SERIALTALKS_BAUDRATE);
    talks.begin(Serial);
    talks.bind(GET_MESURE_OPCODE, GET_MESURE);
    talks.bind(GET_NORMAL_OPCODE,GET_NORMAL);
    talks.bind(ACTIVATE_SENSORS_OPCODE, ACTIVATE_SENSORS);
    talks.bind(DESACTIVATE_SENSORS_OPCODE, DESACTIVATE_SENSORS);
    SensorAv.attach(TRIGGPIN7, ECHOPIN2);
    SensorAr.attach(TRIGGPIN8, ECHOPIN3);
    SensorAv.trig();
    SensorAr.trig();
    ListenerAr.setTimestep(0.025);
    ListenerAv.setTimestep(0.025);
    ListenerAv.attach(&SensorAv,500);
    ListenerAr.attach(&SensorAr,500);
    ListenerAr.enable();
    ListenerAv.enable();

}


void loop() {
  talks.execute(); 
  SensorAv.update();
  SensorAr.update();
  ListenerAv.update();
  ListenerAr.update();

  if (SensorAv.getReady() && activated) {
    SensorAv.trig();
  }
  if (SensorAr.getReady() && activated){
      SensorAr.trig();
  }
}


