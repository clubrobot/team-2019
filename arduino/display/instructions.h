#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include "../common/SerialTalks.h"
#include "configuration.h"


// Opcodes declaration
#define SET_EEPROM_DEFAULT_MESSAGE_OPCODE	 0x09
#define SET_SPEED_MATRIX_OPCODE		 		 0x0A
#define SET_EEPROM_CHAR_LEDMATRIX_OPCODE	 0x0B
#define SET_EEPROM_CHAR_IPDISPLAY_OPCODE	 0x0C
#define SET_MATRIX_MESSAGE_OPCODE			 0x0D
#define SET_IPDISPLAY_MESSAGE_OPCODE		 0x0E

//***********
// Examples *
//***********

// SET_MATRIX_MESSAGE
/*

Display a message on a led matrix

parameters :
- BYTE : IDmartix {1,2,3}
- BYTE : Mode {SLIDE_MODE,ANIMATION_MODE,RIGHT_ROTATION_MODE,LEFT_ROTATION_MODE,UPSIDEDOWN_MODE} :
	-> SLIDE_MODE = 0
	-> ANIMATION_MODE = 1
	-> RIGHT_ROTATION_MODE = 2
	-> LEFT_ROTATION_MODE = 3
	-> UPSIDEDOWN_MODE = 4
- String to display (less than 20 char)

command example :
send(0x0D,BYTE(1),BYTE(0),STRING("ABCD "))	// Matrix 1 displays "ABCD " in SLIDE_MODE

*/

// SET_SPEED_MATRIX
/*

Change the speed of the message

parameters :
- BYTE : IDmartix {1,2,3}
- INT : TimeStep (in ms)

command example :
send(0x0A,BYTE(2),INT(100))	// Matrix 2 timeStep is now 100ms

*/

// SET_EEPROM_CHAR_LEDMATRIX
/*

Set or modify a pattern in EEPROM

parameters :
- CHAR : char code
- 8x BYTE : pattern
- BYTE : width of the pattern

command example :
talks.send(0x0B,CHAR(ord('#')),BYTE(0b01111000),BYTE(0b11111110),BYTE(0b00101111),BYTE(0b00101100),BYTE(0b00111100),BYTE(0b10011110),BYTE(0b01111100),BYTE(0b00000000),BYTE(8))	
// Set a pattern for '#' in EEPROM

*/

// SET_EEPROM_DEFAULT_MESSAGE
/*

Set the default message for the matrix in EEPROM

parameters :
- BYTE : IDmartix {1,2,3}
- INT : TimeStep (in ms)
- BYTE : Mode {SLIDE_MODE,ANIMATION_MODE,RIGHT_ROTATION_MODE,LEFT_ROTATION_MODE,UPSIDEDOWN_MODE} :
	-> SLIDE_MODE = 0
	-> ANIMATION_MODE = 1
	-> RIGHT_ROTATION_MODE = 2
	-> LEFT_ROTATION_MODE = 3
	-> UPSIDEDOWN_MODE = 4
- STRING : default message (less than 8 char)

command example :
talks.send(0x09,BYTE(1),INT(300),BYTE(1),STRING("####%")) 	

*/

// SET_IPDISPLAY_MESSAGE
/*

Set the default message for the ipdisplay in EEPROM

parameters :
- STRING : message to display (12 char or less)

command example :
talks.send(0x0E,STRING("hello"))	

*/

// SET_EEPROM_CHAR_IPDISPLAY
/*

Set a char in EEPROM for 7 seg display

parameters :
- CHAR : char
- BYTE : segments associated to this char

command example :
talks.send(0x0C,CHAR(ord('8')),BYTE(0xFF))	

*/

// Instructions prototypes

void SET_MATRIX_MESSAGE(SerialTalks &talks, Deserializer &input, Serializer &output);
void SET_IPDISPLAY_MESSAGE(SerialTalks &talks, Deserializer &input, Serializer &output);
void SET_EEPROM_CHAR_IPDISPLAY(SerialTalks &talks, Deserializer &input, Serializer &output);
void SET_EEPROM_CHAR_LEDMATRIX(SerialTalks &talks, Deserializer &input, Serializer &output);
void SET_SPEED_MATRIX(SerialTalks &talks, Deserializer &input, Serializer &output);
void SET_EEPROM_DEFAULT_MESSAGE(SerialTalks &talks, Deserializer &input, Serializer &output);


#endif