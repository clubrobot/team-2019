#ifndef __INSTRUCTIONS_H__
#define __INSTRUCTIONS_H__

#include "../../common/SerialTalks.h"

#define _SET_MOTOR1_VELOCITY_OPCODE 			0x03
#define _SET_MOTOR2_VELOCITY_OPCODE		        0x04
#define _SET_AX_POSITION_OPCODE					0X05
#define _GET_AX_POSITION_OPCODE					0X06
#define _GET_AX_TORQUE_OPCODE					0X07
#define _SET_AX_VELOCITY_MOVE_OPCODE			0X08
#define _PING_AX_OPCODE							0x09
#define _SET_AX_HOLD_OPCODE						0X0A
#define _IS_UP_OPCODE						    0X0B
#define _IS_DOWN_OPCODE						    0X0C
#define _SET_SERVO_OPCODE						0X0D
#define _SET_SERVO_MICROSECONDS_OPCODE			0X0E
#define _SET_TOR_OPCODE		    				0X0F
#define _STOP_OPCODE	    					0X10

void SET_MOTOR1_VELOCITY(SerialTalks &inst, Deserializer &input, Serializer &output);

void SET_MOTOR2_VELOCITY(SerialTalks &inst, Deserializer &input, Serializer &output);

void SET_AX_POSITION(SerialTalks &inst, Deserializer &input, Serializer &output);

void GET_AX_TORQUE(SerialTalks &inst, Deserializer &input, Serializer &output);

void SET_AX_VELOCITY_MOVE(SerialTalks &inst, Deserializer &input, Serializer &output);

void PING_AX(SerialTalks &inst, Deserializer &input, Serializer &output);

void SET_AX_HOLD(SerialTalks &inst, Deserializer &input, Serializer &output);

void GET_AX_POSITION(SerialTalks &inst, Deserializer &input, Serializer &output);

void IS_UP(SerialTalks &inst, Deserializer &input, Serializer &output);

void IS_DOWN(SerialTalks &inst, Deserializer &input, Serializer &output);

void SET_SERVO(SerialTalks &inst, Deserializer &input, Serializer &output);

void SET_SERVO_MICROSECONDS(SerialTalks &inst, Deserializer &input, Serializer &output);

void SET_TOR(SerialTalks &inst, Deserializer &input, Serializer &output);

void STOP(SerialTalks &inst, Deserializer &input, Serializer &output);

#endif