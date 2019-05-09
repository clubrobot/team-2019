#ifndef __INSTRUCTIONS_H__
#define __INSTRUCTIONS_H__

#include "../common/SerialTalks.h"
#include "PIN.h"


// Opcodes declaration
#define GET_RANGE_1_OPCODE			0x10
#define GET_RANGE_2_OPCODE       	0x11
#define CHECK_ERROR_OPCODE          0x12
#define GET_BOTH_RANGE_OPCODE       0x13

// Instructions prototypes
void GET_RANGE_1(SerialTalks& inst, Deserializer& input, Serializer& output);

void GET_RANGE_2(SerialTalks& inst, Deserializer& input, Serializer& output);

void CHECK_ERROR(SerialTalks& inst, Deserializer& input, Serializer& output);

void GET_BOTH_RANGE(SerialTalks& inst, Deserializer& input, Serializer& output);

#endif
