#ifndef __INSTRUCTIONS_H__
#define __INSTRUCTIONS_H__

#include "../common/SerialTalks.h"

#define _SET_POSITION_OPCODE			0x10

void SET_POSITION(SerialTalks &inst, Deserializer &input, Serializer &output);

#endif
