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
    position_t pos_id = (position_t)input.read<int>();
    try
    {
        traj_manager.goto_directly(arm_positions[pos_id]);
    }
    catch(const string& err)
    {
        cout << err << endl;
    }
}

void IS_ARRIVED(SerialTalks& talks, Deserializer& input, Serializer& output)
{
    int ret = (int)traj_manager.get_status();
    output.write<int>(ret);
}