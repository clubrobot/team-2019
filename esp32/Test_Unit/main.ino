#include <Arduino.h>
#include <tuple>
#include <iostream>
#include <string>
#include <stdexcept>
#include "../common/SerialTalks.h"
#include "instructions.h"
#include "../common/IK/Joint.h"
#include "../common/IK/Picker.h"
#include "../common/IK/ArmManager.h"
#include "../common/IK/TrajectoryManager.h"
#include "arm_config.h"

using namespace IK;
using namespace std;

TrajectoryManager traj_manager;

double x = 10;
double y = 10;

void setup()
{
    /* init Serial talks */
    Serial.begin(SERIALTALKS_BAUDRATE);
    talks.begin(Serial);

    talks.bind(MOVE_DIRECTLY_OPCODE, MOVE_DIRECTLY);
    talks.bind(IS_ARRIVED_OPCODE, IS_ARRIVED);

    /* init TrajectoryManager */
    traj_manager.init_workspace(WS_FRONT, WS_BACK);                             /*      init workspaces      */
    traj_manager.set_origin(ORIGIN);                                            /*      set arm origin       */
    traj_manager.init_arm(LINK1_LEN, LINK2_LEN, LINK3_LEN, FLIP_ELBOW_BACK);    /*      init arm at pos      */
    traj_manager.attach(ID1, ID2, ID3);                                         /*      attach ax12 motors   */
    traj_manager.begin(INITIAL_POS); 

    while(traj_manager.get_status() != ARRIVED);
    // try
    // {
    //     traj_manager.goto_directly(arm_positions[HOME]);
    // }
    // catch(const string& err)
    // {
        
    // }
    // while(traj_manager.get_status() != ARRIVED);
    // delay(3000);
    // try
    // {
    //     traj_manager.goto_directly(arm_positions[PUCK_POS_INTER]);
    // }
    // catch(const string& err)
    // {
        
    // }
    // while(traj_manager.get_status() != ARRIVED);
    // delay(3000);
    // try
    // {
    //     traj_manager.goto_directly(arm_positions[PUCK_POS]);
    // }
    // catch(const string& err)
    // {
        
    // }
    // while(traj_manager.get_status() != ARRIVED);
    // delay(3000);
    // try
    // {
    //     traj_manager.goto_directly(arm_positions[PUCK_POS_INTER]);
    // }
    // catch(const string& err)
    // {
        
    // }
    // while(traj_manager.get_status() != ARRIVED);

    // delay(3000);
    // try
    // {
    //     traj_manager.goto_directly(arm_positions[TANK_POS_INTER]);
    // }
    // catch(const string& err)
    // {
        
    // }
    // while(traj_manager.get_status() != ARRIVED);
    // delay(3000);
    // try
    // {
    //     traj_manager.goto_directly(arm_positions[TANK_POS_2]);
    // }
    // catch(const string& err)
    // {
        
    // }
    // while(traj_manager.get_status() != ARRIVED);
    // delay(3000);
    // try
    // {
    //     traj_manager.goto_directly(arm_positions[TANK_POS_1]);
    // }
    // catch(const string& err)
    // {
        
    // }
    // while(traj_manager.get_status() != ARRIVED);
    // delay(3000);
    // try
    // {
    //     traj_manager.goto_directly(arm_positions[TANK_POS_0]);
    // }
    // catch(const string& err)
    // {
        
    // }
    // while(traj_manager.get_status() != ARRIVED);
    // delay(3000);
}

void loop()
{  
    talks.execute();
}
