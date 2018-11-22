#ifndef __INSTRUCTIONS_H__
#define __INSTRUCTIONS_H__

#include "../common/IK/Picker.h"
#include "../common/SerialTalks.h"

#define MOVE_DIRECTLY_OPCODE      0x10
#define MOVE_PATH_OPCODE          0x11
#define IS_ARRIVED_OPCODE         0x12

void MOVE_DIRECTLY(SerialTalks& talks, Deserializer& input, Serializer& output);

void MOVE_PATH(SerialTalks& talks, Deserializer& input, Serializer& output);

void IS_ARRIVED(SerialTalks& talks, Deserializer& input, Serializer& output);

#endif //__INSTRUCTIONS_H__

