
#ifndef __FOLLOWANGLE_H__
#define __FOLLOWANGLE_H__

#include "PositionController.h"
#include "Odometry.h"

#include <math.h>

class FollowAngle : public AbstractMoveStrategy
{
private:
    float vel;
public:
    void setVelSetpoint(float vel);
    float getVelSetpoint()const;
protected:
    virtual void computeVelSetpoints(float timestep);
    virtual bool getPositionReached();
};

#endif // __FOLLOWANGLE_H__