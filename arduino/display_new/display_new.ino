#include <Arduino.h>
#include "configuration.h"
#include "../common/SerialTalks.h"

// Variables

#define _SET_DATA_OPCODE 0x10

void SET_DATA(SerialTalks &talks, Deserializer &input, Serializer &output);
void updateMatrix(int datapin, int clockpin, int latch, uint16_t _data);

void setup()
{
    Serial.begin(SERIALTALKS_BAUDRATE);
	talks.begin(Serial);

	talks.bind(_SET_DATA_OPCODE, SET_DATA);
    
    pinMode(DATA_MATRIX1, OUTPUT);
	pinMode(CLOCK_MATRIX1, OUTPUT);
	pinMode(LATCH_MATRIX1, OUTPUT);
    pinMode(DATA_MATRIX2, OUTPUT);
	pinMode(CLOCK_MATRIX2, OUTPUT);
	pinMode(LATCH_MATRIX2, OUTPUT);

    updateMatrix(DATA_MATRIX1,CLOCK_MATRIX1,LATCH_MATRIX1,16384);
    updateMatrix(DATA_MATRIX2,CLOCK_MATRIX2,LATCH_MATRIX2,16384);

}

void updateMatrix(int datapin, int clockpin, int latch, uint16_t _data)
{
	byte octet1 = _data&0x00FF;
	byte octet2 = (_data&0xFF00)>>8;
	digitalWrite(latch,LOW);
	shiftOut(datapin, clockpin, MSBFIRST, octet1);
	shiftOut(datapin, clockpin, MSBFIRST, octet2);
	digitalWrite(latch,HIGH);
}

void SET_DATA(SerialTalks &talks, Deserializer &input, Serializer &output){
    uint16_t _data = input.read<long>();
    updateMatrix(DATA_MATRIX1,CLOCK_MATRIX1,LATCH_MATRIX1,_data);
    updateMatrix(DATA_MATRIX2,CLOCK_MATRIX2,LATCH_MATRIX2,_data);
}

void loop(){
    talks.execute();
}


