#include <Arduino.h>
#include <EEPROM.h>

#include "VelocityController.h"
#include "SerialTalks.h"
#include "mathutils.h"


void VelocityController::setMaxAngAcc   (float maxAngAcc)
{
	m_mutex.acquire();
	m_maxAngAcc = maxAngAcc;
	m_mutex.release();
}

void VelocityController::setMaxLinAcc   (float maxLinAcc)
{
	m_mutex.acquire();
	m_maxLinAcc = maxLinAcc;
	m_mutex.release();
}


void VelocityController::setMaxLinDec   (float maxLinDec)
{
	m_mutex.acquire();
	m_maxLinDec = maxLinDec;
	m_mutex.release();
}

void VelocityController::setMaxAngDec   (float maxAngDec)
{
	m_mutex.acquire();
	m_maxAngDec = maxAngDec;
	m_mutex.release();
}

void VelocityController::setSpinShutdown(bool spinShutdown)
{
	m_mutex.acquire();
	m_spinShutdown = spinShutdown;
	m_mutex.release();
}

float VelocityController::getMaxLinAcc() const
{
	m_mutex.acquire();
	float result = m_maxLinAcc;
	m_mutex.release();
	return result;
}
float VelocityController::getMaxAngAcc() const
{
	m_mutex.acquire();
	float result = m_maxAngAcc;
	m_mutex.release();
	return result;
}
float VelocityController::getMaxLinDec() const
{
	m_mutex.acquire();
	float result = m_maxLinDec;
	m_mutex.release();
	return result;
}
float VelocityController::getMaxAngDec() const
{
	m_mutex.acquire();
	float result = m_maxAngDec;
	m_mutex.release();
	return result;
}
float VelocityController::getLinSpinGoal() const
{
	m_mutex.acquire();
	float result = m_linSpinGoal;
	m_mutex.release();
	return result;
}
float VelocityController::getAngSpinGoal() const
{
	m_mutex.acquire();
	float result = m_angSpinGoal;
	m_mutex.release();
	return result;
}

bool VelocityController::getSpinShutdown() const
{
	m_mutex.acquire();
	bool result = m_angSpinGoal;
	m_mutex.release();
	return result;
}


float VelocityController::genRampSetpoint(float stepSetpoint, float input, float rampSetpoint, float maxAcc, float maxDec, float timestep)
{
	// If we are above the desired setpoint (i.e. the ramp), we no longer try to follow it.
	// Instead we generate a new ramp starting from our current position.
	if ((input - rampSetpoint) * (stepSetpoint - rampSetpoint) > 0)
		rampSetpoint = input;

	// Do we have to accelerate or deccelerate to reach the desired setpoint?
	if (input * (stepSetpoint - input) >= 0)
		rampSetpoint += sign(stepSetpoint - input) * maxAcc * timestep;
	else
		rampSetpoint += sign(stepSetpoint - input) * maxDec * timestep;

	// We clamp the ramp so that it never exceeds the real setpoint
	if ((stepSetpoint - input) * (stepSetpoint - rampSetpoint) < 0) // TODO: test if this condition runs well when rampSetpoint is set to INFINITY
		rampSetpoint = stepSetpoint;

	return rampSetpoint;
}

void VelocityController::process(float timestep)
{
	m_mutex.acquire();
	// Save setpoints
	const float stepLinVelSetpoint = m_linSetpoint;
	const float stepAngVelSetpoint = m_angSetpoint;

	// Compute new setpoints
	m_rampLinVelSetpoint = genRampSetpoint(m_linSetpoint, m_linInput, m_rampLinVelSetpoint, m_maxLinAcc, m_maxLinDec, timestep);
	m_rampAngVelSetpoint = genRampSetpoint(m_angSetpoint, m_angInput, m_rampAngVelSetpoint, m_maxAngAcc, m_maxAngDec, timestep);

	// Do the engineering control
	m_linSetpoint = m_rampLinVelSetpoint;
	m_angSetpoint = m_rampAngVelSetpoint;

	DifferentialController::process(timestep);
	
	// Check for wheels abnormal spin and stop the controller accordingly
	bool linVelSpin = (m_linVelOutput <= m_linPID->getMinOutput()) || (m_linVelOutput >= m_linPID->getMaxOutput());
	bool angVelSpin = (m_angVelOutput <= m_angPID->getMinOutput()) || (m_angVelOutput >= m_angPID->getMaxOutput());
	if (linVelSpin || angVelSpin)
	{
		bool abnormalSpin = (fabs(m_linInput) < 1) && (fabs(m_angInput) < 0.05);
		if (abnormalSpin && m_spinShutdown)
		{
			m_leftWheel ->setVelocity(0);
			m_rightWheel->setVelocity(0);
			m_linSpinGoal = m_linSetpoint;
			m_angSpinGoal = m_angSetpoint;
			disable();
		}
	}
		
	// Restore setpoints
	m_linSetpoint = stepLinVelSetpoint;
	m_angSetpoint = stepAngVelSetpoint;
	m_mutex.release();
}

void VelocityController::onProcessEnabling()
{
	DifferentialController::onProcessEnabling();
	m_rampLinVelSetpoint = 0;
	m_rampAngVelSetpoint = 0;
}

void VelocityController::load(int address)
{
	m_mutex.acquire();

	EEPROM.get(address, m_axleTrack);    address += sizeof(m_axleTrack);
	EEPROM.get(address, m_maxLinAcc);    address += sizeof(m_maxLinAcc);
	EEPROM.get(address, m_maxLinDec);    address += sizeof(m_maxLinDec);
	EEPROM.get(address, m_maxAngAcc);    address += sizeof(m_maxAngAcc);
	EEPROM.get(address, m_maxAngDec);    address += sizeof(m_maxAngDec);
	EEPROM.get(address, m_spinShutdown); address += sizeof(m_spinShutdown);

	m_mutex.release();
}

void VelocityController::save(int address) const
{
	m_mutex.acquire();

	EEPROM.put(address, m_axleTrack);    address += sizeof(m_axleTrack);
	EEPROM.put(address, m_maxLinAcc);    address += sizeof(m_maxLinAcc);
	EEPROM.put(address, m_maxLinDec);    address += sizeof(m_maxLinDec);
	EEPROM.put(address, m_maxAngAcc);    address += sizeof(m_maxAngAcc);
	EEPROM.put(address, m_maxAngDec);    address += sizeof(m_maxAngDec);
	EEPROM.put(address, m_spinShutdown); address += sizeof(m_spinShutdown);
	EEPROM.commit();

	m_mutex.release();
}


#if ENABLE_VELOCITYCONTROLLER_LOGS
void VelocityControllerLogs::process(float timestep)
{
	m_mutex.acquire();

	talks.out << millis() << "\t";
	talks.out << m_controller->m_rampLinVelSetpoint << "\t" << m_controller->m_linInput << "\t" << m_controller->m_linVelOutput << "\t";
	talks.out << m_controller->m_rampAngVelSetpoint << "\t" << m_controller->m_angInput << "\t" << m_controller->m_angVelOutput << "\n";
	
	m_mutex.release();
};
#endif // ENABLE_VELOCITYCONTROLLER_LOGS
