#include "CRC16.h"


CRC16::CRC16()
{
	
}

uint16_t CRC16::CRCprocessByte(uint8_t data)
{
	uint8_t i;

 	crc = crc ^ ((uint16_t)data << 8);
 	for (i = 0; i < 8; i++) 
 	{
   		if (crc & 0x8000)
     	crc = (crc << 1) ^ CRC_POLYNOME;
   		else
     	crc <<= 1;
 	}
 	return crc;
}

uint16_t CRC16::CRCprocessBuffer(const uint8_t *data_p, uint16_t length)
{
	crc = 0;
	for(int i = 0 ; i<length; i++)
	{
   		CRCprocessByte(data_p[i]);
 	}
 	return crc;
}

bool CRC16::CRCcheck(uint8_t *data_p, uint16_t length, uint16_t crc)
{
	uint16_t new_crc;

	new_crc = CRCprocessBuffer(data_p, length);

	// Serial.println(crc, HEX);
	// Serial.println(new_crc, HEX);

	if(crc == new_crc)
		return true;
	else
		return false;
}