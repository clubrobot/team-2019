#include <Arduino.h>
#include <tuple>
#include <iostream>
#include <string>
#include <stdexcept>
#include "../common/SerialTalks.h"
#include "instructions.h"
#include "../common/IK/Picker.h"
#include "../common/IK/ArmManager.h"
#include "../common/IK/TrajectoryManager.h"

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

coords_t origin = {1.0,0.0,0.0};
coords_t tool;
coords_t coord = {0.0,0.0,0.0};
coords_t vel = {0.0,0.0,0.0};
joints_t joints = {0.0,0.0,0.0};

path_t chemin;

ArmManager arm_manager;
TrajectoryManager traj_manager;

void setup()
{
    Serial.begin(SERIALTALKS_BAUDRATE);
    talks.begin(Serial);

    workspace_t ws_side = {-10,
                        10,
                        abs(10 - 10),
                        abs(10 + 10),
                        1.0};

    workspace_t ws_front = {-abs(10 + 10),
                        -abs(10 - 10),
                        -10,
                        10,
                        1.0};

    arm_manager.init(ws_side, ws_front, 0.2);

    

    traj_manager.goto_path(5.0,5.0,0.0);


    traj_manager.goto_directly(9.0,5.0,0.0);


    //tool = arm_manager.m_arm.get_tool();

    // tool.x   = 5.0;
    // tool.y   = 5.0;
    // tool.phi = 0.0;

    // coord = arm_manager.m_arm.get_tool();

    // chemin = arm_manager.go_to(coord, vel , tool , vel);
    //chemin = arm_manager.goto_workspace(coord, vel , tool , vel, ws_side);
    //chemin = arm_manager.goto_position(coord, vel , tool , vel);
    //chemin = arm.get_path(coord, vel , tool , vel, 0.2);


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
    talks.execute();
}