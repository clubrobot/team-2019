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

void setup()
{
    /* init Serial talks */
    Serial.begin(SERIALTALKS_BAUDRATE);
    talks.begin(Serial);

    /* init TrajectoryManager */
    traj_manager.init_workspace(WS_FRONT, WS_BACK);                         /*      init workspaces      */
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
