#ifndef __INSTRUCTIONS_H__
#define __INSTRUCTIONS_H__

#include "../common/SerialTalks.h"


// Opcodes declaration
#define ENABLE_LED_OPCODE          0x11
#define DISABLE_LED_OPCODE      0x12

// Instructions prototypes

void ENABLE_LED(SerialTalks& inst, Deserializer& input, Serializer& output);
void DISABLE_LED(SerialTalks& inst, Deserializer& input, Serializer& output);
#endif
