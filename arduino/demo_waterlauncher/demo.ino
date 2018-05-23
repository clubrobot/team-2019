#include <Arduino.h>
#include <Servo.h>
#include "PIN.h"
#include "instructions.h"
#include "../common/BrushlessMotor.h"
#include "../common/SerialTalks.h"


BrushlessMotor motor;
Servo inDoor;
Servo outDoor;
Servo trash;


void setup(){
    Serial.begin(SERIALTALKS_BAUDRATE);
    talks.begin(Serial);
    talks.bind(_SET_MOTOR_VELOCITY_OPCODE, SET_MOTOR_VELOCITY);
	talks.bind(_WRITE_INDOOR_OPCODE, WRITE_INDOOR);
	talks.bind(_WRITE_OUTDOOR_OPCODE, WRITE_OUTDOOR);
	talks.bind(_WRITE_TRASH_OPCODE, WRITE_TRASH);
	talks.bind(_GET_MOTOR_VELOCITY_OPCODE, GET_MOTOR_VELOCITY);

	pinMode(SERVO1, OUTPUT);
    pinMode(SERVO2, OUTPUT);
	pinMode(SERVO3, OUTPUT);
	pinMode(SERVO4, OUTPUT);

	motor.attach(SERVO4);
	motor.enable();
	motor.setVelocity(MIN_VELOCITY);
	delay(100);

	inDoor.attach(SERVO2);
	outDoor.attach(SERVO3);
	trash.attach(SERVO1);
	trash.write(30);
	outDoor.write(90);
	inDoor.write(90);
}

void loop(){
     talks.execute();
}




