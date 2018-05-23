#include <Arduino.h>
#include <Servo.h>
//#include <Wire.h>
#include "PIN.h"
#include "instructions.h"
#include "../common/BrushlessMotor.h"
#include "../common/SerialTalks.h"
#include "../common/Adafruit_TCS34725.h"
#include "../common/Wire.h"
#include "BallsShaker.h"

BrushlessMotor motor;
Servo indoor;
Servo outdoor;
Servo trash;
Servo trashUnloader;
BallsShaker shaker;
Servo beeActivator;

int ballCount = 0;
bool lastState = HIGH;
unsigned long timeDebouncer = 0;

Adafruit_TCS34725 waterSensor = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

#define INDOOR_DOOR_OPEN  47
#define INDOOR_DOOR_CLOSED  8

#define OUTDOOR_DOOR_OPEN  40
#define OUTDOOR_DOOR_CLOSED  90

#define TRASH_BEFORE_CLOSE  90
#define TRASH_CLOSED  120
#define TRASH_OPEN  149

#define TRASH_UNLOADER_OPEN  140
#define TRASH_UNLOADER_CLOSED  90

#define BEE_CLOSED 0

void resetVelocity();
void incrementBallCount();

void setup(){
  Serial.begin(SERIALTALKS_BAUDRATE);
  talks.begin(Serial);
	talks.bind(_GET_INDOOR_OPCODE, GET_INDOOR);
	talks.bind(_WRITE_INDOOR_OPCODE, WRITE_INDOOR);
	talks.bind(_GET_OUTDOOR_OPCODE, GET_OUTDOOR);
	talks.bind(_WRITE_OUTDOOR_OPCODE, WRITE_OUTDOOR);
	talks.bind(_GET_TRASH_OPCODE, GET_TRASH);
	talks.bind(_WRITE_TRASH_OPCODE, WRITE_TRASH);
	talks.bind(_GET_SHAKER_HORIZONTAL_OPCODE, GET_SHAKER_HORIZONTAL);
	talks.bind(_WRITE_SHAKER_HORIZONTAL_OPCODE, WRITE_SHAKER_HORIZONTAL);
	talks.bind(_GET_SHAKER_VERTICAL_OPCODE, GET_SHAKER_VERTICAL);
	talks.bind(_WRITE_SHAKER_VERTICAL_OPCODE, WRITE_SHAKER_VERTICAL);
	talks.bind(_GET_TRASH_UNLOADER_OPCODE, GET_TRASH_UNLOADER);
	talks.bind(_WRITE_TRASH_UNLOADER_OPCODE, WRITE_TRASH_UNLOADER);
	talks.bind(_GET_MOTOR_VELOCITY_OPCODE, GET_MOTOR_VELOCITY);
  talks.bind(_SET_MOTOR_VELOCITY_OPCODE, SET_MOTOR_VELOCITY);
	talks.bind(_GET_WATER_COLOR_OPCODE, GET_WATER_COLOR);
	talks.bind(_SET_MOTOR_PULSEWIDTH_OPCODE, SET_MOTOR_PULSEWIDTH);
	talks.bind(_GET_MOTOR_PULSEWIDTH_OPCODE, GET_MOTOR_PULSEWIDTH);
	talks.bind(_SET_LED_OFF_OPCODE, SET_LED_OFF);
	talks.bind(_SET_LED_ON_OPCODE, SET_LED_ON);
	talks.bind(_FORCE_PULSEWIDTH_OPCODE, FORCE_PULSEWIDTH);
	talks.bind(_ENABLE_SHAKING_DIFF_OPCODE, ENABLE_SHAKING_DIFF);
	talks.bind(_ENABLE_SHAKING_EQUAL_OPCODE, ENABLE_SHAKING_EQUAL);
	talks.bind(_DISABLE_SHAKING_OPCODE, DISABLE_SHAKING);
	talks.bind(_WRITE_BEEACTIVATOR_OPCODE, WRITE_BEEACTIVATOR);
	talks.bind(SERIALTALKS_DISCONNECT_OPCODE, DISABLE);
	talks.bind(_SET_SHAKER_VELOCITY_OPCODE, SET_SHAKER_VELOCITY);
  talks.bind(_GET_LAUNCHED_WATER_OPCODE, GET_LAUNCHED_WATER);
  talks.bind(_GET_EMERGENCY_STATE_OPCODE, GET_EMERGENCY_STATE);


	pinMode(SERVO1, OUTPUT);
  pinMode(SERVO2, OUTPUT);
	pinMode(SERVO3, OUTPUT);
	pinMode(SERVO4, OUTPUT);
	pinMode(SERVO5, OUTPUT);
	pinMode(SERVO6, OUTPUT);
	pinMode(SWITCH2, OUTPUT);
	pinMode(BRUSHLESS, OUTPUT);
	pinMode(SWITCH1, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(3), incrementBallCount, FALLING);
	attachInterrupt(digitalPinToInterrupt(SWITCH1), resetVelocity, FALLING);

	motor.attach(BRUSHLESS);
	motor.setVelocity(MIN_VELOCITY);
	motor.enableMotor();

	indoor.attach(SERVO1);
	trashUnloader.attach(SERVO4);
	shaker.attachVertical(SERVO6);
	shaker.attachHorizontal(SERVO5);
	outdoor.attach(SERVO2);
	trash.attach(SERVO3);
	beeActivator.attach(SWITCH2);


	beeActivator.write(BEE_CLOSED);
	trash.write(TRASH_CLOSED);
	outdoor.write(OUTDOOR_DOOR_CLOSED);
	indoor.write(INDOOR_DOOR_CLOSED);
	trashUnloader.write(TRASH_UNLOADER_CLOSED);
	shaker.writeHorizontal(SHAKER_HORIZONTAL_1);
  shaker.writeVertical(SHAKER_VERTICAL_1);

	waterSensor.begin();
}

void incrementBallCount(){
  if(micros() - timeDebouncer > 500000){
      ballCount++;
      timeDebouncer = micros();
  }
}


void resetVelocity(){
	motor.enableStartup();
}

void loop(){
  talks.execute();
	motor.updateStartup();
	shaker.updateShaker();
}
