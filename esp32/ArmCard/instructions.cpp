#include "instructions.h"
#include <vector>
#include "../common/SerialTalks.h"
#include "../common/IK/ArmManager.h"
#include "../common/IK/TrajectoryManager.h"

using namespace IK;

extern TrajectoryManager traj_manager;
extern VacumPump pump;

void ADD_MOVE(SerialTalks& talks, Deserializer& input, Serializer& output)
{
    coords_t pos;
    pos.x   = input.read<float>();
    pos.y   = input.read<float>();
    pos.phi = input.read<float>();

    try
    {
        traj_manager.move_directly(pos);
    }
    catch(...)
    {

    }
}

void RUN_BATCH(SerialTalks& talks, Deserializer& input, Serializer& output)
{
    traj_manager.enable();
}

void STOP_BATCH(SerialTalks& talks, Deserializer& input, Serializer& output)
{
    traj_manager.disable();
}

void IS_ARRIVED(SerialTalks& talks, Deserializer& input, Serializer& output)
{
    byte ret = traj_manager.is_arrived();
    byte err = traj_manager.error();
    output.write<byte>(ret);
    output.write<byte>(err);
}

void GET_MOTORS_STATE(SerialTalks& talks, Deserializer& input, Serializer& output)
{
    Motor_state_t m1 = traj_manager.get_motor1_state();
    Motor_state_t m2 = traj_manager.get_motor2_state();
    Motor_state_t m3 = traj_manager.get_motor3_state();

    output.write<byte>(m1.id);
    output.write<byte>(m1.timeout);
    output.write<byte>(m1.err_code);

    output.write<byte>(m2.id);
    output.write<byte>(m2.timeout);
    output.write<byte>(m2.err_code);

    output.write<byte>(m3.id);
    output.write<byte>(m3.timeout);
    output.write<byte>(m3.err_code);
}

void START_PUMP(SerialTalks& talks, Deserializer& input, Serializer& output)
{
    pump.startPump();
}

void STOP_PUMP(SerialTalks& talks, Deserializer& input, Serializer& output)
{
    pump.stopPump();
}

void START_SLUICE(SerialTalks& talks, Deserializer& input, Serializer& output)
{
    pump.startSluice();
}

void STOP_SLUICE(SerialTalks& talks, Deserializer& input, Serializer& output)
{
    pump.stopSluice();
}

void CLEAR_MOTOR_ERR(SerialTalks& talks, Deserializer& input, Serializer& output)
{
    int id = input.read<int>();
    if(id == 1)
    {
        traj_manager.clear_motor1_error();
    }
    if(id == 2)
    {
        traj_manager.clear_motor2_error();
    }
    if(id == 3)
    {
        traj_manager.clear_motor3_error();
    }

}