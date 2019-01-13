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

typedef enum
{
    MOVEMENT_CONTROL,
    UPDATE_MOTOR_1,
    UPDATE_MOTOR_2,
    UPDATE_MOTOR_3,
}main_state_machine_t;

static void secondary_loop(void * parameters)
{
    main_state_machine_t state_machine = MOVEMENT_CONTROL;
    while(1)
    {
        switch(state_machine)
        {
            case MOVEMENT_CONTROL:
                traj_manager.update();
                //cout << "MOT1 : " << endl;
                traj_manager.motor1_update();
                //cout << "MOT2 : " << endl;
                traj_manager.motor2_update();
                //cout << "MOT3 : " << endl;
                //traj_manager.motor3_update();
                 //state_machine = UPDATE_MOTOR_1;
                break;

            case UPDATE_MOTOR_1:

                break;

            case UPDATE_MOTOR_2:
                
                break;

            case UPDATE_MOTOR_3:
               
                break;

            default :
                state_machine = MOVEMENT_CONTROL;
                break;
        }
        
    }
}

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
    AX12::SerialBegin(1000000, 5);

    servoax1.attach(ID1);
    servoax2.attach(ID2);
    servoax3.attach(ID3);

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

    /* init traj Manager */
    traj_manager.set_armManager(arm_manager);
    traj_manager.set_Motors(AX1, AX2, AX3);
    traj_manager.setTimestep(DELTA_T);

    /* Add INITIAL_POS to queue */
    traj_manager.move_directly(arm_positions[HOME]);
    traj_manager.move_directly(arm_positions[PUCK_POS_INTER]);
    // traj_manager.move_directly(arm_positions[PUCK_POS]);
    // traj_manager.move_directly(arm_positions[TANK_POS_INTER]);
    // traj_manager.move_directly(arm_positions[TANK_POS_0]);
    // traj_manager.move_directly(arm_positions[HOME]);
    
    
    traj_manager.enable();
    /* create secondary loop */
    task_manager.create_task(secondary_loop , NULL);
    
}

void loop()
{  
    talks.execute();
}
