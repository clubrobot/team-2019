#ifndef __FULLSPEEDSERVO_H__
#define __FULLSPEEDSERVO_H__

#include "Clock.h"
#include <Servo.h>
#include "PeriodicProcess.h"

class FullSpeedServo : public PeriodicProcess{

private : 

	Servo m_servo;
	float m_time;
	bool m_moving;
	Clock m_clock;

public : 
	
//	void FullSpeedServo();
	void SpeedWrite(int setpoint, float time);

	void write(int setpoint);
	void attach(int pin);
	void detach();
	bool attached(){return m_servo.attached();}
	int read(){return m_servo.read();}

protected:
	
	virtual void process(float timestep);
	
};

#endif