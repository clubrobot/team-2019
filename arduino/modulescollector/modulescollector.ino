#include <Arduino.h>
#include <Servo.h>
#include "PIN.h"
#include "instructions.h"
#include "../common/DCMotor.h"
#include "../common/SerialTalks.h"
#include "../common/Clock.h"
#include "../common/VelocityServo.h"
#include "../common/EndStop.h"
#include "../common/FullSpeedServo.h"

const float BRAKEVELOCITY = 1.5;


DCMotorsDriver motorDriver;
DCMotor gripperMotor;

VelocityServo gripper;
FullSpeedServo dispenser;

EndStop highStop;
EndStop lowStop;
EndStop leftmustache;
EndStop rightmustache;

Clock movingTime;
bool elevatorMoving = false;
bool motorError = false;

void setup(){
    Serial.begin(SERIALTALKS_BAUDRATE);
    talks.begin(Serial);
    talks.bind(_WRITE_DISPENSER_OPCODE, WRITE_DISPENSER);
    talks.bind(_WRITE_GRIP_OPCODE, WRITE_GRIP);
    talks.bind(_IS_UP_OPCODE, IS_UP);
    talks.bind(_IS_DOWN_OPCODE, IS_DOWN);
    talks.bind(_SET_MOTOR_VELOCITY_OPCODE, SET_MOTOR_VELOCITY);
    talks.bind(_OPEN_GRIP_OPCODE, OPEN_GRIP);   
    talks.bind(_SET_GRIP_VELOCITY_OPCODE, SET_GRIP_VELOCITY);
    talks.bind(_GET_MOTOR_VELOCITY_OPCODE,GET_MOTOR_VELOCITY);
    talks.bind(_GET_LEFT_MUSTACHE_OPCODE, GET_LEFT_MUSTACHE);
    talks.bind(_GET_RIGHT_MUSTACHE_OPCODE, GET_RIGHT_MUSTACHE);

    pinMode(SERVO2, OUTPUT);
    dispenser.attach(SERVO2);
    dispenser.enable();
    dispenser.setTimestep(0.1);
    
    highStop.attach(SWITCH1);
    lowStop.attach(SWITCH2);

    leftmustache.attach(SERVO1);
    rightmustache.attach(SERVO4);

    pinMode(SERVO3, OUTPUT);
    gripper.attach(SERVO3);
    gripper.enable();
    gripper.setTimestep(0.01);

    motorDriver.attach(DRIVER_RESET , A7);
    motorDriver.reset();
    
    gripperMotor.attach(MOTOR1_EN, MOTOR1_PWM, MOTOR1_DIR);
    gripperMotor.setConstant(1/11.1);

    // Miscellanous
	TCCR2B = (TCCR2B & 0b11111000) | 1; // Set Timer2 frequency to 16MHz instead of 250kHz
    gripper.detach();   //Empeche les deux servos de forcer pour garder leur position au démarrage de l'arduino
    dispenser.detach();
}

void loop(){
     talks.execute();
     gripper.update();
     dispenser.update();

     if(highStop.getState() && gripperMotor.getVelocity()<BRAKEVELOCITY*(-1)){
        gripperMotor.setVelocity(-BRAKEVELOCITY);
        elevatorMoving = false;
     }
     else if(lowStop.getState() && gripperMotor.getVelocity()>BRAKEVELOCITY){
        gripperMotor.setVelocity(+BRAKEVELOCITY);
        elevatorMoving = false;
     }

     if(elevatorMoving && movingTime.getElapsedTime() >= 5){
         gripperMotor.setVelocity(0);
         motorError = true;
     }


}




