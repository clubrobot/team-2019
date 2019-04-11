#include <Arduino.h>
#include <Servo.h>
#include "../common/SerialTalks.h"
#include "../common/EndStop.h"
#include "PIN.h"
#include "instructions.h"

Servo gripper;
Servo pusher1;
Servo pusher2;
Servo arm;
EndStop endStop1;
EndStop endStop2;

void setup(){
  endStop1.attach(SWITCH1);
  endStop2.attach(SWITCH2);
  Serial.begin(SERIALTALKS_BAUDRATE);
  talks.begin(Serial);
	talks.bind(_SET_POSITION_PUSHERS_OPCODE, SET_POSITION_PUSHERS);
  talks.bind(_SET_POSITION_GRIPPER_OPCODE, SET_POSITION_GRIPPER);
  talks.bind(_SET_POSITION_ARM_OPCODE, SET_POSITION_ARM);
  talks.bind(_GET_ENDSTOP1_STATE_OPCODE, GET_ENDSTOP1_STATE);
  talks.bind(_GET_ENDSTOP2_STATE_OPCODE, GET_ENDSTOP2_STATE);

}

void loop(){
  talks.execute();
}
