#ifndef __INSTRUCTIONS_H__
#define __INSTRUCTIONS_H__

#include "../common/IK/Picker.h"
#include "../common/SerialTalks.h"
#include "arm_position.h"

#define ADD_MOVE_OPCODE      0x10
#define RUN_BATCH_OPCODE     0X11 
#define STOP_BATCH_OPCODE    0X12 
#define IS_ARRIVED_OPCODE    0x13

void ADD_MOVE(SerialTalks& talks, Deserializer& input, Serializer& output);

void RUN_BATCH(SerialTalks& talks, Deserializer& input, Serializer& output);

void STOP_BATCH(SerialTalks& talks, Deserializer& input, Serializer& output);

void IS_ARRIVED(SerialTalks& talks, Deserializer& input, Serializer& output);

#endif //__INSTRUCTIONS_H__

