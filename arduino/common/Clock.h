#ifndef __CLOCK_H__
#define __CLOCK_H__

#include <Arduino.h>


class Clock
{
public:

	Clock() : m_startTime(micros()){}

	float getElapsedTime() const
	{
		unsigned long currentTime = micros();
		float elapsedTimeInSeconds = (currentTime - m_startTime) / float(1e6);
		return elapsedTimeInSeconds;
	}

	float restart()
	{
		unsigned long currentTime = micros();
		float elapsedTimeInSeconds = (currentTime - m_startTime) / float(1e6);
		m_startTime = currentTime;
		return elapsedTimeInSeconds;
	}

private:

	unsigned long m_startTime;
};

#endif // __CLOCK_H__
