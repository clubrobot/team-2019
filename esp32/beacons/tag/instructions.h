#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include "../common/SerialTalks.h"

#define GET_POSITION_OPCODE 0x10
#define SET_POSITION_OPCODE 0x11

// Instructions prototypes

void GET_POSITION(SerialTalks &talks, Deserializer &input, Serializer &output); // return positions in mm
void SET_POSITION(SerialTalks &talks, Deserializer &input, Serializer &output); // calibrate the tag by giving its position
#endif //INSTRUCTIONS_H