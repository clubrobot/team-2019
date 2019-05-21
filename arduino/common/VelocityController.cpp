#include <Arduino.h>
#include <EEPROM.h>

#include "VelocityController.h"
#include "SerialTalks.h"
#include "mathutils.h"


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
}

void VelocityController::onProcessEnabling()
{
	DifferentialController::onProcessEnabling();
	m_rampLinVelSetpoint = 0;
	m_rampAngVelSetpoint = 0;
}

void VelocityController::load(int address)
{
	EEPROM.get(address, m_axleTrack);    address += sizeof(m_axleTrack);
	EEPROM.get(address, m_maxLinAcc);    address += sizeof(m_maxLinAcc);
	EEPROM.get(address, m_maxLinDec);    address += sizeof(m_maxLinDec);
	EEPROM.get(address, m_maxAngAcc);    address += sizeof(m_maxAngAcc);
	EEPROM.get(address, m_maxAngDec);    address += sizeof(m_maxAngDec);
	EEPROM.get(address, m_spinShutdown); address += sizeof(m_spinShutdown);
}

void VelocityController::save(int address) const
{
	EEPROM.put(address, m_axleTrack);
	address += sizeof(m_axleTrack);
	EEPROM.put(address, m_maxLinAcc);
	address += sizeof(m_maxLinAcc);
	EEPROM.put(address, m_maxLinDec);
	address += sizeof(m_maxLinDec);
	EEPROM.put(address, m_maxAngAcc);
	address += sizeof(m_maxAngAcc);
	EEPROM.put(address, m_maxAngDec);
	address += sizeof(m_maxAngDec);
	EEPROM.put(address, m_spinShutdown);
	address += sizeof(m_spinShutdown);
}



void VelocityController::setMaxLinAcc   (float maxLinAcc)   {
	m_maxLinAcc = maxLinAcc;
	update();
}

void VelocityController::setMaxLinDec   (float maxLinDec)   {
	m_maxLinDec = maxLinDec;
	update();
}

void VelocityController::setMaxAngAcc   (float maxAngAcc)   {
	m_maxAngAcc = maxAngAcc;
	update();
}

void VelocityController::setMaxAngDec   (float maxAngDec)   {
	m_maxAngDec = maxAngDec;
	update();
}

void VelocityController::setSpinShutdown   (bool spinShutdown)   {
	m_spinShutdown = spinShutdown;
	update();
}


#if ENABLE_VELOCITYCONTROLLER_LOGS
void VelocityControllerLogs::process(float timestep)
{
	talks.out << millis() << "\t";
	talks.out << m_controller->m_rampLinVelSetpoint << "\t" << m_controller->m_linInput << "\t" << m_controller->m_linVelOutput << "\t";
	talks.out << m_controller->m_rampAngVelSetpoint << "\t" << m_controller->m_angInput << "\t" << m_controller->m_angVelOutput << "\n";
};
#endif // ENABLE_VELOCITYCONTROLLER_LOGS
