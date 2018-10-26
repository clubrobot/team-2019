#include <Arduino.h>
#include "../common/SerialTalks.h"
#include "IPDisplay.h" 
#include "instructions.h"
#include "configuration.h"
#include "ledMatrix.h"

// Variables

Ipdisplay ipdisplay;
LedMatrix ledmatrix1;
LedMatrix ledmatrix2;
LedMatrix ledmatrix3;

void setup()
{
	Serial.begin(SERIALTALKS_BAUDRATE);
	talks.begin(Serial);

	talks.bind(SET_MATRIX_MESSAGE_OPCODE, SET_MATRIX_MESSAGE);
	talks.bind(SET_IPDISPLAY_MESSAGE_OPCODE, SET_IPDISPLAY_MESSAGE);
	talks.bind(SET_EEPROM_CHAR_IPDISPLAY_OPCODE,SET_EEPROM_CHAR_IPDISPLAY);
	talks.bind(SET_SPEED_MATRIX_OPCODE,SET_SPEED_MATRIX);
	talks.bind(SET_EEPROM_CHAR_LEDMATRIX_OPCODE,SET_EEPROM_CHAR_LEDMATRIX);
	talks.bind(SET_EEPROM_DEFAULT_MESSAGE_OPCODE,SET_EEPROM_DEFAULT_MESSAGE);

	// Variables initialisation

	ipdisplay.attach(DATA_IPDISPLAY, CLOCK_IPDISPLAY, LATCH_IPDISPLAY);
	ipdisplay.setTimestep(IP_DISPLAY_TIMESTEP);
	ipdisplay.enable();

	ledmatrix1.attach(DATA_MATRIX1,CLOCK_MATRIX1,LATCH_MATRIX1,ROTATION_MATRIX_1,1);
	ledmatrix1.setTimestep(LED_MATRIX_TIMESTEP);
	ledmatrix1.enable();

	ledmatrix2.attach(DATA_MATRIX2,CLOCK_MATRIX2,LATCH_MATRIX2,ROTATION_MATRIX_2,2);
	ledmatrix2.setTimestep(LED_MATRIX_TIMESTEP);
	ledmatrix2.enable();

	ledmatrix3.attach(DATA_MATRIX3,CLOCK_MATRIX3,LATCH_MATRIX3,ROTATION_MATRIX_3,3);
	ledmatrix3.setTimestep(LED_MATRIX_TIMESTEP);
	ledmatrix3.enable();


}

void loop()
{
	talks.execute();
	ipdisplay.update();
	ledmatrix1.update();
	ledmatrix2.update();
	ledmatrix3.update();
}
