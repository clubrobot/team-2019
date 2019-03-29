#include <Arduino.h>
#include <EEPROM.h>

#include "Odometry.h"
#include "SerialTalks.h"

#include <math.h>


void Odometry::process(float timestep)
{
	const float dL = m_leftCodewheel ->restart();
	const float dR = m_rightCodewheel->restart();

	const float deltaLinPos = (dL + dR) / 2;
	const float deltaOrthLinPos = abs(deltaLinPos) * m_slippage;
	const float deltaAngPos = (dR - dL) / m_axleTrack;

	const float avgTheta = m_pos.theta + deltaAngPos / 2;
	m_pos.x     += deltaLinPos * cos(avgTheta) - deltaOrthLinPos * sin(avgTheta);
	m_pos.y     += deltaLinPos * sin(avgTheta) + deltaOrthLinPos * cos(avgTheta);
	m_pos.theta += deltaAngPos;

	m_linVel = deltaLinPos / timestep;
	m_angVel = deltaAngPos / timestep;
}

void Odometry::load(int address)
{
	EEPROM.get(address, m_axleTrack); address += sizeof(m_axleTrack);
	EEPROM.get(address, m_slippage);  address += sizeof(m_slippage);
	m_valuesModified = true;
}

void Odometry::save(int address)
{
	if(m_valuesModified) {
		EEPROM.put(address, m_axleTrack); address += sizeof(m_axleTrack);
		EEPROM.put(address, m_slippage);  address += sizeof(m_slippage);
	}
	m_valuesModified = false;
}


void Odometry::setAxleTrack   (float axleTrack)   {
	if(m_axleTrack != axleTrack) {
		m_axleTrack = axleTrack;
		m_valuesModified = true;
		update();
	}
}

void Odometry::setSlippage   (float slippage)   {
	if(m_slippage != slippage) {
		m_slippage = slippage;
		m_valuesModified = true;
		update();
	}
}

