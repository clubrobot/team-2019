#ifndef __VELOCITYSERVO_H__
#define __VELOCITYSERVO_H__

#include "Clock.h"
#include <Servo.h>
#include "PeriodicProcess.h"

class VelocityServo : public PeriodicProcess{

private : 

	Servo m_servo;
	int m_setpoint;
	float m_velocity;
	int m_startpoint;
	int m_dir;
	bool m_arrived; 
	Clock m_clock;

public : 
	
	VelocityServo(){m_velocity = 100;}

	void setVelocity(float vel){m_velocity = vel;}
	void Velocitywrite(int setpoint);
	

	void attach(int pin);
	void detach();
	void write(int setpoint){m_arrived = true; m_servo.write(setpoint); }
	bool attached(){return m_servo.attached();}
	int read(){return m_servo.read();}

protected:
	
	virtual void process(float timestep);
	
};

#endif