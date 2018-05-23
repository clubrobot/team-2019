#include "StepByStepMotor.h"
#include "ShiftRegister.h"

#define FORWARD  1
#define BACKWARD 0

#define _BV(bit) (1 << (bit))

extern ShiftRegister shift;

StepByStepMotor::StepByStepMotor()
{}

void StepByStepMotor::attach(int step, int dir, int enable, int rst, int sleep)
{
	m_step    = step;
	m_dir     = dir;
	m_enable  = enable;
	m_rst 	  = rst;
	m_sleep	  = sleep;

	pinMode(m_step, OUTPUT);

}

void StepByStepMotor::begin()
{
	shift.SetLow(m_enable);
	shift.SetHigh(m_dir);
	shift.SetHigh(m_sleep);
	shift.SetHigh(m_rst);

	m_current_pos = 0;
	m_last_pos = 0;

	m_en = true;
}

void StepByStepMotor::step()
{
	//digitalWrite(m_step, LOW);
	PORTB &= ~(_BV(5));
    delayMicroseconds(5);
    PORTB |= _BV(5);            

    if(m_speed < 31)
  	{
    	delay(1000 / m_speed);
    	delayMicroseconds(1000 * (1000 / m_speed - int(1000 / m_speed)) - 110);
  	}
  	else
    	delayMicroseconds(1000000 / m_speed - 110);
}

void StepByStepMotor::update()
{
	long abssteps, pAcc, pDec, p, steps;

	m_current_pos = m_pos;

	steps = (m_current_pos - m_last_pos) * P_MM;

	abssteps = (steps>0 ? steps : -steps);

	pAcc = abssteps / (1.0 + ACC/DECC);
	pDec = abssteps - pAcc;

	if(steps > 0)
		shift.write(m_dir,FORWARD);
	else
		shift.write(m_dir,BACKWARD);

	for(p=1; p<=pAcc; p++)
  	{
	    m_speed = sqrt((2*p-1)*ACC);
	    if(m_speed > PLAT)
	    	m_speed = PLAT;

		step();

	    
  	}
	for(p = pDec; p > 0 ; p--)
	{
		m_speed = sqrt(2*p*DECC);
		if(m_speed > PLAT)
			m_speed = PLAT;
		
		step();
	}

	m_last_pos = m_pos;
}

void StepByStepMotor::enable()
{
	shift.SetLow(m_enable);
	m_en = true;
}

void StepByStepMotor::disable()
{
	shift.SetHigh(m_enable);
	m_en = false;
}