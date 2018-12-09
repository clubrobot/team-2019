#ifndef EEPROM_MANAGMENT_H
#define EEPROM_MANAGMENT_H

#include <Arduino.h>
#include <EEPROM.h>


void EEPROMWriteInt(int p_address, int p_value);	//This function will write a 2 byte integer to the eeprom at the specified address and address + 1
unsigned int EEPROMReadInt(int p_address);	//This function will read a 2 byte integer from the eeprom at the specified address and address + 1
void printEEPROM();		// print EEPROM in the terminal


#endif //EEPROM_MANAGMENT_H