#include <Arduino.h>
#include "../common/AX12/AX12.h"

AX12 motor1;

void setup()
{
    Serial.begin(115200);

    motor1.SerialBegin(1000000,2);
    motor1.attach(1);
}

void loop()
{

}