#include <Arduino.h>
#include <EEPROM.h>

#include "DifferentialController.h"
#include "SerialTalks.h"


void DifferentialController::process(float timestep)
{
	// Compute linear and angular velocities outputs
	m_linVelOutput = m_linPID->compute(m_linSetpoint, m_linInput, timestep);
	m_angVelOutput = m_angPID->compute(m_angSetpoint, m_angInput, timestep);

	// Convert linear and angular velocities into wheels' velocities
	m_leftWheel ->setVelocity(m_linVelOutput - m_angVelOutput * m_axleTrack / 2);
	m_rightWheel->setVelocity(m_linVelOutput + m_angVelOutput * m_axleTrack / 2);
}

void DifferentialController::onProcessEnabling()
{
	m_linPID->reset();
	m_angPID->reset();
}

void DifferentialController::load(int address)
{
	EEPROM.get(address, m_axleTrack); address += sizeof(m_axleTrack);
}

void DifferentialController::save(int address) const
{
	EEPROM.put(address, m_axleTrack); address += sizeof(m_axleTrack);
	EEPROM.commit();
}

void DifferentialController::setInputs(float linInput, float angInput)
{
	m_mutex.acquire();
	m_linInput = linInput;
	m_angInput = angInput;
	m_mutex.release();
}

void DifferentialController::setSetpoints(float linSetpoint, float angSetpoint)
{
	m_mutex.acquire();
	m_linSetpoint = linSetpoint;
	m_angSetpoint = angSetpoint;
	m_mutex.release();
}

void DifferentialController::setAxleTrack(float axleTrack)
{
	m_mutex.acquire();
	m_axleTrack = axleTrack;
	m_mutex.release();
}

float DifferentialController::getLinSetpoint() const
{
	m_mutex.acquire();
	float result = m_linSetpoint;
	m_mutex.release();
	return result;
} 
float DifferentialController::getAngSetpoint() const
{
	m_mutex.acquire();
	float result = m_angSetpoint;
	m_mutex.release();
	return result;
} 

float DifferentialController::getLinOutput() const
{
	m_mutex.acquire();
	float result = m_linVelOutput;
	m_mutex.release();
	return result;
} 
float DifferentialController::getAngOutput() const
{
	m_mutex.acquire();
	float result = m_angVelOutput;
	m_mutex.release();
	return result;
} 

float DifferentialController::getAxleTrack() const
{
	m_mutex.acquire();
	float result = m_axleTrack;
	m_mutex.release();
	return result;
}