#ifndef __INSTRUCTIONS_H__
#define __INSTRUCTIONS_H__

#include "../common/IK/Picker.h"
#include "../common/SerialTalks.h"
#include "arm_position.h"

#define ADD_MOVE_POSITION_OPCODE    0x15
#define RUN_BATCH_OPCODE            0X16  
#define IS_ARRIVED_OPCODE           0x17

void ADD_MOVE_POSITION_DIRECTLY(SerialTalks& talks, Deserializer& input, Serializer& output);

void RUN_BATCH(SerialTalks& talks, Deserializer& input, Serializer& output);

void IS_ARRIVED(SerialTalks& talks, Deserializer& input, Serializer& output);

#endif //__INSTRUCTIONS_H__

