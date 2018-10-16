#include <Arduino.h>
#include <Servo.h>
#include "../common/Clock.h"
#include "../common/SoftwareSerial.h"
#include "../common/SerialTalks.h"

#include "../common/ShiftRegister.h"
#include "../common/ShiftRegAX12.h"

#include "../common/StepByStepMotor.h"
#include "../common/ShiftRegDCMotor.h"

#include "../common/RobotArm.h"
#include "PIN.h"
#include "instructions.h"

#define USE_SHIFTREG 1
#define BEE_CLOSED 40

ShiftRegDCMotorsDriver driver;
ShiftRegDCMotor cuber_manager;

SoftwareSerial SoftSerial(RX_AX12,TX_AX12);

ShiftRegister shift;

StepByStepMotor motor;

ShiftRegAX12 servoax;

//            X |  Y  |  Z  | Th | SPEED
RobotArm arm(-10.0, 10.0, 10.0, 180.0, 500);

Servo beeActivator;

bool open_manager  = false;
bool close_manager = false;

Clock clock;

void setup()
{
	// //Starting SerialTalks
	Serial.begin(SERIALTALKS_BAUDRATE);
    talks.begin(Serial);

    /*****************bind set pos FUNC*****************/
    talks.bind(BEGIN_OPCODE,BEGIN);

    talks.bind(SET_POS_OPCODE,SET_POSITION);

    talks.bind(SET_X_OPCODE,SET_X);
    talks.bind(SET_Y_OPCODE,SET_Y);
    talks.bind(SET_Z_OPCODE,SET_Z);
    talks.bind(SET_THETA_OPCODE,SET_THETA);
    talks.bind(SET_SPEED_OPCODE,SET_SPEED);

    talks.bind(GET_POS_OPCODE,GET_POSITION);
    talks.bind(GET_POS_THEO_OPCODE,GET_POSITION_THEO);

    talks.bind(OPEN_GRIPPER_OPCODE,OPEN_GRIPPER);
    talks.bind(CLOSE_GRIPPER_OPCODE,CLOSE_GRIPPER);

    talks.bind(WRITE_BEEACTIVATOR_OPCODE,WRITE_BEEACTIVATOR);

    talks.bind(OPEN_CUBE_MANAGER_OPCODE,OPEN_CUBE_MANAGER);
    talks.bind(CLOSE_CUBE_MANAGER_OPCODE,CLOSE_CUBE_MANAGER);

    talks.bind(GET_EMERGENCY_STATE_OPCODE, GET_EMERGENCY_STATE);
    /***************************************************/

    //initialise ShiftRegister
    shift.attach(LATCHPIN,CLOCKPIN,DATAPIN);

    motor.attach(STEP_PAP, DIR_PAP, ENABLE_PAP, RST_PAP, SLEEP_PAP);

    ShiftRegAX12::SerialBegin(9600, RX_AX12, TX_AX12, AX12_DATA_CONTROL);

    pinMode(SERVO_BEE, OUTPUT);

    beeActivator.attach(SERVO_BEE);
    beeActivator.write(BEE_CLOSED);

    driver.attach(RST_DRV, 999);
    driver.reset();

    cuber_manager.attach(EN_MOTOR_1, PWM_MOTOR_1, SELECT_MOTOR1);
    cuber_manager.setConstant(1/11.1);

    pinMode(INTER2, INPUT_PULLUP);

}

void loop()
{
	talks.execute();
    //motor.update();

    if(open_manager)
    {
        if(clock.getElapsedTime() < 2)
        {
            cuber_manager.setVelocity(-7);
        }
        else
        {
            cuber_manager.setVelocity(0.1);
        }
    }

    if(close_manager)
    {
        if(clock.getElapsedTime() < 2)
        {
            cuber_manager.setVelocity(7);
        }
        else
        {
            cuber_manager.setVelocity(3);
        }
    }

}