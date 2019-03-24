#ifndef __INSTRUCTIONS_H__
#define __INSTRUCTIONS_H__

#include "../common/SerialTalks.h"


// Opcodes declaration  
#define GET_ROBOT_ACC_OPCODE		    0x10

// Instructions prototypes
void GET_ROBOT_ACC(SerialTalks& inst, Deserializer& input, Serializer& output);

#endif
