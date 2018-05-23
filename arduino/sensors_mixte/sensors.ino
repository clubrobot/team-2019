#include <Arduino.h>

#include "../common/UltrasonicSensor.h"
#include "../common/SensorListener.h"
#include "../common/EndStop.h"
#include "../common/SerialTalks.h"
#include "instructions.h"
#include "PIN.h"
 

bool activated = true;

UltrasonicSensor SensorAv;
UltrasonicSensor SensorAr;
SensorListener   ListenerAv;
SensorListener   ListenerAr;
EndStop          LeftSwitch;
EndStop          RightSwitch;


void setup() {
    Serial.begin(SERIALTALKS_BAUDRATE);
    talks.begin(Serial);
    talks.bind(GET_MESURE_OPCODE, GET_MESURE);
    talks.bind(GET_NORMAL_OPCODE,GET_NORMAL);
    talks.bind(ACTIVATE_SENSORS_OPCODE, ACTIVATE_SENSORS);
    talks.bind(DESACTIVATE_SENSORS_OPCODE, DESACTIVATE_SENSORS);
    talks.bind(GET_LEFT_SWITCH_OPCODE,GET_LEFT_SWITCH);
    talks.bind(GET_RIGHT_SWITCH_OPCODE,GET_RIGHT_SWITCH);
    SensorAv.attach(TRIGGPIN7, ECHOPIN2);
    SensorAr.attach(TRIGGPIN8, ECHOPIN3);
    SensorAv.trig();
    SensorAr.trig();
    ListenerAr.setTimestep(0.025);
    ListenerAv.setTimestep(0.025);
    ListenerAv.attach(&SensorAv,1000);
    ListenerAr.attach(&SensorAr,1000);
    ListenerAr.enable();
    ListenerAv.enable();
    LeftSwitch.attach(LEFT_SWITCH_PIN);
    RightSwitch.attach(RIGHT_SWITCH_PIN);

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


