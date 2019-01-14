#include "FollowAngle.h"
#include "mathutils.h"


void FollowAngle::setVelInput(float vel) {
    this->vel = vel;
}

float FollowAngle::getVelInput() const {
    return vel;
}

void FollowAngle::computeVelSetpoints(float timestep)
{
    const float dtheta = getPosSetpoint().theta - getPosInput().theta;
    const float linVelKp  = getLinVelKp();
    const float linVelMax = getLinVelMax();
    const float angVelKp  = getAngVelKp();
    const float angVelMax = getAngVelMax();

    const float angPosSetpoint = inrange(dtheta, -M_PI, M_PI);
    const float angVelSetpoint = saturate(angVelKp * angPosSetpoint, -angVelMax, angVelMax);
    const float linVelSetpoint = getVelInput();

    setVelSetpoints(linVelSetpoint, angPosSetpoint);
}

bool FollowAngle::getPositionReached() {
    return abs(inrange(getPosSetpoint().theta - getPosInput().theta, -M_PI, M_PI)) < getAngPosThreshold();
}