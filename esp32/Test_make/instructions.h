#ifndef __INSTRUCTIONS_H__
#define __INSTRUCTIONS_H__

#include "../common/IK/Picker.h"
#include "../common/SerialTalks.h"

#define GET_PATH_OPCODE      0x10
#define GET_TOOL_OPCODE      0x11

void GET_PATH(SerialTalks& talks, Deserializer& input, Serializer& output);

void GET_TOOL(SerialTalks& talks, Deserializer& input, Serializer& output);

#endif //__INSTRUCTIONS_H__

