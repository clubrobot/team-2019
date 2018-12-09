#include <Arduino.h>
#include "instructions.h"
#include "IPDisplay.h"
#include "ledMatrix.h"
#include <EEPROM.h>
#include "eepromManagment.h"

// Global variables
extern Ipdisplay ipdisplay;
extern LedMatrix ledmatrix1;
extern LedMatrix ledmatrix2;
extern LedMatrix ledmatrix3;


// Instructions
void SET_MATRIX_MESSAGE(SerialTalks &talks, Deserializer &input, Serializer &output)
{
	byte matrixID = input.read<byte>();			// ID of the matrix to be updates. Set to 0 to send the data to all the matrix
	byte mode = input.read<byte>();				// Mode of the matrix {SLIDE_MODE, ANIMATION_MODE}
	char buffer[NB_PATTERNS_MAX] = "";
	for (int i = 0; i< NB_PATTERNS_MAX; i++){
		buffer[i] = input.read<char>();
	}

	switch(matrixID){
		case 1 :
			ledmatrix1.setMode(mode);
			ledmatrix1.computeBuffer(buffer);
			break;
		case 2 :
			ledmatrix2.setMode(mode);
			ledmatrix2.computeBuffer(buffer);
			break;
		case 3 :
			ledmatrix3.setMode(mode);
			ledmatrix3.computeBuffer(buffer);
	}
}

void SET_IPDISPLAY_MESSAGE(SerialTalks &talks, Deserializer &input, Serializer &output)
{
	
    char buffer[IP_DISPLAY_BUFFER_SIZE] = "";
	for (int i = 0; i< IP_DISPLAY_BUFFER_SIZE; i++){
		buffer[i] = input.read<char>();
	}

	int nbDigits = 0;
	
	for (int i = 0; i < IP_DISPLAY_BUFFER_SIZE && buffer[i]!='\0'; i++)
    {
        if (buffer[i] >= START_CHAR && buffer[i] <= END_CHAR)
        {
            nbDigits++;
			if(buffer[i+1] == '.' && buffer[i] != '.'){	// gère le cas de l'affichage d'un point après un digit
				i++;
			}
			if(nbDigits>12){		// Buffer overflow case
				nbDigits--;
				buffer[i] = 0;
			}
        }
    }
	ipdisplay.computeBuffer(buffer, nbDigits);
}


void SET_EEPROM_CHAR_IPDISPLAY(SerialTalks &talks, Deserializer &input, Serializer &output)
{
	char character = input.read<char>();		// The char to change
	byte data = input.read<byte>();				// The segments to display
	EEPROM.write(EEPROM_IPDISPLAY_START_ADDRESS + character - START_CHAR,data);
}


void SET_EEPROM_CHAR_LEDMATRIX(SerialTalks &talks, Deserializer &input, Serializer &output)
{
	char character = input.read<char>();		// The char to change
	for(int i = 0; i<8;i++){
		 EEPROM.write(EEPROM_LEDMATRIX_DATA_START_ADDRESS + i + 8*(int)( character - START_CHAR_LED_MATRIX), input.read<byte>());		// pattern
	}
	byte width = input.read<byte>();
	EEPROM.write(EEPROM_LEDMATRIX_WIDTH_START_ADDRESS +  (int)character - START_CHAR_LED_MATRIX, width);		// width
}

void SET_SPEED_MATRIX(SerialTalks &talks, Deserializer &input, Serializer &output)
{
	byte idMatrix = input.read<byte>();			// The matrix ID
	int speed = input.read<int>();				// The pattern speed in ms
	switch(idMatrix){
		case 0 :
			ledmatrix1.changeTimeStep((float)speed/1000);
			ledmatrix2.changeTimeStep((float)speed/1000);
			ledmatrix3.changeTimeStep((float)speed/1000);
		case 1 :
			ledmatrix1.changeTimeStep((float)speed/1000);
			break;
		case 2 :
			ledmatrix2.changeTimeStep((float)speed/1000);
			break;
		case 3 :
			ledmatrix3.changeTimeStep((float)speed/1000);
	}
}

void SET_EEPROM_DEFAULT_MESSAGE(SerialTalks &talks, Deserializer &input, Serializer &output)
{
	byte idMatrix = input.read<byte>();			// The matrix ID
	int speed = input.read<int>();				// The pattern speed in ms
	byte mode = input.read<byte>();				// Mode of the matrix {SLIDE_MODE, ANIMATION_MODE}
	char buffer[NB_PATTERNS_MAX];
	
	switch(idMatrix){
		case 1 :
			EEPROM.write(EEPROM_LEDMATRIX1_DEFAULT_MODE_ADDRESS,mode);
			EEPROMWriteInt(EEPROM_LEDMATRIX1_TIMESTEP_START_ADDRESS,speed);
			ledmatrix1.changeTimeStep((float)speed/1000);
			for (int i = 0; i< EEPROM_LEDMATRIX1_MESSAGE_SAVED_LENGTH; i++){
				buffer[i] = input.read<char>();
				EEPROM.write(EEPROM_LEDMATRIX1_MESSAGE_SAVED_START_ADDRESS+i, buffer[i]);
			}
			ledmatrix1.setMode(mode);
			ledmatrix1.computeBuffer(buffer);
			break;
		case 2 :
			EEPROMWriteInt(EEPROM_LEDMATRIX2_TIMESTEP_START_ADDRESS,speed);
			EEPROM.write(EEPROM_LEDMATRIX2_DEFAULT_MODE_ADDRESS,mode);
			for (int i = 0; i< EEPROM_LEDMATRIX2_MESSAGE_SAVED_LENGTH; i++){
				buffer[i] = input.read<char>();
				EEPROM.write(EEPROM_LEDMATRIX2_MESSAGE_SAVED_START_ADDRESS+i, buffer[i]);
			}
			ledmatrix2.changeTimeStep((float)speed/1000);
			ledmatrix2.setMode(mode);
			ledmatrix2.computeBuffer(buffer);
			break;
		case 3 :
			EEPROMWriteInt(EEPROM_LEDMATRIX3_TIMESTEP_START_ADDRESS,speed);
			EEPROM.write(EEPROM_LEDMATRIX3_DEFAULT_MODE_ADDRESS,mode);
			for (int i = 0; i< EEPROM_LEDMATRIX3_MESSAGE_SAVED_LENGTH; i++){
				buffer[i] = input.read<char>();
				EEPROM.write(EEPROM_LEDMATRIX3_MESSAGE_SAVED_START_ADDRESS+i, buffer[i]);
			}
			ledmatrix3.changeTimeStep((float)speed/1000);
			ledmatrix3.setMode(mode);
			ledmatrix3.computeBuffer(buffer);
	}
}

