#include <Arduino.h>
#include <EEPROM.h>

#include "PID.h"
#include "SerialTalks.h"
#include "mathutils.h"


float PID::compute(float setpoint, float input, float timestep)
{
	// Compute the error between the current state and the setpoint
	float currentError = setpoint - input;

	// Compute the error integral
	m_errorIntegral += currentError * timestep;
	m_errorIntegral = saturate(m_errorIntegral, m_minOutput / m_Ki, m_maxOutput / m_Ki);

	// Compute the error derivative
	float errorDerivative = (currentError - m_previousError) / timestep;
	m_previousError = currentError;

	// Compute the PID controller's output
	float output = m_Kp * currentError + m_Ki * m_errorIntegral - m_Kd * errorDerivative;
	return saturate(output, m_minOutput, m_maxOutput);
}

void PID::reset()
{
	m_errorIntegral = 0;
	m_previousError = 0;
}

void PID::load(int address)
{
	EEPROM.get(address, m_Kp); address += sizeof(m_Kp);
	EEPROM.get(address, m_Ki); address += sizeof(m_Ki);
	EEPROM.get(address, m_Kd); address += sizeof(m_Kd);
	EEPROM.get(address, m_minOutput); address += sizeof(m_minOutput);
	EEPROM.get(address, m_maxOutput); address += sizeof(m_maxOutput);
}

void PID::save(int address) const
{
	EEPROM.put(address, m_Kp); address += sizeof(m_Kp);
	EEPROM.put(address, m_Ki); address += sizeof(m_Ki);
	EEPROM.put(address, m_Kd); address += sizeof(m_Kd);
	EEPROM.put(address, m_minOutput); address += sizeof(m_minOutput);
	EEPROM.put(address, m_maxOutput); address += sizeof(m_maxOutput);
}

void PID::setTunings(float Kp, float Ki, float Kd){
	m_Kp = Kp;
	m_Ki = Ki;
	m_Kd = Kd;
}

void PID::setOutputLimits(float minOutput, float maxOutput)
{
	m_minOutput = minOutput;
	m_maxOutput = maxOutput;
}