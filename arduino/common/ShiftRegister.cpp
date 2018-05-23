#include "ShiftRegister.h"
#include <Arduino.h>

//global instace

//ShiftRegister shift;

void ShiftRegister::attach(uint8_t latchpin, uint8_t clockpin, uint8_t datapin)
{
	m_LATCH = latchpin;
	m_CLOCK = clockpin;
	m_DATA  = datapin;

	pinMode(m_LATCH, OUTPUT);
	pinMode(m_CLOCK, OUTPUT);
	pinMode(m_DATA , OUTPUT);

	m_register = 0;
}

void ShiftRegister::SetHigh(int pos)
{
	if(pos <= 7 && pos >= 0)
	{
		m_register |= (1 << pos); 
		shift();
	}
}

void ShiftRegister::SetLow(int pos)
{
	if(pos <= 7 && pos >= 0)
	{
		m_register &= ~(1 << pos); 
		shift();
	}
}

void ShiftRegister::shift()
{
	digitalWrite(m_LATCH, LOW);
    shiftOut(m_DATA, m_CLOCK, MSBFIRST, m_register);
    digitalWrite(m_LATCH, HIGH);
}

void ShiftRegister::write(int pos, int state)
{
	if(state == 1)
		this->SetHigh(pos);
	else
		this->SetLow(pos);

}