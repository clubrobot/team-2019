#include "instructions.h"
#include <vector>
#include "../common/SerialTalks.h"
#include "../common/IK/ArmManager.h"
#include "../common/IK/TrajectoryManager.h"

using namespace IK;

extern TrajectoryManager traj_manager;
extern coords_t arm_positions[];

void MOVE_DIRECTLY(SerialTalks& talks, Deserializer& input, Serializer& output)
{
    
}

void IS_ARRIVED(SerialTalks& talks, Deserializer& input, Serializer& output)
{
}