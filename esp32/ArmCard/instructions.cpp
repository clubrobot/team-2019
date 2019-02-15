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
    int ret = traj_manager.is_arrived();
    output.write<int>(ret);
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