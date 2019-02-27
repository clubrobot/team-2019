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
#include "PIN.h"

using namespace IK;
using namespace std;

VacumPump pump(VACCUM_PIN, SLUICE_PIN);

ArmManager        arm_manager;
TrajectoryManager traj_manager;
TaskManager       task_manager;

MotorWrapper AX1;
MotorWrapper AX2;
MotorWrapper AX3;

static void secondary_loop(void * parameters);

void setup()
{
    /* init Serial talks */
    Serial.begin(SERIALTALKS_BAUDRATE);
    talks.begin(Serial);

    talks.bind(ADD_MOVE_OPCODE          , ADD_MOVE);
    talks.bind(ARM_BEGIN_OPCODE         , ARM_BEGIN);
    talks.bind(ADD_MOVE_OPCODE          , ADD_MOVE);
    talks.bind(RUN_BATCH_OPCODE         , RUN_BATCH);
    talks.bind(STOP_BATCH_OPCODE        , STOP_BATCH);
    talks.bind(IS_ARRIVED_OPCODE        , IS_ARRIVED);

    talks.bind(START_PUMP_OPCODE        , START_PUMP);
    talks.bind(STOP_PUMP_OPCODE         , STOP_PUMP);
    talks.bind(START_SLUICE_OPCODE      , START_SLUICE);
    talks.bind(STOP_SLUICE_OPCODE       , STOP_SLUICE);

    talks.bind(SET_MOTORS_ID_OPCODE     , SET_MOTORS_ID);
    talks.bind(SET_MOTORS_OFFSET_OPCODE , SET_MOTORS_OFFSET);
    talks.bind(SET_WORKSPACE_OPCODE     , SET_WORKSPACE);
    talks.bind(SET_ORIGIN_OPCODE        , SET_ORIGIN);
    talks.bind(SET_LINK_LEN_OPCODE      , SET_LINK_LEN);

    /* init Motors communication */
    AX12::SerialBegin(AX12_SPEED, AX12_PIN);

    /* configure MotorWrapper 1*/
    AX1.attach(ID1_DEFAULT);
    AX1.setOFFSET(LINK1_OFFSET_DEFAULT);
    /* configure MotorWrapper 2*/
    AX2.attach(ID2_DEFAULT);
    AX2.setOFFSET(LINK2_OFFSET_DEFAULT);

    /* configure MotorWrapper 3*/
    AX3.attach(ID3_DEFAULT);
    AX3.setOFFSET(LINK3_OFFSET_DEFAULT);

    /* init Arm Manager */
    arm_manager.init_workspace(WS_FRONT_DEFAULT, WS_BACK_DEFAULT);                                      /*      init workspaces      */
    arm_manager.set_origin(ORIGIN_DEFAULT);                                                             /*      set arm origin       */
    arm_manager.set_initial_joint_pos(INITIAL_JOINTS_DEFAULT);                                          /*      initial joints pos   */
    arm_manager.set_arm_link(LINK1_LEN_DEFAULT, LINK2_LEN_DEFAULT, LINK3_LEN_DEFAULT, FLIP_ELBOW_BACK);

    /* init traj Manager */
    traj_manager.set_armManager(arm_manager);
    traj_manager.set_Motors(AX1, AX2, AX3);
    traj_manager.setTimestep(DELTA_T);

    /* create secondary loop to manage arm deplacements*/
    task_manager.create_task(secondary_loop , NULL);
}

void loop()
{
    talks.execute();
    vTaskDelay(5 / portTICK_PERIOD_MS);
}

static void secondary_loop(void * parameters)
{
    while(1)
    {
        traj_manager.update();
        vTaskDelay(5 / portTICK_PERIOD_MS);
    }
}
