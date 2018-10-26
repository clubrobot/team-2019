#include <Arduino.h>
#include <Servo.h>
#include "PIN.h"
#include "../../common/SoftwareSerial.h"
#include "../../common/AX12.h"
#include "../../common/SerialTalks.h"
#include "../../common/DCMotor.h"
#include "../../common/EndStop.h"
#include "instructions.h"


SoftwareSerial SoftSerial(RX, TX);

AX12 servoax; //bibliothèque_Antoine

DCMotorsDriver motorDriver;

DCMotor motor1; //bibliothèque_Ulysse
DCMotor motor2;

Servo servo1; //bibbliothèque arduino
Servo servo2;
Servo servo3;
Servo servo4;

EndStop highStop; //bibliothèque_Antoine
EndStop lowStop;

// 2 tout ou rien à gérer

void setup(){
  Serial.begin(SERIALTALKS_BAUDRATE);
  talks.begin(Serial);
  talks.bind(_SET_MOTOR1_VELOCITY_OPCODE, SET_MOTOR1_VELOCITY);
  talks.bind(_SET_MOTOR2_VELOCITY_OPCODE,  SET_MOTOR2_VELOCITY);
  talks.bind(_SET_AX_POSITION_OPCODE, SET_AX_POSITION);
  talks.bind(_GET_AX_TORQUE_OPCODE, GET_AX_TORQUE);
  talks.bind(_SET_AX_VELOCITY_MOVE_OPCODE,  SET_AX_VELOCITY_MOVE);
  talks.bind(_PING_AX_OPCODE, PING_AX);
  talks.bind(_SET_AX_HOLD_OPCODE, SET_AX_HOLD);
  talks.bind(_GET_AX_POSITION_OPCODE, GET_AX_POSITION);
  talks.bind(_IS_UP_OPCODE, IS_UP);
  talks.bind(_IS_DOWN_OPCODE, IS_DOWN);
  talks.bind(_SET_SERVO_OPCODE, SET_SERVO);
  talks.bind(_SET_SERVO_MICROSECONDS_OPCODE, SET_SERVO_MICROSECONDS);
  talks.bind(_SET_TOR_OPCODE, SET_TOR);
  talks.bind(_STOP_OPCODE, STOP);

  AX12::SerialBegin(9600,RX,TX,DATA_CONTROL);

  motorDriver.attach(DRIVER_RESET , A7);
  motorDriver.reset();
    
  motor2.attach(MOTOR2_EN, MOTOR2_PWM, MOTOR2_DIR);
  motor2.setConstant(1/11.1);

  motor1.attach(MOTOR1_EN, MOTOR1_PWM, MOTOR1_DIR);
  motor1.setConstant(1/11.1);

  servo1.attach(SERVO1);
  servo2.attach(SERVO2);
  servo3.attach(SERVO3);
  servo4.attach(SERVO4);

  highStop.attach(SWITCH1);
  lowStop.attach(SWITCH2);

  
   // Miscellanous
	//TCCR2B = (TCCR2B & 0b11111000) | 1;

  servoax.attach(1);
  servoax.setShutdownAlarm(0);
  servoax.setMaxTorque(1023);
  servoax.setEndlessMode(OFF);
  servoax.hold(OFF);

  pinMode(TOR_1, OUTPUT);
  pinMode(TOR_2, OUTPUT);
}

void loop(){
  talks.execute();
}