#ifndef __SHIFTREGISTER_H__
#define __SHIFTREGISTER_H__

#include <Arduino.h>

class ShiftRegister
{
	public:
		void attach(int latchpin, int clockpin, int datapin);
		void SetHigh(int pos);
		void SetLow (int pos);
		void write(int pos, int state);

	//protected:
		void update();

		int m_LATCH;
		int m_CLOCK;
		int m_DATA;

		uint8_t m_register;
};


#endif // __SHIFTREGISTER_H__