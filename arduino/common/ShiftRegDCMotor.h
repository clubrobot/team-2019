#ifndef __SHIFTREGDCMOTOR_H__
#define __SHIFTREGDCMOTOR_H__

#include "NonCopyable.h"
#include "DifferentialController.h"

#include <math.h>



class ShiftRegDCMotor : private NonCopyable, public AbstractMotor
{
public:

	ShiftRegDCMotor() : m_enabled(false), m_velocity(0), m_wheelRadius(1 / (2 * M_PI)), m_constant(1), m_maxPWM(1){}

	void attach(int EN, int PWM, int DIR);

	void setVelocity(float velocity){m_velocity = velocity; update();}

	void setConstant   (float constant)   {m_constant    = constant;    update();}
	void setWheelRadius(float wheelRadius){m_wheelRadius = wheelRadius; update();}
	void setMaxPWM     (float maxPWM)     {m_maxPWM      = maxPWM;      update();}

	void enable (){m_enabled = true;  update();}
	void disable(){m_enabled = false; update();}

	float getVelocity   () const {return m_velocity;}
	float getConstant   () const {return m_constant;}
	float getWheelRadius() const {return m_wheelRadius;}
	float getMaxPWM     () const {return m_maxPWM;}
	bool  isEnabled     () const {return m_enabled;}

	float getMaxVelocity() const;

	void load(int address);
	void save(int address) const;
	
protected:

	void update();

	bool  m_enabled;
	float m_velocity; // in mm/s (millimeters per second)
	float m_wheelRadius; // in mm
	float m_constant; // (60 * reduction_ratio / velocity_constant_in_RPM) / supplied_voltage_in_V
	float m_maxPWM; // in range ]0, 1]

	int	m_EN;
	int	m_PWM;
	int	m_DIR;
};

class ShiftRegDCMotorsDriver
{
public:

	void attach(int RESET, int FAULT);

	void reset();

	bool isFaulty();

private:

	int m_RESET;
	int m_FAULT;
};

#endif // __SHIFTREGDCMOTOR_H__
