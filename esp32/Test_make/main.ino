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
coords_t vel;

joints_t joints;

path_t chemin;

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


void setup()
{
    Serial.begin(115200);

    coord.x = 0;
    coord.y = 0;

    vel.x = 0;
    vel.y = 0;

    joints.th1 = 0;
    joints.th2 = 0;

    Scara arm(10,10,joints,coord);

    


    tool.x = -8.41;
    tool.y = 16.28;


    chemin = arm.get_path(coord,vel,tool,vel,0.2);

    std::cout << chemin.path_th1.t << std::endl;
    std::cout << chemin.path_th1.pos << std::endl;
    std::cout << chemin.path_th1.vel << std::endl;
    std::cout << chemin.path_th1.acc << std::endl;

    std::cout << chemin.path_th2.t << std::endl;
    std::cout << chemin.path_th2.pos << std::endl;
    std::cout << chemin.path_th2.vel << std::endl;
    std::cout << chemin.path_th2.acc << std::endl;
}

void loop()
{
 	
}