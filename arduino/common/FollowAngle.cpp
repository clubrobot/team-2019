#include "FollowAngle.h"
#include "mathutils.h"


void FollowAngle::setVelSetpoint(float vel) {
    this->vel = vel;
}

float FollowAngle::getVelSetpoint() const {
    return vel;
}

void FollowAngle::computeVelSetpoints(float timestep)
{
    const float dx = getPosSetpoint().x - getPosInput().x;
    const float dy = getPosSetpoint().y - getPosInput().y;
    const float theta = getPosInput().theta;
    const float dtheta = getPosSetpoint().theta - theta;
    const float linVelKp  = getLinVelKp();
    const float linVelMax = getLinVelMax();
    const float angVelKp  = getAngVelKp();
    const float angVelMax = getAngVelMax();

    const float angPosSetpoint = inrange(dtheta, -M_PI, M_PI);
    const float vel = saturate((1-abs(angPosSetpoint)), 0, 1) * getVelSetpoint();
    const float linVelSetpoint = saturate(vel, -linVelMax, linVelMax);
    const float angVelSetpoint = saturate(angVelKp * angPosSetpoint, -angVelMax, angVelMax);

    setVelSetpoints(linVelSetpoint, angVelSetpoint);
}

bool FollowAngle::getPositionReached() {
    return abs(inrange(getPosSetpoint().theta - getPosInput().theta, -M_PI, M_PI)) < getAngPosThreshold();
}