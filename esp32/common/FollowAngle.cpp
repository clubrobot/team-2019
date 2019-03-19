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
    float dtheta = getPosSetpoint().theta - theta;

    const float linVelKp  = getLinVelKp();
    const float linVelMax = getLinVelMax();
    const float angVelKp  = getAngVelKp();
    const float angVelMax = getAngVelMax();

    int dir = 1;

    if (fabs(dtheta) > M_PI)
    {
        dtheta = -dtheta;
        dir = -1;
    }

    const float angPosSetpoint = inrange(dtheta, -M_PI, M_PI);
    const float linVelSetpoint = saturate(dir * getVelSetpoint(), -linVelMax, linVelMax);
    const float angVelSetpoint = saturate(angVelKp * angPosSetpoint, -angVelMax, angVelMax);

    setVelSetpoints(linVelSetpoint, angVelSetpoint);
}

bool FollowAngle::getPositionReached() {
    return abs(inrange(getPosSetpoint().theta - getPosInput().theta, -M_PI, M_PI)) < getAngPosThreshold();
}