#include <Arduino.h>
#include "../common/SerialTalks.h"
#include "instructions.h"
#include "../common/IK/Joint.h"
#include "../common/IK/Picker.h"
#include "../common/IK/ArmManager.h"
#include "../common/IK/TrajectoryManager.h"
#include "../common/TaskManager.h"
#include "../common/VacumPump.h"
#include "arm_config.h"
#include "arm_position.h"

using namespace IK;
using namespace std;

VacumPump pump(27, 14);

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

static void secondary_loop(void * parameters);

void setup()
{
    /* init Serial talks */
    Serial.begin(SERIALTALKS_BAUDRATE);
    talks.begin(Serial);

    talks.bind(ADD_MOVE_OPCODE,   ADD_MOVE);
    talks.bind(RUN_BATCH_OPCODE,  RUN_BATCH);
    talks.bind(STOP_BATCH_OPCODE, STOP_BATCH);
    talks.bind(IS_ARRIVED_OPCODE, IS_ARRIVED);

    talks.bind(START_PUMP_OPCODE,  START_PUMP);
    talks.bind(STOP_PUMP_OPCODE,   STOP_PUMP);
    talks.bind(START_SLUICE_OPCODE,START_SLUICE);
    talks.bind(STOP_SLUICE_OPCODE, STOP_SLUICE);

    joints_t joint;

    /* init Motors communication */
    AX12::SerialBegin(1000000, 5);

    servoax1.attach(ID1);
    servoax2.attach(ID2);
    servoax3.attach(ID3);

    /* get current motor pos to correctly init arm */
    try
    {
        joint.th1 = (servoax1.readPosition() - LINK1_OFFSET)*M_PI / 180;
        joint.th2 = (servoax2.readPosition() - LINK2_OFFSET)*M_PI / 180;
        joint.th3 = (servoax3.readPosition() - LINK3_OFFSET)*M_PI / 180;
    }
    catch(...)
    {
        joint = {10,10,0};
    }

    /* configure PID for motor 1*/
    AX1_PID.setOutputLimits(1, 532);
    AX1_PID.setTunings(1, 0.01 , 0);

    /* configure PID for motor 2*/
    AX2_PID.setOutputLimits(1, 532);
    AX2_PID.setTunings(1, 0.01 , 0);

    /* configure PID for motor 3*/
    AX3_PID.setOutputLimits(1, 532);
    AX3_PID.setTunings(1, 0.01 , 0);

    /* configure MotorWrapper 1*/
    AX1.setPID(AX1_PID);
    AX1.setMOTOR(servoax1);
    AX1.setOFFSET(LINK1_OFFSET);

    /* configure MotorWrapper 2*/
    AX2.setPID(AX2_PID);
    AX2.setMOTOR(servoax2);
    AX2.setOFFSET(LINK2_OFFSET);

    /* configure MotorWrapper 3*/
    AX3.setPID(AX3_PID);
    AX3.setMOTOR(servoax3);
    AX3.setOFFSET(LINK3_OFFSET);

    /* init Arm Manager */
    arm_manager.init_workspace(WS_FRONT, WS_BACK);                              /*      init workspaces      */
    arm_manager.set_origin(ORIGIN);                                             /*      set arm origin       */
    arm_manager.set_initial_joint_pos(joint);                                   /*      initial joints pos   */
    arm_manager.init_arm(LINK1_LEN, LINK2_LEN, LINK3_LEN, FLIP_ELBOW_BACK);     /*      init arm at pos      */

    /* init traj Manager */
    traj_manager.set_armManager(arm_manager);
    traj_manager.set_Motors(AX1, AX2, AX3);
    traj_manager.setTimestep(DELTA_T);

    /* Add INITIAL_POS to queue */
    traj_manager.move_directly(arm_positions[HOME]);
    

    /* enable traj manager to reach pos */    
    traj_manager.enable();

    /* create secondary loop to manage arm deplacements*/
    task_manager.create_task(secondary_loop , NULL);
}

void loop()
{
    talks.execute();
    vTaskDelay(1);
}

static void secondary_loop(void * parameters)
{
    while(1)
    {
        traj_manager.update();
        vTaskDelay(1);
    }
}
