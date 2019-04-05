#ifndef __INSTRUCTIONS_H__
#define __INSTRUCTIONS_H__

#include "../common/SerialTalks.h"

#define START_OPCODE 0X10
#define ISONTOP_OPCODE 0X11


void START(SerialTalks &inst, Deserializer &input, Serializer &output);
void ISONTOP(SerialTalks &inst, Deserializer &input, Serializer &output);

#endif //__INSTRUCTIONS_H__
