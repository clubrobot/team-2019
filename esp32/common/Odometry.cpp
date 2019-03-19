#include <Arduino.h>
#include <EEPROM.h>

#include "Odometry.h"
#include "SerialTalks.h"

#include <math.h>


void Odometry::process(float timestep)
{
	m_mutex.acquire();
	const float dL = m_leftCodewheel ->restart();
	const float dR = m_rightCodewheel->restart();

	const float deltaLinPos = (dL + dR) / 2;
	const float deltaOrthLinPos = fabs(deltaLinPos) * m_slippage;
	const float deltaAngPos = (dR - dL) / m_axleTrack;

	const float avgTheta = m_pos.theta + deltaAngPos / 2;
	m_pos.x     += deltaLinPos * cos(avgTheta) - deltaOrthLinPos * sin(avgTheta);
	m_pos.y     += deltaLinPos * sin(avgTheta) + deltaOrthLinPos * cos(avgTheta);
	m_pos.theta += deltaAngPos;

	m_linVel = deltaLinPos / timestep;
	m_angVel = deltaAngPos / timestep;
	m_mutex.release();
}

void Odometry::load(int address)
{
	EEPROM.get(address, m_axleTrack); address += sizeof(m_axleTrack);
	EEPROM.get(address, m_slippage);  address += sizeof(m_slippage);
}

void Odometry::save(int address) const
{
	EEPROM.put(address, m_axleTrack); address += sizeof(m_axleTrack);
	EEPROM.put(address, m_slippage);  address += sizeof(m_slippage);
	EEPROM.commit();
}


void Odometry::setPosition(float x, float y, float theta)
{
	m_mutex.acquire();
	m_pos.x = x;
	m_pos.y = y;
	m_pos.theta = theta;
	m_mutex.release();
}

void Odometry::setAxleTrack(float axleTrack)
{
	m_mutex.acquire();
	m_axleTrack = axleTrack;
	m_mutex.release();
}
void Odometry::setSlippage (float slippage)
{
	m_mutex.acquire();
	m_slippage  = slippage;
	m_mutex.release();
}

const Position Odometry::getPosition() const
{
	m_mutex.acquire();
	Position result = m_pos;
	m_mutex.release();
	return result;
}

float Odometry::getLinVel() const
{
	m_mutex.acquire();
	float result = m_linVel;
	m_mutex.release();
	return result;
}

float Odometry::getAngVel() const
{
	m_mutex.acquire();
	float result = m_angVel;
	m_mutex.release();
	return result;
}

float Odometry::getAxleTrack() const
{
	m_mutex.acquire();
	float result = m_axleTrack;
	m_mutex.release();
	return result;
} 
float Odometry::getSlippage () const
{
	m_mutex.acquire();
	float result = m_slippage;
	m_mutex.release();
	return result;
}