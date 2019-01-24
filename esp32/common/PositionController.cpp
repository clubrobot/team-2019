#include <Arduino.h>
#include <EEPROM.h>

#include "PositionController.h"
#include "SerialTalks.h"


void PositionController::setPosInput   (const Position& posInput)
{
	m_mutex.acquire();
	m_posInput    = posInput;
	m_mutex.release();
}

void PositionController::setPosSetpoint(const Position& posSetpoint)
{
	m_mutex.acquire();
	m_posSetpoint = posSetpoint;
	m_mutex.release();
}

void PositionController::setThetaSetpoint(float theta)
{
	m_mutex.acquire();
	m_posSetpoint.theta = theta;
	m_mutex.release();
}

float PositionController::getLinVelSetpoint() const
{
	m_mutex.acquire();
	float result = m_linVelSetpoint;
	m_mutex.release();
	return result;
}
float PositionController::getAngVelSetpoint() const
{
	m_mutex.acquire();
	float result = m_angVelSetpoint;
	m_mutex.release();
	return result;
}
void PositionController::setVelTunings(float linVelKp, float angVelKp)
{
	m_mutex.acquire();
	m_linVelKp  = linVelKp;
	m_angVelKp  = angVelKp;
	m_mutex.release();
}

void PositionController::setVelLimits(float linVelMax, float angVelMax)
{
	m_mutex.acquire();
	m_linVelMax = linVelMax;
	m_angVelMax = angVelMax;
	m_mutex.release();
}

void PositionController::setPosThresholds(float linPosThreshold, float angPosThreshold)
{
	m_mutex.acquire();
	m_linPosThreshold = linPosThreshold;
	m_angPosThreshold = angPosThreshold;
	m_mutex.release();
}

void PositionController::setMoveStrategy(AbstractMoveStrategy& moveStrategy)
{
	m_mutex.acquire();
	m_moveStrategy = &moveStrategy;
	m_moveStrategy->m_context = this;
	m_mutex.release();
}

void PositionController::process(float timestep)
{
	m_mutex.acquire();
	if (m_moveStrategy != 0)
		m_moveStrategy->computeVelSetpoints(timestep);
	m_mutex.release();

}

void PositionController::onProcessEnabling()
{
	m_mutex.acquire();
	//m_posSetpoint = m_posInput;
	m_mutex.release();
}

bool PositionController::getPositionReached()
{
	if (m_moveStrategy != 0)
		return m_moveStrategy->getPositionReached();
	return false;
}

void PositionController::load(int address)
{
	m_mutex.acquire();
	EEPROM.get(address, m_linVelKp); address += sizeof(m_linVelKp);
	EEPROM.get(address, m_angVelKp); address += sizeof(m_angVelKp);
	EEPROM.get(address, m_linVelMax); address += sizeof(m_linVelMax);
	EEPROM.get(address, m_angVelMax); address += sizeof(m_angVelMax);
	EEPROM.get(address, m_linPosThreshold); address += sizeof(m_linPosThreshold);
	EEPROM.get(address, m_angPosThreshold); address += sizeof(m_angPosThreshold);
	m_mutex.release();
}

void PositionController::save(int address) const
{
	m_mutex.acquire();
	EEPROM.put(address, m_linVelKp); address += sizeof(m_linVelKp);
	EEPROM.put(address, m_angVelKp); address += sizeof(m_angVelKp);
	EEPROM.put(address, m_linVelMax); address += sizeof(m_linVelMax);
	EEPROM.put(address, m_angVelMax); address += sizeof(m_angVelMax);
	EEPROM.put(address, m_linPosThreshold); address += sizeof(m_linPosThreshold);
	EEPROM.put(address, m_angPosThreshold); address += sizeof(m_angPosThreshold);
	EEPROM.commit();
	m_mutex.release();
}


float PositionController::getLinVelKp() const
{
	m_mutex.acquire();
	float result =  m_linVelKp;
	m_mutex.release();
	return result;
}

float PositionController::getAngVelKp() const
{
	m_mutex.acquire();
	float result = m_angVelKp;
	m_mutex.release();
	return result;
}
float PositionController::getLinVelMax() const
{
	m_mutex.acquire();
	float result = m_linVelMax;
	m_mutex.release();
	return result;
}
float PositionController::getAngVelMax() const
{
	m_mutex.acquire();
	float result = m_angVelMax;
	m_mutex.release();
	return result;
}
float PositionController::getLinPosThreshold() const
{
	m_mutex.acquire();
	float result = m_linPosThreshold;
	m_mutex.release();
	return result;
}
float PositionController::getAngPosThreshold() const
{
	m_mutex.acquire();
	float result = m_angPosThreshold;
	m_mutex.release();
	return result;
}

