#ifndef __CODEWHEEL_H__
#define __CODEWHEEL_H__

#include "NonCopyable.h"
#include "Odometry.h"

#include <math.h>


class Codewheel : private NonCopyable, public AbstractCodewheel
{
public:

	Codewheel() : m_currentCounter(0), m_startCounter(0), m_wheelRadius(1 / (2 * M_PI)), m_countsPerRev(1000){}

	void attachCounter(int XY, int AXIS, int SEL1, int SEL2, int OE, int RST);
	void attachRegister(int DATA, int LATCH, int CLOCK);

	long getCounter(){update(); return m_currentCounter;}

	long getCountsPerRev(){return m_countsPerRev;}
	float getWheelRadius(){return m_wheelRadius;}
	
	void setCountsPerRev(long countsPerRev){m_countsPerRev = countsPerRev;}
	void setWheelRadius (float wheelRadius){m_wheelRadius  = wheelRadius;}

	void reset();

	float getTraveledDistance();
	float restart();

	void load(int address);
	void save(int address) const;

protected:

	void update();

	long m_currentCounter;
	long m_startCounter;

	float m_wheelRadius; // in mm
	long m_countsPerRev;

	int m_COUNTER_XY;   // Select one of the two quad counters. See below.
	int m_COUNTER_AXIS; // Not a pin: X = 0, Y = 0
	int m_COUNTER_SEL1; // MSB = 0, 2ND = 1, 3RD = 0, LSB = 1
	int m_COUNTER_SEL2; // MSB = 0, 2ND = 0, 3RD = 1, LSB = 1
	int m_COUNTER_OE;   // Active LOW. Enable the tri-states output buffers.
	int m_COUNTER_RST;  // Active LOW. Clear the internal position counter and the position latch.

	int m_REGISTER_DATA;  // Serial data input from the 74HC165 register.
	int m_REGISTER_LATCH; // Active LOW. Latch signal for the 74HC165 register.
	int m_REGISTER_CLOCK; // LOW-to-HIGH edge-triggered. Clock signal for the 74HC165 register.
};

#endif // __ROTARYENCODER_H__
