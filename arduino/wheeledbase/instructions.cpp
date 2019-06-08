#include "instructions.h"

#include "addresses.h"

#include "../common/SerialTalks.h"
#include "../common/DCMotor.h"
#include "../common/Codewheel.h"
#include "../common/Odometry.h"
#include "../common/PID.h"
#include "../common/VelocityController.h"
#include "../common/PositionController.h"
#include "../common/PurePursuit.h"
#include "../common/TurnOnTheSpot.h"
#include "../common/mathutils.h"
#include <math.h>

// Global variables

extern DCMotorsDriver driver;
extern DCMotor leftWheel;
extern DCMotor rightWheel;

extern Codewheel leftCodewheel;
extern Codewheel rightCodewheel;

extern Odometry odometry;

extern VelocityController velocityControl;

extern PID linVelPID;
extern PID angVelPID;

extern PositionController positionControl;

extern PurePursuit   purePursuit;
extern TurnOnTheSpot turnOnTheSpot;

// Instructions


void DISABLE(SerialTalks& talks, Deserializer& input, Serializer& output)
{
	velocityControl.disable();
	positionControl.disable();
	leftWheel .setVelocity(0);
	rightWheel.setVelocity(0);
}

void GOTO_DELTA(SerialTalks& talks, Deserializer& input, Serializer& output)
{
	purePursuit.reset();
	positionControl.disable();

	Position initial_pos =  odometry.getPosition();

	float dx = input.read<float>();
	float dy = input.read<float>();

	Position target_pos;
	target_pos.x = initial_pos.x + dx*cos(initial_pos.theta)    + dy*-1*sin(initial_pos.theta);
	target_pos.y = initial_pos.y + dx*sin(initial_pos.theta) + dy*cos(initial_pos.theta);
	
	target_pos.theta = atan2(target_pos.y-initial_pos.y,target_pos.x-initial_pos.x);
	int direction;
	
	initial_pos.theta = inrange(initial_pos.theta,-M_PI,M_PI);

	if (fabs(inrange(target_pos.theta - initial_pos.theta,-M_PI,M_PI))<(M_PI/2))
	{
		direction = PurePursuit::FORWARD;
	}else{
		direction = PurePursuit::BACKWARD;
	}
	
	purePursuit.setDirection((PurePursuit::Direction) direction);
	purePursuit.addWaypoint(PurePursuit::Waypoint(initial_pos.x, initial_pos.y));
	purePursuit.addWaypoint(PurePursuit::Waypoint(target_pos.x, target_pos.y));

	purePursuit.setFinalAngle(target_pos.theta);

	positionControl.setPosSetpoint(Position(target_pos.x, target_pos.y, target_pos.theta + direction * M_PI));
	
	// Enable PurePursuit controller
	velocityControl.enable();
	positionControl.setMoveStrategy(purePursuit);
	positionControl.enable();

}

void SET_OPENLOOP_VELOCITIES(SerialTalks& talks, Deserializer& input, Serializer& output)
{
	float leftWheelVel  = input.read<float>();
	float rightWheelVel = input.read<float>();

	velocityControl.disable();
	positionControl.disable();
	leftWheel .setVelocity(leftWheelVel);
	rightWheel.setVelocity(rightWheelVel);
}

void GET_CODEWHEELS_COUNTERS(SerialTalks& talks, Deserializer& input, Serializer& output)
{
	long leftCodewheelCounter  = leftCodewheel. getCounter();
	long rightCodewheelCounter = rightCodewheel.getCounter();

	output.write<long>(leftCodewheelCounter);
	output.write<long>(rightCodewheelCounter);
}

void SET_VELOCITIES(SerialTalks& talks, Deserializer& input, Serializer& output)
{
	float linVelSetpoint = input.read<float>();
	float angVelSetpoint = input.read<float>();
	positionControl.disable();
	velocityControl.enable();
	velocityControl.setSetpoints(linVelSetpoint, angVelSetpoint);
}

void RESET_PUREPURSUIT(SerialTalks& talks, Deserializer& input, Serializer& output)
{
	purePursuit.reset();
	positionControl.disable();
}

void START_PUREPURSUIT(SerialTalks& talks, Deserializer& input, Serializer& output)
{
	// Setup PurePursuit
	byte direction = input.read<byte>();
	switch (direction)
	{
	case 0: purePursuit.setDirection(PurePursuit::FORWARD); break;
	case 1: purePursuit.setDirection(PurePursuit::BACKWARD); break;
	}
	purePursuit.setFinalAngle(input.read<float>());

	// Compute final setpoint
	const PurePursuit::Waypoint wp0 = purePursuit.getWaypoint(purePursuit.getNumWaypoints() - 2);
	const PurePursuit::Waypoint wp1 = purePursuit.getWaypoint(purePursuit.getNumWaypoints() - 1);
	positionControl.setPosSetpoint(Position(wp1.x, wp1.y, atan2(wp1.y - wp0.y, wp1.x - wp0.x) + direction * M_PI));
	
	// Enable PurePursuit controller
	velocityControl.enable();
	positionControl.setMoveStrategy(purePursuit);
	positionControl.enable();
}

void ADD_PUREPURSUIT_WAYPOINT(SerialTalks& talks, Deserializer& input, Serializer& output)
{
	// Queue waypoint
	float x = input.read<float>();
	float y = input.read<float>();
	purePursuit.addWaypoint(PurePursuit::Waypoint(x, y));
}

void START_TURNONTHESPOT(SerialTalks& talks, Deserializer& input, Serializer& output)
{
	Position posSetpoint = odometry.getPosition();
	float initTheta = posSetpoint.theta;
	posSetpoint.theta = input.read<float>();
	float angPosSetpoint = inrange((posSetpoint.theta - initTheta), -M_PI, M_PI);
	velocityControl.enable();
	positionControl.setPosSetpoint(posSetpoint);
	if(input.read<byte>()){
		if(angPosSetpoint>0) turnOnTheSpot.setDirection(TurnOnTheSpot::CLOCK);
		else                 turnOnTheSpot.setDirection(TurnOnTheSpot::TRIG);
	}
	else{
		if(angPosSetpoint>0) turnOnTheSpot.setDirection(TurnOnTheSpot::TRIG);
		else                 turnOnTheSpot.setDirection(TurnOnTheSpot::CLOCK);
	}
	positionControl.setMoveStrategy(turnOnTheSpot);
	positionControl.enable();
}

void START_TURNONTHESPOT_DIR(SerialTalks& talks, Deserializer& input, Serializer& output)
{
	Position posSetpoint = odometry.getPosition();
	posSetpoint.theta = input.read<float>();
	velocityControl.enable();
	positionControl.setPosSetpoint(posSetpoint);
	if (input.read<byte>()){
		turnOnTheSpot.setDirection(TurnOnTheSpot::TRIG);
	}
	else{
		turnOnTheSpot.setDirection(TurnOnTheSpot::CLOCK);
	}
	positionControl.setMoveStrategy(turnOnTheSpot);
	positionControl.enable();
}


void POSITION_REACHED(SerialTalks& talks, Deserializer& input, Serializer& output)
{
	bool positionReached = positionControl.getPositionReached() && positionControl.isEnabled();
	bool spinUrgency = !velocityControl.isEnabled();
	output.write<byte>(positionReached);
	output.write<byte>(spinUrgency);
}

void GET_VELOCITIES_WANTED(SerialTalks& talks, Deserializer& input, Serializer& output)
{

	if(input.read<byte>())
	{
		output.write<float>(velocityControl.getLinOutput());
		output.write<float>(velocityControl.getAngOutput());
	}else
	{
		output.write<float>(velocityControl.getLinSpinGoal());
		output.write<float>(velocityControl.getAngSpinGoal());
	}
}


void SET_POSITION(SerialTalks& talks, Deserializer& input, Serializer& output)
{
	float x     = input.read<float>();
	float y     = input.read<float>();
	float theta = input.read<float>();

	odometry.setPosition(x, y, theta);
}

void GET_POSITION(SerialTalks& talks, Deserializer& input, Serializer& output)
{
	const Position& pos = odometry.getPosition();
	
	output.write<float>(pos.x);
	output.write<float>(pos.y);
	output.write<float>(pos.theta);
}

void GET_VELOCITIES(SerialTalks& talks, Deserializer& input, Serializer& output)
{
	const float linVel = odometry.getLinVel();
	const float angVel = odometry.getAngVel();
	
	output.write<float>(linVel);
	output.write<float>(angVel);
}

void SET_PARAMETER_VALUE(SerialTalks& talks, Deserializer& input, Serializer& output)
{
	byte  id = input.read<byte>();
	switch (id)
	{
	case LEFTWHEEL_RADIUS_ID:
		leftWheel.setWheelRadius(input.read<float>());
		break;
	case LEFTWHEEL_CONSTANT_ID:
		leftWheel.setConstant(input.read<float>());
		break;
	case LEFTWHEEL_MAXPWM_ID:
		leftWheel.setMaxPWM(input.read<float>());
		break;

	case RIGHTWHEEL_RADIUS_ID:
		rightWheel.setWheelRadius(input.read<float>());
		break;
	case RIGHTWHEEL_CONSTANT_ID:
		rightWheel.setConstant(input.read<float>());
		break;
	case RIGHTWHEEL_MAXPWM_ID:
		rightWheel.setMaxPWM(input.read<float>());
		break;

	case LEFTCODEWHEEL_RADIUS_ID:
		leftCodewheel.setWheelRadius(input.read<float>());
		break;
	case LEFTCODEWHEEL_COUNTSPERREV_ID:
		leftCodewheel.setCountsPerRev(input.read<long>());
		break;

	case RIGHTCODEWHEEL_RADIUS_ID:
		rightCodewheel.setWheelRadius(input.read<float>());
		break;
	case RIGHTCODEWHEEL_COUNTSPERREV_ID:
		rightCodewheel.setCountsPerRev(input.read<long>());
		break;
	
	case ODOMETRY_AXLETRACK_ID:
		odometry.setAxleTrack(input.read<float>());
		break;
	case ODOMETRY_SLIPPAGE_ID:
		odometry.setSlippage(input.read<float>());
		break;
	
	case VELOCITYCONTROL_AXLETRACK_ID:
		velocityControl.setAxleTrack(input.read<float>());
		break;
	case VELOCITYCONTROL_MAXLINACC_ID:
		velocityControl.setMaxLinAcc(input.read<float>());
		break;
	case VELOCITYCONTROL_MAXLINDEC_ID:
		velocityControl.setMaxLinDec(input.read<float>());
		break;
	case VELOCITYCONTROL_MAXANGACC_ID:
		velocityControl.setMaxAngAcc(input.read<float>());
		break;
	case VELOCITYCONTROL_MAXANGDEC_ID:
		velocityControl.setMaxAngDec(input.read<float>());
		break;
	case VELOCITYCONTROL_SPINSHUTDOWN_ID:
		velocityControl.setSpinShutdown(input.read<byte>());
		break;
	
	case LINVELPID_KP_ID:
		linVelPID.setTunings(input.read<float>(), linVelPID.getKi(), linVelPID.getKd());
		break;
	case LINVELPID_KI_ID:
		linVelPID.setTunings(linVelPID.getKp(), input.read<float>(), linVelPID.getKd());
		break;
	case LINVELPID_KD_ID:
		linVelPID.setTunings(linVelPID.getKp(), linVelPID.getKi(), input.read<float>());
		break;
	case LINVELPID_MINOUTPUT_ID:
		linVelPID.setOutputLimits(input.read<float>(), linVelPID.getMaxOutput());
		break;
	case LINVELPID_MAXOUTPUT_ID:
		linVelPID.setOutputLimits(linVelPID.getMinOutput(), input.read<float>());
		break;
	
	case ANGVELPID_KP_ID:
		angVelPID.setTunings(input.read<float>(), angVelPID.getKi(), angVelPID.getKd());
		break;
	case ANGVELPID_KI_ID:
		angVelPID.setTunings(angVelPID.getKp(), input.read<float>(), angVelPID.getKd());
		break;
	case ANGVELPID_KD_ID:
		angVelPID.setTunings(angVelPID.getKp(), angVelPID.getKi(), input.read<float>());
		break;
	case ANGVELPID_MINOUTPUT_ID:
		angVelPID.setOutputLimits(input.read<float>(), angVelPID.getMaxOutput());
		break;
	case ANGVELPID_MAXOUTPUT_ID:
		angVelPID.setOutputLimits(angVelPID.getMinOutput(), input.read<float>());
		break;
	
	case POSITIONCONTROL_LINVELKP_ID:
		positionControl.setVelTunings(input.read<float>(), positionControl.getAngVelKp());
		break;
	case POSITIONCONTROL_ANGVELKP_ID:
		positionControl.setVelTunings(positionControl.getLinVelKp(), input.read<float>());
		break;
	case POSITIONCONTROL_LINVELMAX_ID:
		positionControl.setVelLimits(input.read<float>(), positionControl.getAngVelMax());
		break;
	case POSITIONCONTROL_ANGVELMAX_ID:
		positionControl.setVelLimits(positionControl.getLinVelMax(), input.read<float>());
		break;
	case POSITIONCONTROL_LINPOSTHRESHOLD_ID:
		positionControl.setPosThresholds(input.read<float>(), positionControl.getAngPosThreshold());
		break;
	case POSITIONCONTROL_ANGPOSTHRESHOLD_ID:
		positionControl.setPosThresholds(positionControl.getLinPosThreshold(), input.read<float>());
		break;

	case PUREPURSUIT_LOOKAHED_ID:
		purePursuit.setLookAhead(input.read<float>());
		break;
	case PUREPURSUIT_LOOKAHEADBIS_ID:
		purePursuit.setLookAheadBis(input.read<float>());
		break;
	}
}

void SAVE_PARAMETERS(SerialTalks& talks, Deserializer& input, Serializer& output)
{
	leftWheel.save(LEFTWHEEL_ADDRESS);
	rightWheel.save(RIGHTWHEEL_ADDRESS);
	leftCodewheel.save(LEFTCODEWHEEL_ADDRESS);
	rightCodewheel.save(RIGHTCODEWHEEL_ADDRESS);
	odometry.save(ODOMETRY_ADDRESS);
	velocityControl.save(VELOCITYCONTROL_ADDRESS);
	linVelPID.save(LINVELPID_ADDRESS);
	angVelPID.save(ANGVELPID_ADDRESS);
	positionControl.save(POSITIONCONTROL_ADDRESS);
	purePursuit.save(PUREPURSUIT_ADDRESS);
}

void GET_PARAMETER_VALUE(SerialTalks& talks, Deserializer& input, Serializer& output)
{
	byte id = input.read<byte>();
	switch (id)
	{
	case LEFTWHEEL_RADIUS_ID:
		output.write<float>(leftWheel.getWheelRadius());
		break;
	case LEFTWHEEL_CONSTANT_ID:
		output.write<float>(leftWheel.getConstant());
		break;
	case LEFTWHEEL_MAXPWM_ID:
		output.write<float>(leftWheel.getMaxPWM());
		break;
	
	case RIGHTWHEEL_RADIUS_ID:
		output.write<float>(rightWheel.getWheelRadius());
		break;
	case RIGHTWHEEL_CONSTANT_ID:
		output.write<float>(rightWheel.getConstant());
		break;
	case RIGHTWHEEL_MAXPWM_ID:
		output.write<float>(rightWheel.getMaxPWM());
		break;

	case LEFTCODEWHEEL_RADIUS_ID:
		output.write<float>(leftCodewheel.getWheelRadius());
		break;
	case LEFTCODEWHEEL_COUNTSPERREV_ID:
		output.write<long>(leftCodewheel.getCountsPerRev());
		break;
	
	case RIGHTCODEWHEEL_RADIUS_ID:
		output.write<float>(rightCodewheel.getWheelRadius());
		break;
	case RIGHTCODEWHEEL_COUNTSPERREV_ID:
		output.write<long>(rightCodewheel.getCountsPerRev());
		break;
	
	case ODOMETRY_AXLETRACK_ID:
		output.write<float>(odometry.getAxleTrack());
		break;
	case ODOMETRY_SLIPPAGE_ID:
		output.write<float>(odometry.getSlippage());
		break;
	
	case VELOCITYCONTROL_AXLETRACK_ID:
		output.write<float>(velocityControl.getAxleTrack());
		break;
	case VELOCITYCONTROL_MAXLINACC_ID:
		output.write<float>(velocityControl.getMaxLinAcc());
		break;
	case VELOCITYCONTROL_MAXLINDEC_ID:
		output.write<float>(velocityControl.getMaxLinDec());
		break;
	case VELOCITYCONTROL_MAXANGACC_ID:
		output.write<float>(velocityControl.getMaxAngAcc());
		break;
	case VELOCITYCONTROL_MAXANGDEC_ID:
		output.write<float>(velocityControl.getMaxAngDec());
		break;
	case VELOCITYCONTROL_SPINSHUTDOWN_ID:
		output.write<byte>(velocityControl.getSpinShutdown());
		break;
	
	case LINVELPID_KP_ID:
		output.write<float>(linVelPID.getKp());
		break;
	case LINVELPID_KI_ID:
		output.write<float>(linVelPID.getKi());
		break;
	case LINVELPID_KD_ID:
		output.write<float>(linVelPID.getKd());
		break;
	case LINVELPID_MINOUTPUT_ID:
		output.write<float>(linVelPID.getMinOutput());
		break;
	case LINVELPID_MAXOUTPUT_ID:
		output.write<float>(linVelPID.getMaxOutput());
		break;
	
	case ANGVELPID_KP_ID:
		output.write<float>(angVelPID.getKp());
		break;
	case ANGVELPID_KI_ID:
		output.write<float>(angVelPID.getKi());
		break;
	case ANGVELPID_KD_ID:
		output.write<float>(angVelPID.getKd());
		break;
	case ANGVELPID_MINOUTPUT_ID:
		output.write<float>(angVelPID.getMinOutput());
		break;
	case ANGVELPID_MAXOUTPUT_ID:
		output.write<float>(angVelPID.getMaxOutput());
		break;

	case POSITIONCONTROL_LINVELKP_ID:
		output.write<float>(positionControl.getLinVelKp());
		break;
	case POSITIONCONTROL_ANGVELKP_ID:
		output.write<float>(positionControl.getAngVelKp());
		break;
	case POSITIONCONTROL_LINVELMAX_ID:
		output.write<float>(positionControl.getLinVelMax());
		break;
	case POSITIONCONTROL_ANGVELMAX_ID:
		output.write<float>(positionControl.getAngVelMax());
		break;
	case POSITIONCONTROL_LINPOSTHRESHOLD_ID:
		output.write<float>(positionControl.getLinPosThreshold());
		break;
	case POSITIONCONTROL_ANGPOSTHRESHOLD_ID:
		output.write<float>(positionControl.getAngPosThreshold());
		break;

	case PUREPURSUIT_LOOKAHED_ID:
		output.write<float>(purePursuit.getLookAhead());
		break;
	case PUREPURSUIT_LOOKAHEADBIS_ID:
		output.write<float>(purePursuit.getLookAheadBis());
		break;
	}
}

void RESET_PARAMETERS(SerialTalks& talks, Deserializer& input, Serializer& output)
{
	leftWheel.load(LEFTWHEEL_ADDRESS);
	rightWheel.load(RIGHTWHEEL_ADDRESS);
	leftCodewheel.load(LEFTCODEWHEEL_ADDRESS);
	rightCodewheel.load(RIGHTCODEWHEEL_ADDRESS);
	odometry.load(ODOMETRY_ADDRESS);
	velocityControl.load(VELOCITYCONTROL_ADDRESS);
	linVelPID.load(LINVELPID_ADDRESS);
	angVelPID.load(ANGVELPID_ADDRESS);
	positionControl.load(POSITIONCONTROL_ADDRESS);
	purePursuit.load(PUREPURSUIT_ADDRESS);
	purePursuit.load(PUREPURSUIT_ADDRESS);
}