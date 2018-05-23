#include "VelocityServo.h" 
#include "PeriodicProcess.h"
#include "SerialTalks.h"

void VelocityServo::Velocitywrite(int setpoint){
	m_setpoint = setpoint;
	m_clock.restart();
	m_startpoint = m_servo.read();
	m_arrived = false;
	if(setpoint>m_startpoint){
		m_dir = 1;
	}
	else if(setpoint<m_startpoint){
		m_dir = -1;
	}
}

void VelocityServo::process(float timestep){
	if(!m_arrived){
		int setpoint = (int) (m_clock.getElapsedTime() * m_velocity * m_dir + m_startpoint);
	
		if(m_dir == -1 && m_servo.read() <= m_setpoint){
			m_arrived = true;
			setpoint = m_setpoint;
		}
		else if(m_dir == 1 && m_servo.read() >= m_setpoint){
			m_arrived = true;
			setpoint = m_setpoint;
		}
		m_servo.write(setpoint);
	}
}

void VelocityServo::attach(int pin){
	m_servo.attach(pin);
	m_arrived = true;
}

void VelocityServo::detach(){
	m_servo.detach();
}