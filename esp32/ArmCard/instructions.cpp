#include "instructions.h"
#include <vector>
#include "addresses.h"
#include "../common/SerialTalks.h"
#include "../common/IK/ArmManager.h"
#include "../common/IK/TrajectoryManager.h"
#include "../common/PressureSensor.h"

using namespace IK;

extern VacumPump pump;
extern PressureSensor pSensor;

void GET_PRESSURE(SerialTalks& talks, Deserializer& input, Serializer& output){
	output.write<int>(pSensor.getPressurekPa());
}

void ATMOSPHERE_PRESSURE(SerialTalks& talks, Deserializer& input, Serializer& output){
	output.write<bool>(pSensor.currentlyAtmospherePressure());
}

extern ArmManager        arm_manager;
extern TrajectoryManager traj_manager;

extern MotorWrapper AX1;
extern MotorWrapper AX2;
extern MotorWrapper AX3;

void ADD_MOVE(SerialTalks& talks, Deserializer& input, Serializer& output)
{
    Coords pos(input.read<float>(), input.read<float>(), input.read<float>());

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

void SET_PARAMETER_VALUE(SerialTalks& talks, Deserializer& input, Serializer& output)
{
    byte  id = input.read<byte>();
	switch (id)
	{
        case MOTOR1_ID_ID :
            AX1.setID(input.read<int>());
            AX1.save(MOTOR1_ADDRESS);
            break;
        case MOTOR1_OFFSET_ID :
            AX1.setOFFSET(input.read<float>());
            AX1.save(MOTOR1_ADDRESS);
            break;
        case MOTOR2_ID_ID :
            AX2.setID(input.read<int>());
            AX2.save(MOTOR2_ADDRESS);
            break;
        case MOTOR2_OFFSET_ID :
            AX2.setOFFSET(input.read<float>());
            AX2.save(MOTOR2_ADDRESS);
            break;
        case MOTOR3_ID_ID :
            AX3.setID(input.read<int>());
            AX3.save(MOTOR3_ADDRESS);
            break;
        case MOTOR3_OFFSET_ID :
            AX3.setOFFSET(input.read<float>());
            AX3.save(MOTOR3_ADDRESS);
            break;
        case WORKSPACE_FRONT_ID :
            {
            Workspace ws(input.read<float>(),input.read<float>(),input.read<float>(),input.read<float>(),input.read<float>());
            arm_manager.set_workspace(ws, arm_manager.get_workspace_back());
            arm_manager.save(ARM_MANAGER_ADDRESS);
            break;
            }
        case WORKSPACE_BACK_ID :
            {
            Workspace ws(input.read<float>(),input.read<float>(),input.read<float>(),input.read<float>(),input.read<float>());
            arm_manager.set_workspace(arm_manager.get_workspace_front(), ws);
            arm_manager.save(ARM_MANAGER_ADDRESS);
            break;
            }
        case ORIGIN_ID :
            {
            Coords origin(input.read<float>(), input.read<float>(), input.read<float>());
            arm_manager.set_origin(origin);
            arm_manager.save(ARM_MANAGER_ADDRESS);
            break;
            }
        case JOINTS_ID :
            {
            Joints joint(input.read<float>(), input.read<float>(), input.read<float>());
            arm_manager.set_initial_joint_pos(joint);
            arm_manager.save(ARM_MANAGER_ADDRESS);
            }
            break;
        case LINK_1_ID :
            arm_manager.set_arm_link(input.read<float>(), arm_manager.get_link2(), arm_manager.get_link3(), arm_manager.get_elbow());
            arm_manager.save(ARM_MANAGER_ADDRESS);
            break;
        case LINK_2_ID :
            arm_manager.set_arm_link(arm_manager.get_link1(), input.read<float>(), arm_manager.get_link3(), arm_manager.get_elbow());
            arm_manager.save(ARM_MANAGER_ADDRESS);
            break;
        case LINK_3_ID :
            arm_manager.set_arm_link(arm_manager.get_link1(), arm_manager.get_link2(), input.read<float>(), arm_manager.get_elbow());
            arm_manager.save(ARM_MANAGER_ADDRESS);
            break;
        case ELBOW_ID :
            arm_manager.set_arm_link(arm_manager.get_link1(), arm_manager.get_link2(), arm_manager.get_link3(), input.read<float>());
            arm_manager.save(ARM_MANAGER_ADDRESS);
            break;
        case TIMESTEP_ID :
            traj_manager.set_timestep(input.read<float>());
            traj_manager.save(TRAJ_MANAGER_ADDRESS);
            break;
    }
}

void GET_PARAMETER_VALUE(SerialTalks& talks, Deserializer& input, Serializer& output)
{
    byte  id = input.read<byte>();
	switch (id)
	{
        case MOTOR1_ID_ID :
            output.write<int>(AX1.getID());
            break;
        case MOTOR1_OFFSET_ID :
            output.write<float>(AX1.getOFFSET());
            break;
        case MOTOR2_ID_ID :
            output.write<int>(AX2.getID());
            break;
        case MOTOR2_OFFSET_ID :
            output.write<float>(AX2.getOFFSET());
            break;
        case MOTOR3_ID_ID :
            output.write<int>(AX3.getID());
            break;
        case MOTOR3_OFFSET_ID :
            output.write<float>(AX3.getOFFSET());
            break;
        case WORKSPACE_FRONT_ID :
            {
            Workspace ws;
            ws = arm_manager.get_workspace_front();
            output.write<float>(ws.x_min);
            output.write<float>(ws.x_max);
            output.write<float>(ws.y_min);
            output.write<float>(ws.y_max);
            output.write<float>(ws.elbow_or);
            break;
            }
        case WORKSPACE_BACK_ID :
            {
            Workspace ws;
            ws = arm_manager.get_workspace_back();
            output.write<float>(ws.x_min);
            output.write<float>(ws.x_max);
            output.write<float>(ws.y_min);
            output.write<float>(ws.y_max);
            output.write<float>(ws.elbow_or);
            break;
            }
        case ORIGIN_ID :
            {
            Coords origin;
            origin = arm_manager.get_origin();
            output.write<float>(origin.x);
            output.write<float>(origin.y);
            output.write<float>(origin.phi);
            break;
            }
        case JOINTS_ID :
            {
            Joints joint;
            joint = arm_manager.get_joints();
            output.write<float>(joint.th1);
            output.write<float>(joint.th2);
            output.write<float>(joint.th3);
            }
            break;
        case LINK_1_ID :
            output.write<float>(arm_manager.get_link1());
            break;
        case LINK_2_ID :
            output.write<float>(arm_manager.get_link2());
            break;
        case LINK_3_ID :
            output.write<float>(arm_manager.get_link3());
            break;
        case ELBOW_ID :
            output.write<float>(arm_manager.get_elbow());
            break;
        case TIMESTEP_ID :
            output.write<float>(traj_manager.get_timestep());
            break;
    }
}
