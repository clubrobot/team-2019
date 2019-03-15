#ifndef __TURNONTHESPOT_H__
#define __TURNONTHESPOT_H__

#include "PositionController.h"
#include "Odometry.h"

#include <math.h>


class TurnOnTheSpot : public AbstractMoveStrategy
{
protected:

	virtual void computeVelSetpoints(float timestep);
	virtual bool getPositionReached();
};

#endif // __TURNONTHESPOT_H__
