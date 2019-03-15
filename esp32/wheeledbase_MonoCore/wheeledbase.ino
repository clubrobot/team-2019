#include <Arduino.h>

#include "PIN.h"
#include "constants.h"
#include "instructions.h"
#include "addresses.h"

#include "../MonoCoreUtils/SerialTalks.h"
#include "../MonoCoreUtils/DCMotor.h"
#include "../MonoCoreUtils/Codewheel.h"
#include "../MonoCoreUtils/Odometry.h"
#include "../MonoCoreUtils/PID.h"
#include "../MonoCoreUtils/VelocityController.h"
#include "../MonoCoreUtils/PositionController.h"
#include "../MonoCoreUtils/PurePursuit.h"
#include "../MonoCoreUtils/TurnOnTheSpot.h"
#include "../MonoCoreUtils/mathutils.h"

// Load the different modules

DCMotorsDriver driver;
DCMotor leftWheel;
DCMotor rightWheel;

Codewheel leftCodewheel;
Codewheel rightCodewheel;

Odometry odometry;

VelocityController velocityControl;
#if ENABLE_VELOCITYCONTROLLER_LOGS
VelocityControllerLogs controllerLogs;
#endif // ENABLE_VELOCITYCONTROLLER_LOGS

PID linVelPID;
PID angVelPID;

PositionController positionControl;

PurePursuit   purePursuit;
TurnOnTheSpot turnOnTheSpot;

// Setup

void setup()
{
	// Communication
	Serial.begin(SERIALTALKS_BAUDRATE);
	talks.begin(Serial);
	
	talks.bind(SET_OPENLOOP_VELOCITIES_OPCODE, SET_OPENLOOP_VELOCITIES);
	talks.bind(GET_CODEWHEELS_COUNTERS_OPCODE, GET_CODEWHEELS_COUNTERS);
	talks.bind(SET_VELOCITIES_OPCODE, SET_VELOCITIES);
	talks.bind(RESET_PUREPURSUIT_OPCODE, RESET_PUREPURSUIT);
	talks.bind(ADD_PUREPURSUIT_WAYPOINT_OPCODE, ADD_PUREPURSUIT_WAYPOINT);
	talks.bind(START_PUREPURSUIT_OPCODE, START_PUREPURSUIT);
	talks.bind(START_TURNONTHESPOT_OPCODE, START_TURNONTHESPOT);
	talks.bind(POSITION_REACHED_OPCODE, POSITION_REACHED);
	talks.bind(SET_POSITION_OPCODE, SET_POSITION);
	talks.bind(GET_POSITION_OPCODE, GET_POSITION);
	talks.bind(GET_VELOCITIES_OPCODE, GET_VELOCITIES);
	talks.bind(SET_PARAMETER_VALUE_OPCODE, SET_PARAMETER_VALUE);
	talks.bind(GET_PARAMETER_VALUE_OPCODE, GET_PARAMETER_VALUE);
	
	// DC motors wheels
	driver.attach(DRIVER_RESET, DRIVER_FAULT);
	driver.reset();
	
	leftWheel .attach(LEFT_MOTOR_EN,  LEFT_MOTOR_PWM,LEFT_MOTOR_CHANNEL, PWM_FREQUENCY, LEFT_MOTOR_DIR);
	
	rightWheel.attach(RIGHT_MOTOR_EN, RIGHT_MOTOR_PWM,RIGHT_MOTOR_CHANNEL, PWM_FREQUENCY,RIGHT_MOTOR_DIR);
	
	leftWheel .load(LEFTWHEEL_ADDRESS);
	
	rightWheel.load(RIGHTWHEEL_ADDRESS);
	
	// Codewheels
	leftCodewheel .attachCounter(QUAD_COUNTER_XY, QUAD_COUNTER_Y_AXIS, QUAD_COUNTER_SEL1, QUAD_COUNTER_SEL2, QUAD_COUNTER_OE, QUAD_COUNTER_RST_Y);
	rightCodewheel.attachCounter(QUAD_COUNTER_XY, QUAD_COUNTER_X_AXIS, QUAD_COUNTER_SEL1, QUAD_COUNTER_SEL2, QUAD_COUNTER_OE, QUAD_COUNTER_RST_X);
	leftCodewheel .attachRegister(SHIFT_REG_DATA, SHIFT_REG_LATCH, SHIFT_REG_CLOCK);
	rightCodewheel.attachRegister(SHIFT_REG_DATA, SHIFT_REG_LATCH, SHIFT_REG_CLOCK);
	leftCodewheel .load(LEFTCODEWHEEL_ADDRESS);
	rightCodewheel.load(RIGHTCODEWHEEL_ADDRESS);
	leftCodewheel .reset();
	rightCodewheel.reset();

	// Odometry
	odometry.load(ODOMETRY_ADDRESS);
	odometry.setCodewheels(leftCodewheel, rightCodewheel);
	odometry.setTimestep(ODOMETRY_TIMESTEP);
	odometry.enable();
	
	// Engineering control
	velocityControl.load(VELOCITYCONTROL_ADDRESS);
	velocityControl.setWheels(leftWheel, rightWheel);
	velocityControl.setPID(linVelPID, angVelPID);
	velocityControl.disable();

	linVelPID.load(LINVELPID_ADDRESS);
	angVelPID.load(ANGVELPID_ADDRESS);

#if ENABLE_VELOCITYCONTROLLER_LOGS
	controllerLogs.setController(velocityControl);
	controllerLogs.setTimestep(VELOCITYCONTROLLER_LOGS_TIMESTEP);
	controllerLogs.enable();
#endif // VELOCITYENABLE_CONTROLLER_LOGS

	// Position control
	positionControl.load(POSITIONCONTROL_ADDRESS);
	positionControl.setTimestep(POSITIONCONTROL_TIMESTEP);
	positionControl.disable();

	purePursuit.load(PUREPURSUIT_ADDRESS);
	
	// Miscellanous
	//TCCR2B = (TCCR2B & 0b11111000) | 1; // Set Timer2 frequency to 16MHz instead of 250kHz
	
}

// Loop

void loop()
{	
	talks.execute();
	
	// Update odometry
	if (odometry.update())
	{
		positionControl.setPosInput(odometry.getPosition());
		velocityControl.setInputs(odometry.getLinVel(), odometry.getAngVel());
	}

	// Compute trajectory
	if (positionControl.update())
	{
		float linVelSetpoint = positionControl.getLinVelSetpoint();
		float angVelSetpoint = positionControl.getAngVelSetpoint();
		velocityControl.setSetpoints(linVelSetpoint, angVelSetpoint);
	}

	// Integrate engineering control
#if ENABLE_VELOCITYCONTROLLER_LOGS
	if (velocityControl.update())
		controllerLogs.update();
#else
	velocityControl.update();
#endif // ENABLE_VELOCITYCONTROLLER_LOGS //


}
