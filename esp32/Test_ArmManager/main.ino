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

#define LINK1_LEN 10.0
#define LINK2_LEN 10.0
#define LINK3_LEN 5.0

#define ORIGIN_X 0.0
#define ORIGIN_Y 0.0
#define ORIGIN_PHI 0.0

#define ID1 1
#define ID2 2
#define ID3 3

#define USE_SHIFTREG 1

#define LATCHPIN	10
#define CLOCKPIN	12
#define DATAPIN		5 

/* debug */
template<typename T> ostream& operator<< (ostream& out, const vector<T>& v);

ShiftRegister shift;

ArmManager arm_manager(0.2);
TrajectoryManager traj_manager;

void setup()
{
    Serial.begin(SERIALTALKS_BAUDRATE);
    talks.begin(Serial);

    talks.bind(MOVE_DIRECTLY_OPCODE, MOVE_DIRECTLY);
    talks.bind(MOVE_PATH_OPCODE, MOVE_PATH);
    talks.bind(IS_ARRIVED_OPCODE, IS_ARRIVED);

    pinMode(LATCHPIN, OUTPUT);
    pinMode(CLOCKPIN, OUTPUT);
    pinMode(DATAPIN , OUTPUT);

    shift.attach(LATCHPIN,CLOCKPIN,DATAPIN);

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

    /* Configure Arm Manager */
    arm_manager.init_workspace(ws_side, ws_front);                      /*      init workspaces      */
    arm_manager.set_origin(ORIGIN_X, ORIGIN_Y, ORIGIN_PHI);             /*      set arm origin       */
    arm_manager.attach(ID1, ID2, ID3, LINK1_LEN, LINK2_LEN, LINK3_LEN); /*      attach ax12 motors   */
    arm_manager.init_arm(5,5,0);                                        /*      init arm at pos      */
    
    /* begin TrajectoryManager */
    // traj_manager.goto_path(5.0,5.0,0.0);

    // traj_manager.goto_directly(9.0,5.0,0.0);

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