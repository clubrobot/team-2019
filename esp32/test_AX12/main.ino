#include <Arduino.h>
#include "../common/SerialTalks.h"
#include <iostream>
#include "../common/AX12/AX12.h"
#include "instructions.h"
#include "MotorWrapper.h"
#include "PID.h"

using namespace std;
using namespace IK;

AX12 servoax;
MotorWrapper AX1;
PID AX1_PID;

void setup()
{

	//Starting SerialTalks
	Serial.begin(SERIALTALKS_BAUDRATE);
    talks.begin(Serial);

    AX12::SerialBegin(1000000, 5);
    servoax.attach(2);

    AX1_PID.setOutputLimits(1, 1024);
    AX1_PID.setTunings(3, 0.01 , 0);

    AX1.setPID(AX1_PID);
    AX1.setMOTOR(servoax);
    AX1.setOFFSET(60);

    AX1.setMaxAcc(4000);
    AX1.setMaxDec(4000);
    AX1.setMaxSpeed(600);
    AX1.setTimestep(0.010);

    AX1.getRealVel();
    AX1.setGoalPos(0.0, 500);

    AX1.enable();
    
}
void loop()
{  
    AX1.update();
	talks.execute();

}