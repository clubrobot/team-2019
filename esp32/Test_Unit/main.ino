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

using namespace IK;
using namespace std;

#define LINK1_LEN 10.0
#define LINK2_LEN 10.0
#define LINK3_LEN 3.8

#define ORIGIN_X 0.0
#define ORIGIN_Y 0.0
#define ORIGIN_PHI 0.0

#define ID1 1
#define ID2 3
#define ID3 2

Picker pick;

ArmManager arm;

void setup()
{
    /* init Serial talks */
    Serial.begin(SERIALTALKS_BAUDRATE);
    talks.begin(Serial);

    joints_t joint = {0,0,0};
    coords_t  tool = {0,0,0};

    pick.init(LINK1_LEN, LINK2_LEN, LINK3_LEN, joint, tool, FLIP_ELBOW_FRONT);

     /* front worksapce coordinate */
    workspace_t ws_front = {0, 20.5, -5, 20.5, -1.0};

    /* back worksapce coordinate */
    workspace_t ws_back = {-20.5, 0, -5, 20.5, 1.0};

    /* init TrajectoryManager */

    arm.init_workspace(ws_front, ws_back);                      /*      init workspaces      */
    arm.set_origin(ORIGIN_X, ORIGIN_Y, ORIGIN_PHI);             /*      set arm origin       */
    arm.attach(ID1, ID2, ID3, LINK1_LEN, LINK2_LEN, LINK3_LEN); /*      attach ax12 motors   */
    arm.init_arm(3.8,20,0,FLIP_ELBOW_FRONT);                    /*      init arm at pos      */


    try
    {
       tool = {15,5,0};
       path_t path;
       path = arm.go_to({10,5,0},{0,0,0},{10,6,0},{0,0,0});
       cout << path << endl;

        path = arm.go_to({10,6,0},{0,0,0},{11,6,0},{0,0,0});
        cout << path << endl;

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
