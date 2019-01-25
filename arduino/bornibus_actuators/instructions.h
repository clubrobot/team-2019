#ifndef __INSTRUCTIONS_H__
#define __INSTRUCTIONS_H__

#include "../common/SerialTalks.h"

#define _SET_POSITION_GRIPPER_OPCODE    0x10
#define _SET_POSITION_PUSHERS_OPCODE    0x11
#define _SET_POSITION_ARM_OPCODE        0x12

void SET_POSITION_GRIPPER(SerialTalks &inst, Deserializer &input, Serializer &output);
void SET_POSITION_PUSHERS(SerialTalks &inst, Deserializer &input, Serializer &output);
void SET_POSITION_ARM(SerialTalks &inst, Deserializer &input, Serializer &output);

#endif
