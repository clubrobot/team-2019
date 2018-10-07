#include <Arduino.h>
#include <tuple>
#include <iostream>
#include <string>
#include <stdexcept>
#include "../common/AX12/AX12.h"
#include "../common/Scara/Joint.h"

AX12 motor1;

Joint art(1,-1,1,-1,1,-1,1);

double_tuple_t pos_tuple;
double_tuple_t vel_tuple;
double_tuple_t acc_tuple;
double_tuple_t t_tuple;

double pos[4];
double vel[4];
double acc[4];
double t[4];

double initial_pos = 0.1;
double initial_vel = 0;
double final_pos = 0.5;
double final_vel = 0;


void setup()
{
    Serial.begin(115200);

    motor1.SerialBegin(1000000,2);
    motor1.attach(1);

    std::tie(t_tuple, pos_tuple, vel_tuple, acc_tuple) = art.get_path(initial_pos,initial_vel,final_pos,final_vel,10,0.2);

    std::tie(t[0], t[1], t[2] , t[3]) = t_tuple;
    std::tie(pos[0], pos[1], pos[2] , pos[3]) = pos_tuple;
    std::tie(vel[0], vel[1], vel[2] , vel[3]) = vel_tuple;
    std::tie(acc[0], acc[1], acc[2] , acc[3]) = acc_tuple;

    Serial.print("T1 :");
    Serial.print(t[0]);
    Serial.print(" |T2 :");
    Serial.print(t[1]);
    Serial.print(" |T3 :");
    Serial.print(t[2]);
    Serial.print(" |T4 :");
    Serial.println(t[3]);

    Serial.print("pos1 :");
    Serial.print(pos[0]);
    Serial.print(" |pos2 :");
    Serial.print(pos[1]);
    Serial.print(" |pos3 :");
    Serial.print(pos[2]);
    Serial.print(" |pos4 :");
    Serial.println(pos[3]);

    Serial.print("vel1 :");
    Serial.print(vel[0]);
    Serial.print(" |vel2 :");
    Serial.print(vel[1]);
    Serial.print(" |vel3 :");
    Serial.print(vel[2]);
    Serial.print(" |vel4 :");
    Serial.println(vel[3]);

    Serial.print("acc1 :");
    Serial.print(acc[0]);
    Serial.print(" |acc2 :");
    Serial.print(acc[1]);
    Serial.print(" |acc3 :");
    Serial.print(acc[2]);
    Serial.print(" |acc4 :");
    Serial.println(acc[3]);


}

void loop()
{
 	
}