#ifndef __INSTRUCTIONS_H__
#define __INSTRUCTIONS_H__

#include "../common/SerialTalks.h"

#define _SET_ROLLER_VELOCITY_OPCODE 			0x12
#define _SET_FIRING_HAMMER_VELOCITY_OPCODE		0x04
#define _SETUP_AX_OPCODE						0x0C
#define _SET_AX_POSITION_OPCODE					0X05
#define _GET_AX_POSITION_OPCODE					0X06
#define _GET_AX_TORQUE_OPCODE					0X07
#define _SET_AX_VELOCITY_MOVE_OPCODE			0X08
#define _PING_AX_OPCODE							0x09
#define _SET_AX_HOLD_OPCODE						0X0A
#define _GET_AX_VELOCITY_OPCODE					0x10
#define _GET_AX_MOVING_OPCODE					0x0D

#define _SET_ROLLER_VELOCITY_OPCODE 			0x12
#define _SET_FIRING_HAMMER_VELOCITY_OPCODE		0x04
#define _RETURN_TO_SAFE_POSITION_OPCODE			0x0B

#define AX12_SEND_INSTRUCTION_PACKET_OPCODE 0x0E
#define AX12_RECEIVE_STATUS_PACKET_OPCODE   0x0F

#define LAUNCHPAD_SET_POSITION_OPCODE				0x11

void SET_ROLLER_VELOCITY(SerialTalks &inst, Deserializer &input, Serializer &output);

void SET_FIRING_HAMMER_VELOCITY(SerialTalks &inst, Deserializer &input, Serializer &output);
void RETURN_TO_SAFE_POSITION(SerialTalks& talks, Deserializer& input, Serializer& output);

void SETUP_AX(SerialTalks& talks, Deserializer& input, Serializer& output);

void SET_AX_POSITION(SerialTalks &inst, Deserializer &input, Serializer &output);

void GET_AX_TORQUE(SerialTalks &inst, Deserializer &input, Serializer &output);

void SET_AX_VELOCITY_MOVE(SerialTalks &inst, Deserializer &input, Serializer &output);

void PING_AX(SerialTalks &inst, Deserializer &input, Serializer &output);

void SET_AX_HOLD(SerialTalks &inst, Deserializer &input, Serializer &output);

void GET_AX_POSITION(SerialTalks &inst, Deserializer &input, Serializer &output);

void AX12_SEND_INSTRUCTION_PACKET(SerialTalks& talks, Deserializer& input, Serializer& output);

void AX12_RECEIVE_STATUS_PACKET(SerialTalks& talks, Deserializer& input, Serializer& output);

void GET_AX_VELOCITY(SerialTalks& talks, Deserializer& input, Serializer& output);

void GET_AX_MOVING(SerialTalks& talks, Deserializer& input, Serializer& output);

void LAUNCHPAD_SET_POSITION(SerialTalks& talks, Deserializer& input, Serializer& output);

#endif