#include <Arduino.h>
#include "../common/SerialTalks.h"
#include "../common/ShiftRegister.h"
#include "../common/ShiftRegAX12.h"

#define USE_SHIFTREG 1

#define RX_AX12 16
#define TX_AX12 17

#define LATCHPIN	13
#define CLOCKPIN	12
#define DATAPIN		2 

#define AX12_DATA_CONTROL	0

ShiftRegAX12 servoax;

ShiftRegister shift;

void setup()
{
    // pinMode(LATCHPIN, OUTPUT);
    // pinMode(CLOCKPIN, OUTPUT);
    // pinMode(DATAPIN , OUTPUT);

    // pinMode(2 , OUTPUT);
    // digitalWrite(2,HIGH);

	//Starting SerialTalks
	//Serial.begin(SERIALTALKS_BAUDRATE);
    //talks.begin(Serial);
    /***************************************************/
    //initialise ShiftRegister
    shift.attach(LATCHPIN,CLOCKPIN,DATAPIN);

    ShiftRegAX12::SerialBegin(9600, RX_AX12, TX_AX12, AX12_DATA_CONTROL);

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
	//talks.execute();
}