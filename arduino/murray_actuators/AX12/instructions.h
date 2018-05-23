#ifndef __INSTRUCTIONS_H__
#define __INSTRUCTIONS_H__

#include "../../common/SerialTalks.h"



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

#define OPEN_GRIPPER_OPCODE 0X1B
#define CLOSE_GRIPPER_OPCODE 0X1C
#define GET_EMERGENCY_STATE_OPCODE 0x1D
#define WRITE_BEEACTIVATOR_OPCODE 0x1E

void BEGIN(SerialTalks &inst, Deserializer &input, Serializer &output);

void SET_POSITION(SerialTalks &inst, Deserializer &input, Serializer &output);

void SET_X(SerialTalks &inst, Deserializer &input, Serializer &output);

void SET_Y(SerialTalks &inst, Deserializer &input, Serializer &output);

void SET_Z(SerialTalks &inst, Deserializer &input, Serializer &output);

void SET_THETA(SerialTalks &inst, Deserializer &input, Serializer &output);

void SET_SPEED(SerialTalks &inst, Deserializer &input, Serializer &output);

void GET_POSITION(SerialTalks &inst, Deserializer &input, Serializer &output);

void GET_POSITION_THEO(SerialTalks &inst, Deserializer &input, Serializer &output);

void SET_ANGLES(SerialTalks &inst, Deserializer &input, Serializer &output);

void OPEN_GRIPPER(SerialTalks &inst, Deserializer &input, Serializer &output);

void CLOSE_GRIPPER(SerialTalks &inst, Deserializer &input, Serializer &output);

void GET_EMERGENCY_STATE(SerialTalks &inst, Deserializer &input, Serializer &output);

void WRITE_BEE_ACTIVATOR(SerialTalks &inst, Deserializer &input, Serializer &output);

#endif //__INSTRUCTIONS_H__
