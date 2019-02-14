#include <Arduino.h>
#include "../common/SerialTalks.h"
#include "instructions.h"

void setup()
{
	//Starting SerialTalks
	Serial.begin(SERIALTALKS_BAUDRATE);
    talks.begin(Serial);

    talks.bind(ON_OPCODE,ON);
    talks.bind(OFF_OPCODE,OFF);
    talks.bind(WRITE_OPCODE,WRITE);
    talks.bind(READ_OPCODE,READ);

    pinMode(2, OUTPUT);
}
void loop()
{  
    talks.execute();
}