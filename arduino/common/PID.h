#ifndef __PID_H__
#define __PID_H__

#include <math.h>


class PID
{
public:

	PID() : m_Kp(1), m_Ki(0), m_Kd(0), m_minOutput(-INFINITY), m_maxOutput(INFINITY){}

	float compute(float setpoint, float input, float timestep);
	
	void reset();

	void setTunings(float Kp, float Ki, float Kd){m_Kp = Kp, m_Ki = Ki, m_Kd = Kd;}
	void setOutputLimits(float minOutput, float maxOutput){m_minOutput = minOutput; m_maxOutput = maxOutput;}

	float getKp() const {return m_Kp;}
	float getKi() const {return m_Ki;}
	float getKd() const {return m_Kd;}
	float getMinOutput() const {return m_minOutput;}
	float getMaxOutput() const {return m_maxOutput;}

	void load(int address);
	void save(int address) const;

private:

	float m_errorIntegral;
	float m_previousError;

	float m_Kp;
	float m_Ki;
	float m_Kd;
	float m_minOutput;
	float m_maxOutput;
};

#endif // __PID_H__
