#include <Arduino.h>
#include <Servo.h>
#include "../common/SerialTalks.h"
#include "PIN.h"
#include "instructions.h"

Servo gripper;
Servo pusher1;
Servo pusher2;

void setup(){
  gripper.attach(SERVO1);
  pusher1.attach(SERVO3);
  pusher2.attach(SERVO4);
  Serial.begin(SERIALTALKS_BAUDRATE);
  talks.begin(Serial);
	talks.bind(_SET_POSITION_PUSHERS_OPCODE, SET_POSITION_PUSHERS);
  talks.bind(_SET_POSITION_GRIPPER_OPCODE, SET_POSITION_GRIPPER);
}

void loop(){
  talks.execute();
}
