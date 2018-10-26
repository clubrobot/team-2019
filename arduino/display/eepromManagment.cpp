#include "eepromManagment.h"
#include "../common/SerialTalks.h"
#include "configuration.h"

extern SerialTalks talks;

//This function will write a 2 byte integer to the eeprom at the specified address and address + 1
void EEPROMWriteInt(int p_address, int p_value)
{
	byte lowByte = ((p_value >> 0) & 0xFF);
	byte highByte = ((p_value >> 8) & 0xFF);

	EEPROM.write(p_address, lowByte);
	EEPROM.write(p_address + 1, highByte);
}

//This function will read a 2 byte integer from the eeprom at the specified address and address + 1
unsigned int EEPROMReadInt(int p_address)
{
	byte lowByte = EEPROM.read(p_address);
	byte highByte = EEPROM.read(p_address + 1);

	return ((lowByte << 0) & 0xFF) + ((highByte << 8) & 0xFF00);
}

void printEEPROM()
{
	/*
	talks.out << "--------------------------------------------------------------------------------------------------------------------------------\n";
	talks.out << "                                                  Arduino EEPROM\n";
	talks.out << "--------------------------------------------------------------------------------------------------------------------------------\n";
	for (int i = 0; i<64; i++){
		for (int j = 0; j<16; j++){
			talks.out << i*16+j << "\t";
		}
		talks.out << "\n";
		for (int j = 0; j<16; j++){
			talks.out << EEPROM.read(i*16+j) << "\t";
			talks.execute();
		}
		talks.out << "\n";
		talks.out << "--------------------------------------------------------------------------------------------------------------------------------\n";
	}
	*/
	talks.out << "--------------------------------------------------------------------------------------------------------------------------------\n";
	talks.out << "                                                  Arduino EEPROM\n";
	talks.out << "--------------------------------------------------------------------------------------------------------------------------------\n";
	talks.out << "width\n";
	for(char c = START_CHAR_LED_MATRIX; c < END_CHAR_LED_MATRIX; c++){
		talks.execute();
		talks.out << c << " : " << EEPROM.read(EEPROM_LEDMATRIX_WIDTH_START_ADDRESS + c - START_CHAR_LED_MATRIX) << "\n";
	}
}