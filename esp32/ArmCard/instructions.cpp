#include "instructions.h"
#include <vector>
#include "../common/SerialTalks.h"
#include "../common/IK/ArmManager.h"
#include "../common/IK/TrajectoryManager.h"

using namespace IK;

extern VacumPump pump;

extern ArmManager        arm_manager;
extern TrajectoryManager traj_manager;

extern MotorWrapper AX1;
extern MotorWrapper AX2;
extern MotorWrapper AX3;

void ARM_BEGIN(SerialTalks& talks, Deserializer& input, Serializer& output)
{
    traj_manager.begin();
}

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
        output.write<byte>(1);
    }
    output.write<byte>(0);
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
    output.write<byte>(ret);
    output.write<byte>(0);
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

void SET_MOTORS_ID(SerialTalks& talks, Deserializer& input, Serializer& output)
{
    float id1     = input.read<int>();
    float id2     = input.read<int>();
    float id3     = input.read<int>();

    AX1.attach(id1);
    AX2.attach(id2);
    AX3.attach(id3);
}

void SET_MOTORS_OFFSET(SerialTalks& talks, Deserializer& input, Serializer& output)
{
    float off1     = input.read<float>();
    float off2     = input.read<float>();
    float off3     = input.read<float>();

    AX1.setOFFSET(off1);
    AX2.setOFFSET(off2);
    AX3.setOFFSET(off3);
}

void SET_WORKSPACE(SerialTalks& talks, Deserializer& input, Serializer& output)
{
    workspace_t front;
    workspace_t back;

    front.x_min             = input.read<float>();
    front.x_max             = input.read<float>();
    front.y_min             = input.read<float>();
    front.y_max             = input.read<float>();
    front.elbow_orientation = input.read<int>();

    back.x_min              = input.read<float>();
    back.x_max              = input.read<float>();
    back.y_min              = input.read<float>();
    back.y_max              = input.read<float>();
    back.elbow_orientation  = input.read<int>();

    arm_manager.init_workspace(front, back);

}

void SET_ORIGIN(SerialTalks& talks, Deserializer& input, Serializer& output)
{
    coords_t origin;
    origin.x   = input.read<float>();
    origin.y   = input.read<float>();
    origin.phi = input.read<float>();

    arm_manager.set_origin(origin);
}

void SET_LINK_LEN(SerialTalks& talks, Deserializer& input, Serializer& output)
{
    float link1     = input.read<float>();
    float link2     = input.read<float>();
    float link3     = input.read<float>();
    int   elbow     = input.read<int>();

    arm_manager.set_arm_link(link1, link2, link3, elbow);
}