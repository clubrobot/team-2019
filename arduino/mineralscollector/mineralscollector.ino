#include <Arduino.h>
#include <Servo.h>
#include "PIN.h"
#include "../common/SoftwareSerial.h"
#include "../common/AX12.h"
#include "../common/SerialTalks.h"
#include "../common/DCMotor.h"
#include "instructions.h"
#include "../common/PeriodicProcess.h"
#include "SafePosition.h"
#include "../common/EndStop.h"
#include "../common/Clock.h"


SoftwareSerial SoftSerial(RX, TX);

AX12 servoax;

Servo launchPad;

Clock tps;

DCMotorsDriver motorDriver;

DCMotor rollerMotor;
DCMotor hammerMotor;

SafePosition safeHammer(&hammerMotor);

bool launchingPosition;
bool validPress = true; 

EndStop button; 

void setup(){
  Serial.begin(SERIALTALKS_BAUDRATE);
  talks.begin(Serial);
  talks.bind(_SET_ROLLER_VELOCITY_OPCODE, SET_ROLLER_VELOCITY);
  talks.bind(_SET_FIRING_HAMMER_VELOCITY_OPCODE,  SET_FIRING_HAMMER_VELOCITY);
  talks.bind(_RETURN_TO_SAFE_POSITION_OPCODE, RETURN_TO_SAFE_POSITION);
  talks.bind(_SETUP_AX_OPCODE, SETUP_AX);
  talks.bind(_SET_AX_POSITION_OPCODE, SET_AX_POSITION);
  talks.bind(_GET_AX_TORQUE_OPCODE, GET_AX_TORQUE);
  talks.bind(_SET_AX_VELOCITY_MOVE_OPCODE,  SET_AX_VELOCITY_MOVE);
  talks.bind(_PING_AX_OPCODE, PING_AX);
  talks.bind(_SET_AX_HOLD_OPCODE, SET_AX_HOLD);
  talks.bind(_GET_AX_POSITION_OPCODE, GET_AX_POSITION);
  talks.bind(AX12_SEND_INSTRUCTION_PACKET_OPCODE, AX12_SEND_INSTRUCTION_PACKET);
  talks.bind(AX12_RECEIVE_STATUS_PACKET_OPCODE, AX12_RECEIVE_STATUS_PACKET);
  talks.bind(_GET_AX_VELOCITY_OPCODE, GET_AX_VELOCITY);
  talks.bind(_GET_AX_MOVING_OPCODE, GET_AX_MOVING);
  talks.bind(LAUNCHPAD_SET_POSITION_OPCODE, LAUNCHPAD_SET_POSITION);

  AX12::SerialBegin(9600, RX, TX, DATA_CONTROL);

  motorDriver.attach(DRIVER_RESET , A7);
  motorDriver.reset();
    
  rollerMotor.attach(MOTOR2_EN, MOTOR2_PWM, MOTOR2_DIR);
  rollerMotor.setConstant(1/11.1);

  hammerMotor.attach(MOTOR1_EN, MOTOR1_PWM, MOTOR1_DIR);
  hammerMotor.setConstant(1/11.1);

   // Miscellanous
	TCCR2B = (TCCR2B & 0b11111000) | 1;

  safeHammer.setTimestep(3);
  safeHammer.toSafePosition();

  launchPad.attach(SERVO1);
  launchPad.write(159);
  launchingPosition = false;

  button.attach(SWITCH1);
}

void loop(){
  talks.execute();
  safeHammer.update();

  if(button.getState() && validPress && !launchingPosition && tps.getElapsedTime() >= 0.5){
    launchPad.write(60);
    launchingPosition = true;
    tps.restart();
    validPress = false;
  }

  else if(button.getState() && validPress && launchingPosition && tps.getElapsedTime() >= 0.5){
    launchPad.write(159);
    launchingPosition = false;
    tps.restart();
    validPress = false;
  }

  else if(!button.getState()){
    validPress = true;
  }
}