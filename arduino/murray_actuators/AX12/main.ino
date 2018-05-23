#include <Arduino.h>
#include <Servo.h>
#include "../../common/SoftwareSerial.h"
#include "../../common/SerialTalks.h"
#include "../../common/ShiftRegister.h"
#include "../../common/ShiftRegAX12.h"
#include "../../common/StepByStepMotor.h"
#include "../../common/RobotArm.h"
#include "PIN.h"
#include "instructions.h"

#define USE_SHIFTREG 1

#define BEE_CLOSED 170


//#include "../../common/StepByStepMotor.h"
SoftwareSerial SoftSerial(RX_AX12,TX_AX12);

Servo beeActivator;

ShiftRegister shift;

StepByStepMotor motor;

ShiftRegAX12 servoax;

//            X |  Y  |  Z  | Th | SPEED
RobotArm arm(15.0, 15.0, 10.0, 0.0, 100);

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
		talks.bind(GET_EMERGENCY_STATE_OPCODE, GET_EMERGENCY_STATE);
		talks.bind(WRITE_BEEACTIVATOR_OPCODE, WRITE_BEEACTIVATOR);

    /***************************************************/

		pinMode(INTER2, INPUT_PULLUP);
		pinMode(SERVO1, OUTPUT);

		beeActivator.attach(SERVO1);
		beeActivator.write(BEE_CLOSED);

    //initialise ShiftRegister
    shift.attach(LATCHPIN,CLOCKPIN,DATAPIN);

    motor.attach(STEP_PAP, DIR_PAP, ENABLE_PAP, RST_PAP, SLEEP_PAP);
    
    ShiftRegAX12::SerialBegin(9600, RX_AX12, TX_AX12, AX12_DATA_CONTROL);


    //arm.set_angles(150.0, 150.0, 150.0);

    arm.attach(2,1,3,SERVO1);

    arm.begin();

}

void loop()
{
	talks.execute();
    motor.update();

}

