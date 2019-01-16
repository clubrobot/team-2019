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
TaskManager       task_manager;

AX12 servoax1;
AX12 servoax2;
AX12 servoax3;

MotorWrapper AX1;
MotorWrapper AX2;
MotorWrapper AX3;

PID AX1_PID;
PID AX2_PID;
PID AX3_PID;


static void secondary_loop(void * parameters)
{
    while(1)
    {
        traj_manager.update();
    }
}

void setup()
{
    /* init Serial talks */
    Serial.begin(SERIALTALKS_BAUDRATE);
    talks.begin(Serial);

    joints_t joint;

    /* init Motors */
    AX12::SerialBegin(1000000, 5);

    try
    {
        servoax1.attach(ID1);
        servoax2.attach(ID2);
        servoax3.attach(ID3);
    }
    catch(...)
    {
        cout << "err" <<endl;
    }

    joint.th1 = (servoax1.readPosition() - LINK1_OFFSET)*M_PI / 180;
    joint.th2 = (servoax2.readPosition() - LINK2_OFFSET)*M_PI / 180;
    joint.th3 = (servoax3.readPosition() - LINK3_OFFSET)*M_PI / 180;

    cout << joint << endl;
    AX1_PID.setOutputLimits(1, 532);
    AX1_PID.setTunings(1, 0.01 , 0);

    AX2_PID.setOutputLimits(1, 532);
    AX2_PID.setTunings(1, 0.01 , 0);

    AX3_PID.setOutputLimits(1, 532);
    AX3_PID.setTunings(1, 0.01 , 0);

    AX1.setPID(AX1_PID);
    AX1.setMOTOR(servoax1);
    AX1.setOFFSET(LINK1_OFFSET);

    AX2.setPID(AX2_PID);
    AX2.setMOTOR(servoax2);
    AX2.setOFFSET(LINK2_OFFSET);

    AX3.setPID(AX3_PID);
    AX3.setMOTOR(servoax3);
    AX3.setOFFSET(LINK3_OFFSET);

    /* init Arm Manager */
    arm_manager.init_workspace(WS_FRONT, WS_BACK);                              /*      init workspaces      */
    arm_manager.set_origin(ORIGIN);                                             /*      set arm origin       */
    arm_manager.set_initial_joint_pos(joint);                                   
    arm_manager.init_arm(LINK1_LEN, LINK2_LEN, LINK3_LEN, FLIP_ELBOW_BACK);     /*      init arm at pos      */

    /* init traj Manager */
    traj_manager.set_armManager(arm_manager);
    traj_manager.set_Motors(AX1, AX2, AX3);
    traj_manager.setTimestep(DELTA_T);

    /* Add INITIAL_POS to queue */
    traj_manager.move_directly(arm_positions[HOME]);
    traj_manager.move_directly(arm_positions[PUCK_POS_INTER]);
    traj_manager.move_directly(arm_positions[PUCK_POS]);
    traj_manager.move_directly(arm_positions[PUCK_POS_INTER_1]);
    traj_manager.move_directly(arm_positions[TANK_POS_INTER]);
    traj_manager.move_directly(arm_positions[TANK_POS_INTER_2]);
    traj_manager.move_directly(arm_positions[TANK_POS_0]);

    traj_manager.move_directly(arm_positions[PUCK_POS_INTER]);
    traj_manager.move_directly(arm_positions[PUCK_POS]);
    traj_manager.move_directly(arm_positions[PUCK_POS_INTER_1]);
    traj_manager.move_directly(arm_positions[TANK_POS_INTER]);
    traj_manager.move_directly(arm_positions[TANK_POS_INTER_2]);
    traj_manager.move_directly(arm_positions[TANK_POS_1]);

    traj_manager.move_directly(arm_positions[PUCK_POS_INTER]);
    traj_manager.move_directly(arm_positions[PUCK_POS]);
    traj_manager.move_directly(arm_positions[PUCK_POS_INTER_1]);
    traj_manager.move_directly(arm_positions[TANK_POS_INTER]);
    traj_manager.move_directly(arm_positions[TANK_POS_INTER_2]);
    traj_manager.move_directly(arm_positions[TANK_POS_2]);
    
    
    traj_manager.enable();
    /* create secondary loop */
    task_manager.create_task(secondary_loop , NULL);
    
}

void loop()
{  
    talks.execute();
}
