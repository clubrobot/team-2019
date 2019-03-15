#ifndef __PUREPURSUIT_H__
#define __PUREPURSUIT_H__

#include "PositionController.h"
#include "Odometry.h"

#include <math.h>

#ifndef PUREPURSUIT_MAX_WAYPOINTS
#define PUREPURSUIT_MAX_WAYPOINTS 16
#endif


class PurePursuit : public AbstractMoveStrategy
{
public:

	struct Waypoint
	{
		Waypoint() : x(0), y(0){}
		Waypoint(float x, float y) : x(x), y(y){}
		Waypoint(const Position& pos) : x(pos.x), y(pos.y){}

		float x, y;
	};

	enum Direction {FORWARD=1, BACKWARD=-1};

	PurePursuit() : m_numWaypoints(0), m_direction(FORWARD){}

	void setDirection(Direction direction);
	void setFinalAngle(float finalAngle);
	bool addWaypoint(const Waypoint& waypoint);

	void reset();

	void setLookAhead   (float lookAhead)   {m_lookAhead    = lookAhead;}
	void setLookAheadBis(float lookAheadBis){m_lookAheadBis = lookAheadBis;}

	Direction getDirection() const {return m_direction;}
	float getFinalAngle() const {return m_finalAngle;}
	const Waypoint& getWaypoint(int index) const {return m_waypoints[index];}
	int getNumWaypoints() const {return m_numWaypoints;}

	float getLookAhead()    const {return m_lookAhead;}
	float getLookAheadBis() const {return m_lookAheadBis;}

	void load(int address);
	void save(int address) const;

protected:

	virtual void computeVelSetpoints(float timestep);
	virtual bool getPositionReached();

	bool checkLookAheadGoal(const float x, const float y);
	void checkProjectionGoal(const float x, const float y);
	float getDistAfterGoal();

	// Trajectory specifications
	Waypoint m_waypoints[PUREPURSUIT_MAX_WAYPOINTS];
	int m_numWaypoints;
	Direction m_direction;
	float m_finalAngle;

	// Computation variables
	int m_goalIndex;
	float m_goalParam;
	bool m_goalReached;

	// Path following tunings
	float m_lookAhead;
	float m_lookAheadBis;
};

#endif // __PUREPURSUIT_H__
