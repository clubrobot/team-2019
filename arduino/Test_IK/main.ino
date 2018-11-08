#include <Arduino.h>
#include <iostream>
#include <string>
#include <stdexcept>
#include "../common/SerialTalks.h"
#include "instructions.h"
#include "../common/IK/IK.h"
#include "../common/IK/Arm.h"

/* debug */
template<typename T>
ostream& operator<< (ostream& out, const vector<T>& v) {
    out << "{";
    size_t last = v.size() - 1;
    for(size_t i = 0; i < v.size(); ++i) {
        out << v[i];
        if (i != last) 
            out << ", ";
    }
    out << "}";
    return out;
}

ShiftRegAX12 servoax;
ShiftRegister shift;

// coords_t tool;
// coords_t coord;
// coords_t vel;
// joints_t joints;

// path_t chemin;

// Arm arm(5,5,0);

//IK arm(10,10,5,joints,coord);

void setup()
{
    //Serial.begin(SERIALTALKS_BAUDRATE);
    //talks.begin(Serial);
    //talks.bind(GET_TOOL_OPCODE,GET_TOOL);
    //talks.bind(GET_PATH_OPCODE,GET_PATH);
    PIN_FUNC_SELECT( IO_MUX_GPIO13_REG, PIN_FUNC_GPIO);
    PIN_FUNC_SELECT( IO_MUX_GPIO12_REG, PIN_FUNC_GPIO);
    PIN_FUNC_SELECT( IO_MUX_GPIO15_REG, PIN_FUNC_GPIO);

    pinMode(LATCHPIN, OUTPUT);
    pinMode(CLOCKPIN, OUTPUT);
    pinMode(DATAPIN , OUTPUT);

    // shift.attach(LATCHPIN,CLOCKPIN,DATAPIN);
    // servoax.SerialBegin(9600, RX_AX12, TX_AX12, AX12_DATA_CONTROL);

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

    // arm.attach(2,1,3);
    // arm.begin();

    // arm.reach_position();

    // coord.x = 0;
    // coord.y = 0;
    // coord.phi = 0;

    // joints.th1 = 0;
    // joints.th2 = 0;
    // joints.th3 = 0;

    // tool.x = 10;
    // tool.y = 10;
    // tool.phi = 0;

    // Serial.print("x: ");
    // Serial.println(tool.x);
    // Serial.print("y: ");
    // Serial.println(tool.y);

    // joints = arm.inverse_kinematics(tool);

    // Serial.print("th1 rad: ");
    // Serial.println(joints.th1);
    // Serial.print("th2 rad: ");
    // Serial.println(joints.th2);
    // Serial.print("th3 rad: ");
    // Serial.println(joints.th3);

    //  Serial.print("th1 deg: ");
    //  Serial.println(joints.th1*(180/M_PI));
    //  Serial.print("th2 deg: ");
    //  Serial.println(joints.th2*(180/M_PI));
    //  Serial.print("th3 deg: ");
    //  Serial.println(joints.th3*(180/M_PI));

    // coord.x   = 0;
    // coord.y   = 0;
    // coord.phi = 0;

    // vel.x   = 0;
    // vel.y   = 0;
    // vel.phi = 0;

    // joints.th1 = 0;
    // joints.th2 = 0;
    // joints.th3 = 0;

    // tool.x   = 5;
    // tool.y   = 15;
    // tool.phi = 0;

    // chemin = arm.get_path(coord,vel,tool,vel,0.2);

    // std::cout << "t : " << chemin.path_th1.t << std::endl;
    // std::cout << "pos : " << chemin.path_th1.pos << std::endl;
    // std::cout << "vel : " << chemin.path_th1.vel << std::endl;
    // std::cout << std::endl;

    // std::cout << "t : " << chemin.path_th2.t << std::endl;
    // std::cout << "pos : " << chemin.path_th2.pos << std::endl;
    // std::cout << "vel : " << chemin.path_th2.vel << std::endl;
    // std::cout << std::endl;

    // std::cout << "t : " << chemin.path_th3.t << std::endl;
    // std::cout << "pos : " << chemin.path_th3.pos << std::endl;
    // std::cout << "vel : " << chemin.path_th3.vel << std::endl;
    // std::cout << std::endl;

}

void loop()
{
    servoax.attach(254);
    servoax.move(500);
 	//talks.execute();
}