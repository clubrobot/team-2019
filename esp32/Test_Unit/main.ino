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

ArmManager        arm_manager;
TrajectoryManager traj_manager;

double x = 10;
double y = 10;

void setup()
{
    /* init Serial talks */
    Serial.begin(SERIALTALKS_BAUDRATE);
    talks.begin(Serial);

    /* init Arm Manager */
    arm_manager.init_workspace(WS_FRONT, WS_BACK);                              /*      init workspaces      */
    arm_manager.set_origin(ORIGIN);                                             /*      set arm origin       */
    arm_manager.init_arm(LINK1_LEN, LINK2_LEN, LINK3_LEN, FLIP_ELBOW_BACK);     /*      init arm at pos      */

    /* init Motors */

    /* init traj Manager */
    traj_manager.set_armManager(arm_manager);

    traj_manager.move_directly({10,15,0});
    traj_manager.move_directly({-10,15,M_PI});
}

void loop()
{  
    talks.execute();
    traj_manager.update();
}
