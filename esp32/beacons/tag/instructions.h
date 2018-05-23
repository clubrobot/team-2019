#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include "../common/SerialTalks.h"

#define GET_POSITION_OPCODE 0x10

// Instructions prototypes

void GET_POSITION(SerialTalks &talks, Deserializer &input, Serializer &output); // return positions in mm

#endif //INSTRUCTIONS_H