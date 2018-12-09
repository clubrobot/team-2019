#include "instructions.h"
#include <vector>
#include "../common/SerialTalks.h"
#include "../common/IK/ArmManager.h"
#include "../common/IK/TrajectoryManager.h"

using namespace IK;

extern TrajectoryManager traj_manager;

void MOVE_DIRECTLY(SerialTalks& talks, Deserializer& input, Serializer& output)
{
    double trajectory_time, x, y, phi;

    x   = (double)input.read<float>();
    y   = (double)input.read<float>();
    phi = (double)input.read<float>();

    try
    {
        trajectory_time = traj_manager.goto_directly(x, y, phi);
    }
    catch(const string& err)
    {
        trajectory_time = -1;
    }


    output.write<float>((float)trajectory_time);
}


void MOVE_PATH(SerialTalks& talks, Deserializer& input, Serializer& output)
{
    double trajectory_time, x, y, phi;

    x   = (double)input.read<float>();
    y   = (double)input.read<float>();
    phi = (double)input.read<float>();

    try
    {
        trajectory_time = traj_manager.goto_path(x, y, phi);
    }
    catch(const string& err)
    {
        trajectory_time = -1;
    }


    output.write<float>((float)trajectory_time);
}

void IS_ARRIVED(SerialTalks& talks, Deserializer& input, Serializer& output)
{
    int ret = (int)traj_manager.get_status();
    output.write<int>(12);
}

void GO_HOME(SerialTalks& talks, Deserializer& input, Serializer& output)
{
    traj_manager.goto_home();
}