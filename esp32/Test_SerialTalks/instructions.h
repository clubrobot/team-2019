#ifndef __INSTRUCTIONS_H__
#define __INSTRUCTIONS_H__

#include "../common/SerialTalks.h"

#define ON_OPCODE 0X10
#define OFF_OPCODE 0X11

#define WRITE_OPCODE 0X12
#define READ_OPCODE 0X13

void ON(SerialTalks &inst, Deserializer &input, Serializer &output);
void OFF(SerialTalks &inst, Deserializer &input, Serializer &output);

void WRITE(SerialTalks &inst, Deserializer &input, Serializer &output);

void READ(SerialTalks &inst, Deserializer &input, Serializer &output);

#endif //__INSTRUCTIONS_H__