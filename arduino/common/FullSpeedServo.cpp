#include "FullSpeedServo.h"
#include "Clock.h"
#include <Servo.h>

void FullSpeedServo::SpeedWrite(int setpoint, float t){
	m_servo.write(setpoint);
	m_moving = true;
	m_time = t;
	m_clock.restart();
}

void FullSpeedServo::write(int setpoint){
	m_servo.write(setpoint);
}

void FullSpeedServo::attach(int pin){
	m_servo.attach(pin);
}

void FullSpeedServo::detach(){
	m_servo.detach();
}

void FullSpeedServo::process(float timestep){
	if(m_moving && m_clock.getElapsedTime()>= m_time){
		m_servo.detach();
		m_moving = false;
	}
}