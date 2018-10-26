#ifndef __SHIFTREGISTER_H__
#define __SHIFTREGISTER_H__

#include <Arduino.h>

class ShiftRegister
{
	public:
		void attach(uint8_t latchpin, uint8_t clockpin, uint8_t datapin);
		void SetHigh(int pos);
		void SetLow (int pos);
		void write(int pos, int state);

	//protected:
		void shift();

		uint8_t m_LATCH;
		uint8_t m_CLOCK;
		uint8_t m_DATA;

		volatile uint8_t m_register;
};

//extern ShiftRegister shift;
#endif // __SHIFTREGISTER_H__