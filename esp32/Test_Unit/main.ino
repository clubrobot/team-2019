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

typedef enum
{
    HOME,
    PUCK_POS,
    PUCK_POS_INTER,
    TANK_POS_INTER,
    MID_POS,
    TANK_POS_0,
    TANK_POS_1,
    TANK_POS_2,
    BALANCE_POS_INTER,
    BALANCE_POS,
}position_t;

static coords_t arm_positions[] = 
{
    [HOME]              = {10.0, 15.0, 0},
    [PUCK_POS]          = {-12.0, 15.0, M_PI},
    [PUCK_POS_INTER]    = {-8.0, 15.0, M_PI},
    [TANK_POS_INTER]    = {10.0, 8.0, -M_PI/2},
    [MID_POS]           = {0.0, 20.0, 0},
    [TANK_POS_0]        = {10.0, 0.0, -M_PI/2},
    [TANK_POS_1]        = {10.0, 2.0, -M_PI/2},
    [TANK_POS_2]        = {10.0, 4.0, -M_PI/2},
    [BALANCE_POS_INTER] = {10.0, 5.0, 0},
    [BALANCE_POS]       = {10.0, 5.0, 0},
};

TrajectoryManager traj_manager;

double x = 10;
double y = 10;

void setup()
{
    /* init Serial talks */
    Serial.begin(SERIALTALKS_BAUDRATE);
    talks.begin(Serial);

    talks.bind(MOVE_DIRECTLY_OPCODE, MOVE_DIRECTLY);
    talks.bind(MOVE_PATH_OPCODE, MOVE_PATH);
    talks.bind(IS_ARRIVED_OPCODE, IS_ARRIVED);
    talks.bind(GO_HOME_OPCODE, GO_HOME);

    /* init TrajectoryManager */
    traj_manager.init_workspace(WS_FRONT, WS_BACK);                             /*      init workspaces      */
    traj_manager.set_origin(ORIGIN);                                            /*      set arm origin       */
    traj_manager.init_arm(LINK1_LEN, LINK2_LEN, LINK3_LEN, FLIP_ELBOW_BACK);    /*      init arm at pos      */
    traj_manager.attach(ID1, ID2, ID3);                                         /*      attach ax12 motors   */
    traj_manager.begin(INITIAL_POS); 

    delay(2000);
    try
    {
        traj_manager.goto_directly(arm_positions[HOME]);
    }
    catch(const string& err)
    {
        cout << err << endl;
    }
    delay(2000);

    try
    {
        traj_manager.goto_directly(arm_positions[PUCK_POS_INTER]);
    }
    catch(const string& err)
    {
        cout << err << endl;
    }
    delay(2000);

    try
    {
        traj_manager.goto_directly(arm_positions[PUCK_POS]);
    }
    catch(const string& err)
    {
        cout << err << endl;
    }
    delay(2000);

    try
    {
        traj_manager.goto_directly(arm_positions[PUCK_POS_INTER]);
    }
    catch(const string& err)
    {
        cout << err << endl;
    }
    delay(2000);

    try
    {
        traj_manager.goto_directly(arm_positions[MID_POS]);
    }
    catch(const string& err)
    {
        cout << err << endl;
    }
    delay(2000);

    try
    {
        traj_manager.goto_directly(arm_positions[TANK_POS_INTER]);
    }
    catch(const string& err)
    {
        cout << err << endl;
    }
    delay(2000);

    try
    {
        traj_manager.goto_directly(arm_positions[TANK_POS_2]);
    }
    catch(const string& err)
    {
        cout << err << endl;
    }
    delay(2000);

    try
    {
        traj_manager.goto_directly(arm_positions[TANK_POS_1]);
    }
    catch(const string& err)
    {
        cout << err << endl;
    }
    delay(2000);

    try
    {
        traj_manager.goto_directly(arm_positions[TANK_POS_0]);
    }
    catch(const string& err)
    {
        cout << err << endl;
    }
    delay(2000);

}

void loop()
{  
    talks.execute();
}
