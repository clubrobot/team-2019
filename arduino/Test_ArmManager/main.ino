#include <Arduino.h>
#include "../common/SerialTalks.h"
#include "instructions.h"
#include "../common/IK/Picker.h"
#include "../common/IK/ArmManager.h"
#include "../common/IK/Arm.h"
#include "../common/Vector.h"

template<class T> inline Print &operator <<(Print &obj, T arg) { obj.print(arg); return obj; }

/* debug */
// template<typename T>
// Print& operator<< (Print& out, const Vector<T>& v) {
//     out << "{";
//     int last = v.size() - 1;
//     for(int i = 0; i < v.size(); ++i) {
//         out << v[i];
//         if (i != last) 
//             out << ", ";
//     }
//     out << "}";
//     return out;
// }

coords_t tool = {0.0,0.0,0.0};
coords_t coord = {0.0,0.0,0.0};
coords_t vel = {0.0,0.0,0.0};
joints_t joints = {0.0,0.0,0.0};

path_t chemin;

Picker arm(10,10,5,joints,tool);

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

    ArmManager arm_manager(ws_side, ws_front, 0.2);

    // tool = arm_manager.m_arm->get_tool();

    // coord.x   = 0;
    // coord.y   = 0;
    // coord.phi = 0;

    // vel.x   = 0;
    // vel.y   = 0;
    // vel.phi = 0;

    // tool.x   = 5.0;
    // tool.y   = 5.0;
    // tool.phi = 0.0;

    // coord = arm_manager.m_arm->get_tool();

    // chemin = arm_manager.go_to(coord, vel , tool , vel);
    //chemin = arm_manager.goto_workspace(coord, vel , tool , vel, ws_side);
    //chemin = arm_manager.goto_position(coord, vel , tool , vel);
    //chemin = arm.get_path(coord, vel , tool , vel, 0.2);


    Serial << "t : " ;//<< chemin.path_th1.t;
    // Serial << "pos : " << chemin.path_th1.pos;
    // Serial << "vel : " << chemin.path_th1.vel;

    // Serial << "t : " << chemin.path_th2.t;
    // Serial << "pos : " << chemin.path_th2.pos;
    // Serial << "vel : " << chemin.path_th2.vel;

    // Serial << "t : " << chemin.path_th3.t;
    // Serial << "pos : " << chemin.path_th3.pos;
    // Serial << "vel : " << chemin.path_th3.vel;

}

void loop()
{  
    talks.execute();
}