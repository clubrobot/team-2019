#include "instructions.h"
#include <ArduinoSTL.h>
#include "../common/SerialTalks.h"
#include "../common/IK/IK.h"
#include "../common/IK/Arm.h"

extern Arm arm;

// void GET_TOOL(SerialTalks& talks, Deserializer& input, Serializer& output)
// {
//     coords_t tool;
//     joints_t joints;

//     tool.x      = input.read<float>();
//     tool.y      = input.read<float>();
//     tool.phi    = input.read<float>();

//     joints = arm.inverse_kinematics(tool);

//     output.write<float>(joints.th1);
//     output.write<float>(joints.th2);
//     output.write<float>(joints.th3);

//     tool = arm.forward_kinematics(joints);

//     output.write<float>(tool.x);
//     output.write<float>(tool.y);
//     output.write<float>(tool.phi);
// }


void GET_PATH(SerialTalks& talks, Deserializer& input, Serializer& output)
{
    joints_t joints;

    double x = input.read<float>();
    double y = input.read<float>();
    double phi = input.read<float>();
    arm.set_pos(x, y, phi);
    joints = arm.reach_position();

    output.write<float>(joints.th1);
    output.write<float>(joints.th2);
    output.write<float>(joints.th3);
}