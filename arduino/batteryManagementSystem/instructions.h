#ifndef __INSTRUCTIONS_H__
#define __INSTRUCTIONS_H__

#include "../common/SerialTalks.h"


// Opcodes declaration
#define GET_SOC_OPCODE         0x04


// Instructions prototypes

void GET_SOC(SerialTalks& inst, Deserializer& input, Serializer& output);


#endif //__INSTRUCTIONS_H__

