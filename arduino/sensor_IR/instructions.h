#ifndef __INSTRUCTIONS_H__
#define __INSTRUCTIONS_H__

#include "../common/SerialTalks.h"


// Opcodes declaration  
#define GET_DIST_OPCODE		    0x10
#define GET_STATE_OPCODE        0X11
#define GET_VL53_OPCODE         0x12
#define GET_VL61_OPCODE         0x13

// Instructions prototypes
void GET_DIST(SerialTalks& inst, Deserializer& input, Serializer& output);
void GET_VL53(SerialTalks& inst, Deserializer& input, Serializer& output);
void GET_VL61(SerialTalks& inst, Deserializer& input, Serializer& output);
void GET_STATE(SerialTalks& inst, Deserializer& input, Serializer& output);
#endif
