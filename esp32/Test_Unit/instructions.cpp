#include "instructions.h"
#include <vector>
#include "../common/SerialTalks.h"
#include "../common/IK/ArmManager.h"
#include "../common/IK/TrajectoryManager.h"

using namespace IK;

extern TrajectoryManager traj_manager;

void ADD_MOVE_POSITION(SerialTalks& talks, Deserializer& input, Serializer& output)
{
    int pos_index = input.read<int>();

    traj_manager.move_directly(arm_positions[pos_index]);
}

void RUN_BATCH(SerialTalks& talks, Deserializer& input, Serializer& output)
{
    traj_manager.enable();
}

void IS_ARRIVED(SerialTalks& talks, Deserializer& input, Serializer& output)
{
    traj_manager.is_arrived();
}