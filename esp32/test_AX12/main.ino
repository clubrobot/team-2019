#include <Arduino.h>
#include "../common/SerialTalks.h"
#include "../common/ShiftRegister.h"
#include "../common/ShiftRegAX12.h"
#include "instructions.h"

#define USE_SHIFTREG 1

#define LATCHPIN	10
#define CLOCKPIN	12
#define DATAPIN		5 

ShiftRegAX12 servoax;

ShiftRegister shift;

void setup()
{
    pinMode(LATCHPIN, OUTPUT);
    pinMode(CLOCKPIN, OUTPUT);
    pinMode(DATAPIN , OUTPUT);

	//Starting SerialTalks
	Serial.begin(SERIALTALKS_BAUDRATE);
    talks.begin(Serial);

    /*****************bind set pos FUNC*****************/
    // talks.bind(BEGIN_OPCODE,BEGIN);
    // talks.bind(SET_ANGLES_OPCODE,SET_ANGLES);
    /***************************************************/
    //initialise ShiftRegister
    shift.attach(LATCHPIN,CLOCKPIN,DATAPIN);

    ShiftRegAX12::SerialBegin(9600, 0, 0, 0);

    servoax.attach(254);
    //servoax.setSRL(1); // Respond only to READ_DATA instructions
    servoax.setLEDAlarm(32); // max torque only
    servoax.setShutdownAlarm(32); // max torque only
    servoax.setMaxTorque(1023);
    servoax.setEndlessMode(OFF);
    servoax.hold(OFF);

    servoax.move(150.0);

    
}

void loop()
{
	talks.execute();
}