#include "instructions.h"
#include <vector>
#include "../common/SerialTalks.h"
#include "../common/IK/ArmManager.h"
#include "../common/IK/TrajectoryManager.h"

using namespace IK;

extern TrajectoryManager traj_manager;

void MOVE_DIRECTLY(SerialTalks& talks, Deserializer& input, Serializer& output)
{
}


void MOVE_PATH(SerialTalks& talks, Deserializer& input, Serializer& output)
{
    
}

void IS_ARRIVED(SerialTalks& talks, Deserializer& input, Serializer& output)
{
    int ret = (int)traj_manager.get_status();
    output.write<int>(ret);
}

void GO_HOME(SerialTalks& talks, Deserializer& input, Serializer& output)
{
}