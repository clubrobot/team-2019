 #include <Arduino.h>
#include <EEPROM.h>

#include "Codewheel.h"
#include "SerialTalks.h"

uint8_t shiftInSlow(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder) {
    uint8_t value = 0;
    uint8_t i;

    for(i = 0; i < 8; ++i) {
        digitalWrite(clockPin, HIGH);
        delayMicroseconds(1);
        if(bitOrder == LSBFIRST)
            value |= digitalRead(dataPin) << i;
        else
            value |= digitalRead(dataPin) << (7 - i);
        digitalWrite(clockPin, LOW);
        delayMicroseconds(1);
    }
    return value;
}

void Codewheel::attachCounter(int XY, int AXIS, int SEL1, int SEL2, int OE, int RST)
{
	m_COUNTER_AXIS = AXIS;
	m_COUNTER_XY   = XY;
	m_COUNTER_SEL1 = SEL1;
	m_COUNTER_SEL2 = SEL2;
	m_COUNTER_OE   = OE;
	m_COUNTER_RST  = RST;
	pinMode(m_COUNTER_XY,   OUTPUT);
	pinMode(m_COUNTER_SEL1, OUTPUT);
	pinMode(m_COUNTER_SEL2, OUTPUT);
	pinMode(m_COUNTER_OE,   OUTPUT);
	pinMode(m_COUNTER_RST,  OUTPUT);
}

void Codewheel::attachRegister(int DATA, int LATCH, int CLOCK)
{
	m_REGISTER_DATA  = DATA;
	m_REGISTER_LATCH = LATCH;
	m_REGISTER_CLOCK = CLOCK;
	pinMode(m_REGISTER_DATA,  INPUT);
	pinMode(m_REGISTER_LATCH, OUTPUT);
	pinMode(m_REGISTER_CLOCK, OUTPUT);
}

void Codewheel::reset()
{
	m_startCounter = 0;
	digitalWrite(m_COUNTER_RST, LOW);
	digitalWrite(m_COUNTER_RST, HIGH);
}

void Codewheel::update()
{
	digitalWrite(m_COUNTER_XY, m_COUNTER_AXIS);
	digitalWrite(m_COUNTER_OE, LOW);
	m_currentCounter = 0;
	for (int i = 0; i < 4; i++)
	{
		// Select one byte of the 32 bits quad counter
		// Its value will be stored in the shift register
		byte SEL1 = i & 0x01;
		byte SEL2 = (~i >> 1) & 0x01;
		digitalWrite(m_COUNTER_SEL1, SEL1);
		digitalWrite(m_COUNTER_SEL2, SEL2);

		// The counter value is received in big endian order
		m_currentCounter <<= 8;
        
		// Read the value stored in the shift register
		digitalWrite(m_REGISTER_CLOCK, HIGH);
		digitalWrite(m_REGISTER_LATCH, HIGH);
		m_currentCounter += shiftInSlow(m_REGISTER_DATA, m_REGISTER_CLOCK, MSBFIRST);
		digitalWrite(m_REGISTER_LATCH, LOW);
	}
	digitalWrite(m_COUNTER_OE, HIGH);
}

float Codewheel::getTraveledDistance()
{
	return (float)(getCounter() - m_startCounter) / m_countsPerRev * (2.0 * M_PI * m_wheelRadius);
}

float Codewheel::restart()
{
	float distance = getTraveledDistance();
	m_startCounter = m_currentCounter;
	return distance;
}

void Codewheel::load(int address)
{
	EEPROM.get(address, m_wheelRadius);  address += sizeof(m_wheelRadius);
	EEPROM.get(address, m_countsPerRev); address += sizeof(m_countsPerRev);
}

void Codewheel::save(int address) const
{
	EEPROM.put(address, m_wheelRadius);  address += sizeof(m_wheelRadius);
	EEPROM.put(address, m_countsPerRev); address += sizeof(m_countsPerRev);
	EEPROM.commit();
}

void Codewheel::setCountsPerRev(long countsPerRev)
{
        m_countsPerRev = countsPerRev;
}

void Codewheel::setWheelRadius (float wheelRadius)
{
        m_wheelRadius = wheelRadius;
}
