#include <Arduino.h>
#include <Servo.h>
#include "../common/DCMotor.h"
#include "./pin.h"

int selectedServo = 0;
int selectedDCMotor = 0;
const int SERVO = 0;
const int AX12 = 2;
const int DCMOTOR = 1;
int mode = DCMOTOR;

Servo* servos[3];
DCMotorsDriver motorDriver;

DCMotor motor1;
DCMotor motor2;



void setup(){
  for(int n=0;n<3;n++){
    servos[n] = (new Servo());
  }

  servos[0]->attach(SERVO1);
  servos[1]->attach(SERVO2);
  servos[2]->attach(SERVO3);

  motorDriver.attach(DRIVER_RESET , DRIVER);
  motorDriver.reset();
    
  motor1.attach(MOTOR2_EN, MOTOR2_PWM, MOTOR2_DIR);
  motor1.setConstant(1/11.1);

  motor2.attach(MOTOR1_EN, MOTOR1_PWM, MOTOR1_DIR);
  motor2.setConstant(1/11.1);

  pinMode(BUTTONMODE, INPUT_PULLUP);
  pinMode(BUTTONSELECT, INPUT_PULLUP);
  motor1.setVelocity(0);
  motor2.setVelocity(0);
}

void loop(){
   delay(50);

   if(digitalRead(BUTTONMODE) == HIGH){
       mode++;
       while(digitalRead(BUTTONMODE)==HIGH){delay(50);}
       if(mode>=2){
           mode=0;
           motor1.setVelocity(0);
           motor2.setVelocity(0);
       }
   }

   if(digitalRead(BUTTONSELECT) == HIGH){
       if(mode == SERVO){
           selectedServo++;
       }else if(mode == DCMOTOR){
           selectedDCMotor++;
       }
       while(digitalRead(BUTTONSELECT)==HIGH){delay(50);}
       if(selectedServo>=3){selectedServo=0;}
       if(selectedDCMotor>=2){selectedDCMotor=0;}
   }

   int val = analogRead(POT);

   if(mode == SERVO){
       val = map(val, 0, 1023, 0, 180);
       servos[selectedServo]->write(val);
   }else if(mode == DCMOTOR){
       val = map(val, 0, 1023, -10, 10);
       if(selectedDCMotor==0){
           motor1.setVelocity(val);
       }else{
           motor2.setVelocity(val);
       }
   }else if(mode == AX12){

   }
 }