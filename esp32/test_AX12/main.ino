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

    servoax.attach(1);

    //servoax.setID(1);
    servoax.setSRL(2); // Respond only to READ_DATA instructions
    // servoax.setLEDAlarm(32); // max torque only
    // servoax.setShutdownAlarm(32); // max torque only
    // servoax.setMaxTorque(1023);
    // servoax.setEndlessMode(OFF);
    // servoax.hold(OFF);

    
    try
    {
        servoax.move(150.0);
    }
    catch(const AX12error& e)
    {
        cout << "id : " <<e.get_id()<< endl;
    }
    catch(const AX12Timeout& e)
    {
        cout << "id : " <<e.get_id()<< endl;
    }
    
    
}
void loop()
{
    try
    {
        cout << servoax.readPosition() << endl;
    }
    catch(const AX12error& e)
    {
        cout << "id : " <<e.get_id()<< endl;
    }
    catch(const AX12Timeout& e)
    {
        cout << "id : " <<e.get_id()<< endl;
    }
    delay(1000);
	talks.execute();
}