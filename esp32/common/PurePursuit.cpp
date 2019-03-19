#include <Arduino.h>
#include <EEPROM.h>

#include "PurePursuit.h"
#include "SerialTalks.h"
#include "mathutils.h"


void PurePursuit::setDirection(Direction direction)
{
	m_direction = direction;
}

void PurePursuit::setFinalAngle(float finalAngle)
{
	m_finalAngle = finalAngle;
}

bool PurePursuit::addWaypoint(const Waypoint& waypoint)
{
	if (m_numWaypoints < PUREPURSUIT_MAX_WAYPOINTS)
	{
		m_waypoints[m_numWaypoints++] = waypoint;
		return true;
	}
	return false;
}

void PurePursuit::reset()
{
	m_numWaypoints = 0;
	m_direction = FORWARD;
	m_goalIndex = 0;
	m_goalParam = 0;
	m_goalReached = false;
}

bool PurePursuit::checkLookAheadGoal(const float x, const float y)
{
	// The purpose of this function is to find the intersection point between a circle of radius
	// `m_lookAhead` centered at the robot position and the path. As there may be several of them,
	// we iterate through the path segments in the order of passing and stop as soon as we find
	// one.
	for (int i = m_goalIndex; i < m_numWaypoints; i++)
	{
		float dx = x - m_waypoints[i].x;
		float dy = y - m_waypoints[i].y;
		float edgedx, edgedy;
		if (i < m_numWaypoints-1)
		{
			edgedx = m_waypoints[i+1].x - m_waypoints[i].x;
			edgedy = m_waypoints[i+1].y - m_waypoints[i].y;	
		}
		else
		{
			edgedx = cos(m_finalAngle);
			edgedy = sin(m_finalAngle);
		}
		float edgeLength = sqrt(edgedx * edgedx + edgedy * edgedy);

		// `h` is the distance between the robot and the current line (i.e. the current segment but
		// without regard to its endpoints).
		float h = fabs(edgedx * dy - edgedy * dx) / edgeLength;
		if (m_lookAhead < h) // There is no intersection between the current line and the circle.
			continue;
		
		// `t` is the relative position of the robot projection in the current segment.
		// `t = 0` means that the robot projection is on its first endpoint and `t = 1` means
		// that the robot projection is on its second endpoint.
		float t = (edgedx * dx + edgedy * dy) / (edgeLength * edgeLength);

		// There is two intersection points between the circle and the line but we only consider
		// the one ahead of the robot: if it is beyond the second endpoint (so it's not on the
		// segment), then we go to the next.
		float t1 = t - sqrt(m_lookAhead * m_lookAhead - h * h) / edgeLength;
		float t2 = t + sqrt(m_lookAhead * m_lookAhead - h * h) / edgeLength;
		
		// Skip if the intersection point is beyond the second endpoint (see above).
		if (t2 < 0)
			continue;
		else if (t2 > 1 && i < m_numWaypoints-1)
			continue;
		else if (t1 > 1 && i == m_numWaypoints-1)
			continue;
		else if (t2 > 1 + m_lookAheadBis / edgeLength)
			t2 = 1 + m_lookAheadBis / edgeLength;

		// Save the new goal.
		if (i > m_goalIndex || t2 > m_goalParam)
		{
			m_goalIndex = i;
			m_goalParam = t2;
		}
		return true;
	}
	return false;
}

void PurePursuit::checkProjectionGoal(const float x, const float y)
{
	// The purpose of this function is to find the closest point between the path and the robot. To
	// do so we iterate through all the segments, cmpute their distance to the robot and sort them.
	float hmin = INFINITY;
	for (int i = m_goalIndex; i < m_numWaypoints-1; i++)
	{
		float dx = x - m_waypoints[i].x;
		float dy = y - m_waypoints[i].y;
		float edgedx = m_waypoints[i+1].x - m_waypoints[i].x;
		float edgedy = m_waypoints[i+1].y - m_waypoints[i].y;
		float edgeLength = sqrt(edgedx * edgedx + edgedy * edgedy);

		// `t` and `h` have the same meaning than in the `checkLookAheadGoal` method.
		float h, t = (edgedx * dx + edgedy * dy) / (edgeLength * edgeLength);
		if (t > 1 && i+1 < m_numWaypoints-1)
			continue;
		
		if (t > 1) // The closest point of the segment is its second endpoint.
		{
			float dx2 = x - m_waypoints[i+1].x;
			float dy2 = y - m_waypoints[i+1].y;
			h = sqrt(dx2 * dx2 + dy2 * dy2);
			t = 1;
		}
		else if (t <= 0) // The closest point of the segment is its first endpoint.
		{
			h = sqrt(dx * dx + dy * dy);
			t = 0;
		}
		else
		{
			h = fabs(edgedx * dy - edgedy * dx) / edgeLength;
		}

		// Save the new projection point if it is closer than the current one.
		if (h < hmin)
		{
			hmin = h;
			if (i > m_goalIndex || t > m_goalParam)
			{
				m_goalIndex = i;
				m_goalParam = t;
			}
		}
	}
}

float PurePursuit::getDistAfterGoal()
{
	// This function computes the remaining distance between the current goal and the last
	// waypoint. It basically does the sum of all the last segments lengths.
	float dist = 0;
	for (int i = m_goalIndex; i < m_numWaypoints; i++)
	{
		float edgedx, edgedy;
		if (i < m_numWaypoints - 1)
		{
			edgedx = m_waypoints[i+1].x - m_waypoints[i].x;
			edgedy = m_waypoints[i+1].y - m_waypoints[i].y;
		}
		else
		{
			edgedx = cos(m_finalAngle);
			edgedy = sin(m_finalAngle);
		}
		float edgeLength = sqrt(edgedx * edgedx + edgedy * edgedy);
		if (i == m_goalIndex)
			dist += (1 - m_goalParam) * edgeLength;
		else
			dist += edgeLength;
	}
	return dist;
}

void PurePursuit::computeVelSetpoints(float timestep)
{
	// Aliases
	const float x = getPosInput().x;
	const float y = getPosInput().y;
	const float theta = getPosInput().theta;
	const float linVelKp = getLinVelKp();
	const float angVelKp = getLinVelKp();
	const float linVelMax = getLinVelMax();
	const float angVelMax = getAngVelMax();

	// Compute the goal position on the path: `m_goalIndex` is the index of the segment it is
	// located on and `m_goalParam` is its relative position on that segment.
	if (!checkLookAheadGoal(x, y))
		checkProjectionGoal(x, y);

	// Compute the goal cartesian position.
	int i = m_goalIndex;
	float t = m_goalParam;
	Waypoint goal;
	if (i < m_numWaypoints - 1)
	{
		goal.x = (1-t) * m_waypoints[i].x + t * m_waypoints[i+1].x;
		goal.y = (1-t) * m_waypoints[i].y + t * m_waypoints[i+1].y;
	}
	else
	{
		goal.x = m_waypoints[i].x + t * cos(m_finalAngle);
		goal.y = m_waypoints[i].y + t * sin(m_finalAngle);
	}

	// Compute the norm and the argument of the vector going from the robot to its goal.
	float chord = sqrt((goal.x - x) * (goal.x - x) + (goal.y - y) * (goal.y - y));
	float delta = atan2(goal.y - y, goal.x - x) - theta + M_PI / 2 * (1 - m_direction);

	// The minimum curvature that the robot must follow is `2 * sin(delta) / chord`. So we deduce
	// from this and the maximum linear velocity allowed the maximum angular velocity setpoint. If
	// it exceeds the maximum angular velocity allowed, then we do the opposite: we use the maximum
	// angular velocity allowed to compute the maximum linear velocity setpoint.
	float newLinVelMax = linVelMax;
	float newAngVelMax = angVelMax;
	if (newAngVelMax * chord >= newLinVelMax * fabs(2 * sin(delta)))
		newAngVelMax = newLinVelMax * fabs(2 * sin(delta)) / chord;
	else
		newLinVelMax = newAngVelMax * chord / fabs(2 * sin(delta));
	
	// Then we do a simple proportional control for both linear and angular velocities.
	float linPosSetpoint = (chord + getDistAfterGoal()) * m_direction;
	float linVelSetpoint = saturate(linVelKp * linPosSetpoint, -newLinVelMax, newLinVelMax);

	float angPosSetpoint = inrange(delta, -M_PI, M_PI);
	float angVelSetpoint = saturate(angVelKp * angPosSetpoint, -newAngVelMax, newAngVelMax);

	// When traveling on the path, we slow down the robot if its orientation is farther than it
	// should. The following is an empiric but continuous formula.
	if (cos(delta) > 0)
		linVelSetpoint *= (1 + cos(2 * delta)) / 2;
	else
		linVelSetpoint *= 0;
	
	// This could be computed elsewhere but here is convenient. 
	m_goalReached = fabs(chord + getDistAfterGoal()) < getLinPosThreshold();

	setVelSetpoints(linVelSetpoint, angVelSetpoint);
}

bool PurePursuit::getPositionReached()
{
	return m_goalReached;
}

void PurePursuit::load(int address)
{
	EEPROM.get(address, m_lookAhead);    address += sizeof(m_lookAhead);
	EEPROM.get(address, m_lookAheadBis); address += sizeof(m_lookAheadBis);
}

void PurePursuit::save(int address) const
{
	EEPROM.put(address, m_lookAhead);    address += sizeof(m_lookAhead);
	EEPROM.put(address, m_lookAheadBis); address += sizeof(m_lookAheadBis);
	EEPROM.commit();
}
