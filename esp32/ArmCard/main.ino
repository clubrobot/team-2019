#include <Arduino.h>
#include "../common/SerialTalks.h"
#include "instructions.h"
#include "../common/IK/Joint.h"
#include "../common/IK/Picker.h"
#include "../common/IK/ArmManager.h"
#include "../common/IK/TrajectoryManager.h"
#include "../common/TaskManager.h"
#include "../common/VacumPump.h"
#include "addresses.h"
#include "PIN.h"
#include "../common/PressureSensor.h"


using namespace IK;
using namespace std;

VacumPump pump(VACCUM_PIN, SLUICE_PIN);
PressureSensor pSensor = PressureSensor(ANALOG_SENSOR_PIN);

ArmManager        arm_manager;
TrajectoryManager traj_manager;
TaskManager       task_manager;

MotorWrapper AX1;
MotorWrapper AX2;
MotorWrapper AX3;

TickType_t xDelay = 10 / portTICK_PERIOD_MS; // 10 ms task Delay

static void secondary_loop(void * parameters);

void setup()
{
    /* init Serial talks */
    Serial.begin(SERIALTALKS_BAUDRATE);
    talks.begin(Serial);

    talks.bind(ADD_MOVE_OPCODE              , ADD_MOVE);
    talks.bind(ADD_MOVE_OPCODE              , ADD_MOVE);
    talks.bind(RUN_BATCH_OPCODE             , RUN_BATCH);
    talks.bind(STOP_BATCH_OPCODE            , STOP_BATCH);
    talks.bind(IS_ARRIVED_OPCODE            , IS_ARRIVED);

    talks.bind(START_PUMP_OPCODE            , START_PUMP);
    talks.bind(STOP_PUMP_OPCODE             , STOP_PUMP);
    talks.bind(START_SLUICE_OPCODE          , START_SLUICE);
    talks.bind(STOP_SLUICE_OPCODE           , STOP_SLUICE);
    talks.bind(GET_PRESSURE_OPCODE          , GET_PRESSURE);
    talks.bind(ATMOSPHERE_PRESSURE_OPCODE   , ATMOSPHERE_PRESSURE);
    talks.bind(UPDATE_THRESHOLD_OPCODE      , UPDATE_THRESHOLD);
    talks.bind(READ_THRESHOLD_OPCODE        , READ_THRESHOLD);
    talks.bind(SERIALTALKS_DISCONNECT_OPCODE, STOP_ALL);

    talks.bind(SET_PARAMETERS_OPCODE, SET_PARAMETER_VALUE);
    talks.bind(GET_PARAMETERS_OPCODE, GET_PARAMETER_VALUE);

    /* init Motors communication */
    AX12::SerialBegin(AX12_SPEED, AX12_PIN);

    /* configure MotorWrapper 1*/
    AX1.load(MOTOR1_ADDRESS);
    AX1.init();

    /* configure MotorWrapper 2*/
    AX2.load(MOTOR2_ADDRESS);
    AX2.init();

    /* configure MotorWrapper 3*/
    AX3.load(MOTOR3_ADDRESS);
    AX3.init();

    /* init Arm Manager */
    arm_manager.load(ARM_MANAGER_ADDRESS);
    arm_manager.init();

    /* init traj Manager */
    traj_manager.load(TRAJ_MANAGER_ADDRESS);
    traj_manager.set_armManager(arm_manager);
    traj_manager.set_Motors(AX1, AX2, AX3);
    traj_manager.init();
    /* create secondary loop to manage arm deplacements*/
    task_manager.create_task(secondary_loop , NULL);
}

void loop()
{
    talks.execute();
    vTaskDelay( xDelay );       /* include 10 ms delay for better task management by ordonnancer */
}

static void secondary_loop(void * parameters)
{
    while(1)
    {
        traj_manager.update();
        vTaskDelay( xDelay );   /* include 10 ms delay for better task management by ordonnancer */
    }
}
