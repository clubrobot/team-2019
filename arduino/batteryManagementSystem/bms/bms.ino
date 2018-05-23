#include <Arduino.h>
#include "SBSCommands.h"
#include "BMS_SMBus.h"


#define LED_PIN 5

#define WRITE_CONFIG 0

#define DATA_BUFFER_LENGTH 32
uint8_t dataBuffer[DATA_BUFFER_LENGTH];

void setup() {
	// initialize the digital pin as an output.
	pinMode(LED_PIN, OUTPUT);

	// Shutdown SPI and TWI, timers, and ADC
	PRR = (1 << PRSPI) | (1 << PRTWI) | (1 << PRTIM1) | (1 << PRTIM2) | (1 << PRADC);
	// Disable  digital input on all unused ADC channel pins to reduce power consumption
	DIDR0 = ADC0D | ADC1D | ADC2D | ADC3D;

	Serial.begin(115200);  // start serial for output
	initSMBus(SBM_DEVICE_ADDRESS, LED_PIN);

  writeWordToManufacturerAccess(0xFF36);
  writeWordToManufacturerAccess(0xFFFF);

  
#if WRITE_CONFIG
	Serial.print("Update AFE CELL MAP : ");
	Serial.println(writeDataFlashU2(AFE_CELL_MAP,(uint16_t)0x0013));
  	delay(200);
  
	Serial.print("Update MFG INIT : ");
	Serial.println(writeDataFlashU2(MANUFACTURING_STATUS_INIT,(uint16_t)0x0230));
  	delay(200);
 
	Serial.print("Update FET OPTIONS : ");
	Serial.println(writeDataFlashU2(FET_OPTIONS,(uint16_t)0x0001));
  	delay(200);
  
	Serial.print("Update DESIGN CAPACITY : ");
	Serial.println(writeDataFlashI2(DESIGN_CAPACITY_MA_CONFIG,(int16_t)1650));
	delay(200);
#endif

  	//readWordFromManufacturerAccess(DEVICE_RESET);
  	//delay(1000);
	Serial.print("Design Capacity mA : ");
	Serial.println((uint16_t)readWord(DESIGN_CAPACITY));
  delay(200);
	Serial.print("Operation Status - Length :");
	uint8_t length = readBlockFromManufacturerBlockAccess(OPERATION_STATUS, dataBuffer, DATA_BUFFER_LENGTH);
	Serial.print(length);
	Serial.print(" - 0x");
	for(uint8_t i=0; i<length; i++){
		Serial.print(dataBuffer[i],HEX);
	}
	Serial.print("Security Keys : ");
	Serial.println(readU32FromManufacturerAccess((uint16_t)0x0035), HEX);
}



void loop() {
	//Serial Talk Execution
	//talks.execute();
	Serial.print("Operation Status - Length :");
  uint8_t length = readBlock(0x54, dataBuffer, DATA_BUFFER_LENGTH);
  Serial.print(length);
  Serial.print(" - 0x");
  for(uint8_t i=0; i<length; i++){
    Serial.print(dataBuffer[i],HEX);
  }
  Serial.println("");
	delay(1000);
	writeWordToManufacturerAccess(0x002B); //toggle LEDs
  delay(1000);
}




