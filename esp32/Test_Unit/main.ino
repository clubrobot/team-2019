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

    /* init TrajectoryManager */
    traj_manager.init_workspace(WS_FRONT, WS_BACK);                             /*      init workspaces      */
    traj_manager.set_origin(ORIGIN);                                            /*      set arm origin       */
    traj_manager.init_arm(LINK1_LEN, LINK2_LEN, LINK3_LEN, FLIP_ELBOW_BACK);  /*      init arm at pos      */
    traj_manager.attach(ID1, ID2, ID3);    /*      attach ax12 motors   */
    traj_manager.begin(INITIAL_POS); 

    while(traj_manager.get_status() != ARRIVED);
    double t;
    try
    {
        t = traj_manager.goto_path(15,10,0);
        cout << "trajectory time : "<< t << endl;
    }
    catch(const string& err)
    {
        cout << "error : " << err << endl;
    }

  
}

void loop()
{  
    // while(traj_manager.get_status() != ARRIVED);
    // delay(2000);
    // double t;
    // try
    // {
    //     t = traj_manager.goto_path(15,15,0);
    //     cout << "trajectory time : "<< t << endl;
    // }
    // catch(const string& err)
    // {
    //     cout << "error : " << err << endl;
    // }

    // while(traj_manager.get_status() != ARRIVED);
    // delay(2000);
    //  try
    // {
    //     t = traj_manager.goto_path(10,15,0);
    //     cout << "trajectory time : "<< t << endl;
    // }
    // catch(const string& err)
    // {
    //     cout << "error : " << err << endl;
    // }

  

    // while(traj_manager.get_status() != ARRIVED);
    // delay(2000);
    //  try
    // {
    //     t = traj_manager.goto_path(10,15,0);
    //     cout << "trajectory time : "<< t << endl;
    // }
    // catch(const string& err)
    // {
    //     cout << "error : " << err << endl;
    // }
    
        //traj_manager.motor_read();
        // delay(1000);
        // cout << "....."<<endl;
    //talks.execute();
}
