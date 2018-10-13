#include <Arduino.h>
#include <tuple>
#include <iostream>
#include <string>
#include <stdexcept>
#include "../common/AX12/AX12.h"
#include "../common/Scara/Joint.h"
#include "../common/Scara/Scara.h"

AX12 motor1;
 
coords_t tool;

coords_t coord;

joints_t joints;



void setup()
{
    Serial.begin(115200);

    coord.x = 0;
    coord.y = 0;

    joints.th1 = 0;
    joints.th2 = 0;

    Scara arm(10,10,joints,coord);

    


    tool.x = -8.41;
    tool.y = 16.28;

    Serial.print("x: ");
    Serial.println(tool.x);
    Serial.print("y: ");
    Serial.println(tool.y);

    joints = arm.inverse_kinematics(tool);

    Serial.print("th1 rad: ");
    Serial.println(joints.th1);
    Serial.print("th2 rad: ");
    Serial.println(joints.th2);

    Serial.print("th1 deg: ");
    Serial.println(joints.th1*(180/M_PI));
    Serial.print("th2 deg: ");
    Serial.println(joints.th2*(180/M_PI));
}

void loop()
{
 	
}