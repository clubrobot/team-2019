#ifndef __TURNONTHESPOT_H__
#define __TURNONTHESPOT_H__

#include "PositionController.h"
#include "Odometry.h"

#include <math.h>

/** Class TurOnTheSpot
 *  
 *  \brief Rotation du robot sans translations.
 */
class TurnOnTheSpot : public AbstractMoveStrategy
{
public:
	TurnOnTheSpot() : m_direction(TRIG){}
	enum Direction {TRIG=1, CLOCK=-1};
	void setDirection(Direction direction);
protected:

	virtual void computeVelSetpoints(float timestep);
	virtual bool getPositionReached();

private:
	Direction m_direction;
};

#endif // __TURNONTHESPOT_H__
