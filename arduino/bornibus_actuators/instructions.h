#ifndef __INSTRUCTIONS_H__
#define __INSTRUCTIONS_H__

#include "../common/SerialTalks.h"

#define _SET_POSITION_GRIPPER_OPCODE    0x10
#define _SET_POSITION_PUSHERS_OPCODE    0x11
#define _SET_POSITION_ARM_OPCODE        0x12
#define _GET_ENDSTOP1_STATE_OPCODE      0x13
#define _GET_ENDSTOP2_STATE_OPCODE      0x14
#define _GET_ENDSTOP3_STATE_OPCODE      0x15


void SET_POSITION_GRIPPER(SerialTalks &inst, Deserializer &input, Serializer &output);
void SET_POSITION_PUSHERS(SerialTalks &inst, Deserializer &input, Serializer &output);
void SET_POSITION_ARM(SerialTalks &inst, Deserializer &input, Serializer &output);
void GET_ENDSTOP1_STATE(SerialTalks &inst, Deserializer &input, Serializer &output);
void GET_ENDSTOP2_STATE(SerialTalks &inst, Deserializer &input, Serializer &output);
void GET_ENDSTOP3_STATE(SerialTalks &inst, Deserializer &input, Serializer &output);

#endif
