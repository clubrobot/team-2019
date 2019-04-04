#include <Arduino.h>
#include <EEPROM.h>

#include "TurnOnTheSpot.h"
#include "SerialTalks.h"
#include "mathutils.h"


void TurnOnTheSpot::computeVelSetpoints(float timestep)
{
	const float dx = getPosSetpoint().x - getPosInput().x;
	const float dy = getPosSetpoint().y - getPosInput().y;
	const float theta = getPosInput().theta;
	const float linVelKp  = getLinVelKp();
	const float linVelMax = getLinVelMax();
	const float angVelKp  = getAngVelKp();
	const float angVelMax = getAngVelMax();

	const float linPosSetpoint = cos(theta) * dx + sin(theta) * dy;
	const float angPosSetpoint = inrange(getPosSetpoint().theta - theta, -M_PI, M_PI);

	const float linVelSetpoint = saturate(linVelKp * linPosSetpoint, -linVelMax, linVelMax);
	const float angVelSetpoint = saturate(angVelKp * angPosSetpoint, -angVelMax, angVelMax);
	setVelSetpoints(linVelSetpoint, angVelSetpoint);
}

bool TurnOnTheSpot::getPositionReached()
{
	const float theta = getPosInput().theta;
	const float angPosSetpoint = inrange(getPosSetpoint().theta - theta, -M_PI, M_PI);
	return fabs(angPosSetpoint) < getAngPosThreshold();
}
