#include <Arduino.h>
#include "../common/SerialTalks.h"
#include <iostream>
#include "../common/AX12/AX12.h"
#include "instructions.h"

#define USE_SHIFTREG 1

// #define LATCHPIN	19
// #define CLOCKPIN	18
// #define DATAPIN		5 
using namespace std;
AX12 servoax;

//ShiftRegister shift;
double pos;
void setup()
{

	//Starting SerialTalks
	Serial.begin(SERIALTALKS_BAUDRATE);
    talks.begin(Serial);

    /*****************bind set pos FUNC*****************/
    // talks.bind(BEGIN_OPCODE,BEGIN);
    // talks.bind(SET_ANGLES_OPCODE,SET_ANGLES);
    /***************************************************/
    //initialise ShiftRegister
    //shift.attach(LATCHPIN,CLOCKPIN,DATAPIN);

    AX12::SerialBegin(1000000, 5);

    servoax.attach(254);

    //servoax.setID(1);
    //servoax.setSRL(1); // Respond only to READ_DATA instructions
    // servoax.setLEDAlarm(32); // max torque only
    // servoax.setShutdownAlarm(32); // max torque only
    // servoax.setMaxTorque(1023);
    // servoax.setEndlessMode(OFF);
    // servoax.hold(OFF);

    servoax.move(150.0);
}
void loop()
{

	talks.execute();
}