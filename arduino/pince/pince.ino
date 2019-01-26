#include <Arduino.h>
#include <Servo.h>
#include "../common/SerialTalks.h"
#include "PIN.h"
#include "instructions.h"

Servo pince;

void setup(){
  Serial.begin(SERIALTALKS_BAUDRATE);
  talks.begin(Serial);
	talks.bind(_SET_POSITION_OPCODE, SET_POSITION);
	pinMode(A2, OUTPUT);
	pince.attach(A2);
	pince.write(100);
	pince.write(10);
}

void loop(){
  talks.execute();
}
