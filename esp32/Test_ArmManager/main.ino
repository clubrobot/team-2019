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

using namespace IK;

#define LINK1_LEN 10.0
#define LINK2_LEN 10.0
#define LINK3_LEN 3.8

#define ORIGIN_X 0.0
#define ORIGIN_Y 0.0
#define ORIGIN_PHI 0.0

#define ID1 1
#define ID2 3
#define ID3 2

/* debug */
template<typename T> ostream& operator<< (ostream& out, const vector<T>& v);

TrajectoryManager traj_manager;

void setup()
{
    /* init Serial talks */
    Serial.begin(SERIALTALKS_BAUDRATE);
    talks.begin(Serial);

    /* bind methods */
    talks.bind(MOVE_DIRECTLY_OPCODE, MOVE_DIRECTLY);
    talks.bind(MOVE_PATH_OPCODE, MOVE_PATH);
    talks.bind(IS_ARRIVED_OPCODE, IS_ARRIVED);
    talks.bind(GO_HOME_OPCODE, GO_HOME);

    /* front worksapce coordinate */
    workspace_t ws_front = {0, 20.5, 0, 20.5, -1.0};

    /* back worksapce coordinate */
    workspace_t ws_back = {-20.5, 0, 0, 20.5, 1.0};

    /* init TrajectoryManager */
    traj_manager.init_workspace(ws_front, ws_back);                      /*      init workspaces      */
    traj_manager.set_origin(ORIGIN_X, ORIGIN_Y, ORIGIN_PHI);             /*      set arm origin       */
    traj_manager.attach(ID1, ID2, ID3, LINK1_LEN, LINK2_LEN, LINK3_LEN); /*      attach ax12 motors   */
    traj_manager.init_arm(5,5,0,FLIP_ELBOW_FRONT);                       /*      init arm at pos      */

    traj_manager.goto_directly(20.0, 3.8, M_PI/2);
}

void loop()
{  
    talks.execute();
}

template<typename T> ostream& operator<< (ostream& out, const vector<T>& v){
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