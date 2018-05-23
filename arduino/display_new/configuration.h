#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include "../common/SerialTalks.h"

//************************
// initialisation EEPROM *
//************************

//************
// Constants *
//************

// Time constants
#define IP_DISPLAY_TIMESTEP 3e-3    // Execution step time in s
#define LED_MATRIX_TIMESTEP 2e-4    // Execution step time in s
#define PATTERN_TIMESTEP 100e-3    // Execution step time in s

// IP_display constants
#define IP_DISPLAY_BUFFER_SIZE 25   // Size of the data buffer receiver
#define DISP_NUMBER 3               // Number of 7 seg common pin

//*****************************
// 7seg Display configuration *
//*****************************

// common pin DISP1 , DISP2 , DISP3
const byte DISP_PIN[] = {4, 3, 2};

// shift register pins
#define DATA_IPDISPLAY 5
#define CLOCK_IPDISPLAY 6
#define LATCH_IPDISPLAY 7

//ASCII TABLE => from 32 to 126
//a,b,c,d,e,f,g,dp  
#define START_CHAR ' '
#define END_CHAR '~'

//Weight of each segment
//                         a , b , c , d , e , f , g , dp
const byte DISP[4][8] = {{1, 128, 16, 4, 2, 64, 32, 8},
                         {1, 128, 32, 8, 4, 2, 64, 16},
                         {1, 128, 16, 4, 2, 64, 32, 8},		
                         {1, 128, 32, 8, 4, 2, 64, 16}};

//***************************
// LED Matrix Configuration *
//***************************

// shift register pins (Matrix 1)
#define DATA_MATRIX1 	8
#define CLOCK_MATRIX1 	9
#define LATCH_MATRIX1 	10

// shift register pins (Matrix 2)
#define DATA_MATRIX2	11
#define CLOCK_MATRIX2	12
#define LATCH_MATRIX2	13

// shift register pins (Matrix 3)
#define DATA_MATRIX3	A0
#define CLOCK_MATRIX3	A1
#define LATCH_MATRIX3	A2

// general configuration

#define ROTATION_MATRIX_1 90
#define ROTATION_MATRIX_2 90
#define ROTATION_MATRIX_3 90
#define NB_PATTERNS_MAX 20
enum {SLIDE_MODE,ANIMATION_MODE,RIGHT_ROTATION_MODE,LEFT_ROTATION_MODE,UPSIDEDOWN_MODE};

// Weight of each dot
const unsigned int rows[] = {4, 16384, 8, 32768, 128, 1, 512, 16}; //Row1,Row2,...,Row8
const unsigned int cols[] = {256, 2048, 64, 2, 32, 4096, 8192, 1024}; //Col1,Col2,...,Col8

/*
const unsigned int rows[] = {4, 16384, 8, 32768, 128, 1, 512, 16}; //Row1,Row2,...,Row8
const unsigned int cols[] = {256, 2048, 64, 2, 32, 4096, 8192, 1024}; //Col1,Col2,...,Col8
*/

#define START_CHAR_LED_MATRIX ' '	// Ascii for group separator
#define END_CHAR_LED_MATRIX '~'

 
//***********************
// EEPROM configuration *
//************************

// IP Display
#define EEPROM_START_ADDRESS SERIALTALKS_UUID_ADDRESS+SERIALTALKS_UUID_LENGTH
#define EEPROM_IPDISPLAY_START_ADDRESS EEPROM_START_ADDRESS
#define EEPROM_IPDISPLAY_LENGTH END_CHAR-START_CHAR+1
// LED Matrix
#define EEPROM_LEDMATRIX_DATA_START_ADDRESS EEPROM_IPDISPLAY_START_ADDRESS+EEPROM_IPDISPLAY_LENGTH
#define EEPROM_LEDMATRIX_DATA_LENGTH	((END_CHAR_LED_MATRIX-START_CHAR_LED_MATRIX+1)*8)
#define EEPROM_LEDMATRIX_WIDTH_START_ADDRESS EEPROM_LEDMATRIX_DATA_START_ADDRESS + EEPROM_LEDMATRIX_DATA_LENGTH
#define EEPROM_LEDMATRIX_WITDH_LENGTH END_CHAR_LED_MATRIX-START_CHAR_LED_MATRIX+1
#define EEPROM_LEDMATRIX1_MESSAGE_SAVED_START_ADDRESS EEPROM_LEDMATRIX_WIDTH_START_ADDRESS + EEPROM_LEDMATRIX_WITDH_LENGTH
#define EEPROM_LEDMATRIX1_MESSAGE_SAVED_LENGTH 8
#define EEPROM_LEDMATRIX2_MESSAGE_SAVED_START_ADDRESS EEPROM_LEDMATRIX1_MESSAGE_SAVED_START_ADDRESS + EEPROM_LEDMATRIX1_MESSAGE_SAVED_LENGTH
#define EEPROM_LEDMATRIX2_MESSAGE_SAVED_LENGTH 8
#define EEPROM_LEDMATRIX3_MESSAGE_SAVED_START_ADDRESS EEPROM_LEDMATRIX2_MESSAGE_SAVED_START_ADDRESS + EEPROM_LEDMATRIX2_MESSAGE_SAVED_LENGTH
#define EEPROM_LEDMATRIX3_MESSAGE_SAVED_LENGTH 8
#define EEPROM_LEDMATRIX1_TIMESTEP_START_ADDRESS EEPROM_LEDMATRIX3_MESSAGE_SAVED_START_ADDRESS + EEPROM_LEDMATRIX3_MESSAGE_SAVED_LENGTH
#define EEPROM_LEDMATRIX1_TIMESTEP_LENGTH sizeof(int)
#define EEPROM_LEDMATRIX2_TIMESTEP_START_ADDRESS EEPROM_LEDMATRIX1_TIMESTEP_START_ADDRESS + EEPROM_LEDMATRIX1_TIMESTEP_LENGTH
#define EEPROM_LEDMATRIX2_TIMESTEP_LENGTH sizeof(int)
#define EEPROM_LEDMATRIX3_TIMESTEP_START_ADDRESS EEPROM_LEDMATRIX2_TIMESTEP_START_ADDRESS + EEPROM_LEDMATRIX2_TIMESTEP_LENGTH
#define EEPROM_LEDMATRIX3_TIMESTEP_LENGTH sizeof(int)
#define EEPROM_LEDMATRIX1_DEFAULT_MODE_ADDRESS EEPROM_LEDMATRIX3_TIMESTEP_START_ADDRESS + EEPROM_LEDMATRIX3_TIMESTEP_LENGTH + 1
#define EEPROM_LEDMATRIX2_DEFAULT_MODE_ADDRESS EEPROM_LEDMATRIX1_DEFAULT_MODE_ADDRESS + 1
#define EEPROM_LEDMATRIX3_DEFAULT_MODE_ADDRESS EEPROM_LEDMATRIX2_DEFAULT_MODE_ADDRESS + 1

#endif