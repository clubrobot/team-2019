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

TrajectoryManager traj_manager(0.5);

void setup()
{
    /* init Serial talks */
    Serial.begin(SERIALTALKS_BAUDRATE);
    talks.begin(Serial);

    joints_t joint = {0,0,0};
    coords_t  tool = {0,0,0};

    /* front worksapce coordinate */
    workspace_t ws_front = {0, 20.5, -5, 20.5, -1.0};

    /* back worksapce coordinate */
    workspace_t ws_back = {-20.5, 0, -5, 20.5, 1.0};

    /* init TrajectoryManager */

    traj_manager.init_workspace(ws_front, ws_back);                         /*      init workspaces      */
    traj_manager.set_origin(ORIGIN);                                        /*      set arm origin       */
    traj_manager.attach(ID1, ID2, ID3, LINK1_LEN, LINK2_LEN, LINK3_LEN);    /*      attach ax12 motors   */
    traj_manager.init_arm(INITIAL_POS,FLIP_ELBOW_FRONT);                    /*      init arm at pos      */

    double t;
    try
    {
        t = traj_manager.goto_path(10,10,0);
        cout << "trajectory time : "<< t << endl;
    }
    catch(const string& err)
    {
        cout << "error : " << err << endl;
    }
}

void loop()
{  
    talks.execute();
}
