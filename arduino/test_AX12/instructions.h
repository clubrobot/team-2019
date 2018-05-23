#ifndef __INSTRUCTIONS_H__
#define __INSTRUCTIONS_H__

#include "../common/SerialTalks.h"



#define BEGIN_OPCODE 0X11
#define SET_POS_OPCODE 0X12

#define SET_X_OPCODE 0X13
#define SET_Y_OPCODE 0X14
#define SET_Z_OPCODE 0X15
#define SET_THETA_OPCODE 0X16

#define SET_SPEED_OPCODE 0X17

#define GET_POS_OPCODE 0X18
#define GET_POS_THEO_OPCODE 0X19

#define SET_ANGLES_OPCODE 0X1A

void BEGIN(SerialTalks &inst, Deserializer &input, Serializer &output);

void SET_ANGLES(SerialTalks &inst, Deserializer &input, Serializer &output);


#endif //__INSTRUCTIONS_H__