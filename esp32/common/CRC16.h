#include <Arduino.h>
#include <stdio.h>
#include <string.h>

#define CRC_POLYNOME 0X1021 //CRC16_CCITT


class CRC16
{
	public:
		CRC16();

		uint16_t CRCprocessByte(uint8_t data);
		uint16_t CRCprocessBuffer(const uint8_t *data_p, uint16_t length);
		bool 	 CRCcheck(uint8_t *data_p, uint16_t length, uint16_t crc);

	private:

		uint16_t crc;

	
};
