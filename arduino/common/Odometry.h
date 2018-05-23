#ifndef __ODOMETRY_H__
#define __ODOMETRY_H__

#include "PeriodicProcess.h"


struct Position
{
	Position() : x(0), y(0), theta(0){}
	Position(float x, float y, float theta) : x(x), y(y), theta(theta){}

	float x, y;
	float theta;
};

class AbstractCodewheel
{
public:

	virtual ~AbstractCodewheel(){}

	virtual float getTraveledDistance() = 0;
	virtual float restart() = 0;
};

class Odometry : public PeriodicProcess
{
public:

	void setPosition(float x, float y, float theta){m_pos.x = x; m_pos.y = y; m_pos.theta = theta;}

	void setAxleTrack(float axleTrack){m_axleTrack = axleTrack;}
	void setSlippage (float slippage) {m_slippage  = slippage;}
	
	void setCodewheels(AbstractCodewheel& leftCodewheel, AbstractCodewheel& rightCodewheel){m_leftCodewheel = &leftCodewheel, m_rightCodewheel = &rightCodewheel;}

	const Position&	getPosition() const {return m_pos;}

	float getLinVel() const {return m_linVel;}
	float getAngVel() const {return m_angVel;}

	float getAxleTrack() const {return m_axleTrack;}
	float getSlippage () const {return m_slippage;}
	
	void load(int address);
	void save(int address) const;

protected:

	virtual void process(float timestep);

	Position m_pos;
	float m_linVel;
	float m_angVel;
	float m_axleTrack;
	float m_slippage;

	AbstractCodewheel* m_leftCodewheel;
	AbstractCodewheel* m_rightCodewheel;
};

#endif // __ODOMETRY_H__
