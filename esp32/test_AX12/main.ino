#include <Arduino.h>
#include "../common/SerialTalks.h"
#include <iostream>
#include "../common/AX12/AX12.h"
#include "instructions.h"
#include "MotorWrapper.h"
#include "PID.h"

using namespace std;
using namespace IK;

AX12 servoax1;
AX12 servoax2;
AX12 servoax3;
void setup()
{

	//Starting SerialTalks
	Serial.begin(SERIALTALKS_BAUDRATE);
    talks.begin(Serial);

    AX12::SerialBegin(1000000, 5);
    servoax1.attach(254);
    try
    {
        servoax1.move(150);
    }
    catch(const AX12error &err)
    {
        cout<<"error"<<endl;
    }
    catch(const AX12Timeout &err)
    {
        cout<<"Timeout"<<endl;
    }
    
    
    try
    {
        cout << servoax1.readPosition() << endl;
    }
    catch(const AX12error &err)
    {
        cout<<"error"<<endl;
    }
    catch(const AX12Timeout &err)
    {
        cout<<"Timeout"<<endl;
    }
   

    

    
    
}
void loop()
{  

}