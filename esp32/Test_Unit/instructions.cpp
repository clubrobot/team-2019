#include "instructions.h"
#include <vector>
#include "../common/SerialTalks.h"
#include "../common/IK/ArmManager.h"
#include "../common/IK/TrajectoryManager.h"

using namespace IK;

extern TrajectoryManager traj_manager;

void ADD_MOVE(SerialTalks& talks, Deserializer& input, Serializer& output)
{
    coords_t pos;
    pos.x   = input.read<float>();
    pos.y   = input.read<float>();
    pos.phi = input.read<float>();

    traj_manager.move_directly(pos);
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
    traj_manager.is_arrived();
}