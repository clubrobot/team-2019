#ifndef __ODOMETRY_H__
#define __ODOMETRY_H__

#include "PeriodicProcess.h"
#include "thread_tools.h"

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

	void setPosition(float x, float y, float theta);

	void setAxleTrack(float axleTrack);
	void setSlippage (float slippage);
	
	void setCodewheels(AbstractCodewheel& leftCodewheel, AbstractCodewheel& rightCodewheel){m_leftCodewheel = &leftCodewheel, m_rightCodewheel = &rightCodewheel;}

	const Position	getPosition() const;

	float getLinVel() const;
	float getAngVel() const;

	float getAxleTrack() const; 
	float getSlippage () const;
	
	void load(int address);
	void save(int address) const;

protected:

	virtual void process(float timestep);

	Position m_pos;
	float m_linVel;
	float m_angVel;
	float m_axleTrack;
	float m_slippage;
	Mutex m_mutex;

	AbstractCodewheel* m_leftCodewheel;
	AbstractCodewheel* m_rightCodewheel;
};

#endif // __ODOMETRY_H__
