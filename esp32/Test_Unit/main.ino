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

TrajectoryManager traj_manager(DELTA_T);

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

    double trajectory_time;
    try
    {
        trajectory_time = traj_manager.goto_path(10, 10, 0);
    }
    catch(const string& err)
    {
        cout << err << endl;
    }
    delay(trajectory_time * 1000);

    try
    {
        trajectory_time = traj_manager.goto_path(10, 15, 0);
    }
    catch(const string& err)
    {
        cout << err << endl;
    }
    delay(trajectory_time * 1000);

    try
    {
        trajectory_time = traj_manager.goto_path(15, 15, 0);
    }
    catch(const string& err)
    {
        cout << err << endl;
    }
    delay(trajectory_time * 1000);

    try
    {
        trajectory_time = traj_manager.goto_path(10, 10, 0);
    }
    catch(const string& err)
    {
        cout << err << endl;
    }
    delay(trajectory_time * 1000);

    try
    {
        trajectory_time = traj_manager.goto_path(5, 15,0);
    }
    catch(const string& err)
    {
        cout << err << endl;
    }
    delay(trajectory_time * 1000);
}
double trajectory_time;
void loop()
{  

    try
    {
        trajectory_time = traj_manager.goto_path(x, y,0);
    }
    catch(const string& err)
    {
        cout << err << endl;
    }

    if(x == 15)
    {
        x = 8;
    }
    else
    {
        x++;
    }
    if(y == 15)
    {
        y = 8;
    }
    else
    {
        y++;
    }
    delay(trajectory_time * 1000);
    talks.execute();
}
