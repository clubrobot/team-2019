#ifndef __INSTRUCTIONS_H__
#define __INSTRUCTIONS_H__

#include "../common/SerialTalks.h"
#include "../common/RobotArm.h"



#define _WRITE_DISPENSER_OPCODE     0x04
#define _WRITE_GRIP_OPCODE          0X05
#define _OPEN_GRIP_OPCODE			0x06	
#define _SET_GRIP_VELOCITY_OPCODE	0X07
#define _IS_UP_OPCODE               0x08
#define _IS_DOWN_OPCODE             0x09
#define _GET_MOTOR_VELOCITY_OPCODE	0x0B
#define _SET_MOTOR_VELOCITY_OPCODE  0x0C
#define _GET_LEFT_MUSTACHE_OPCODE	0X0D
#define _GET_RIGHT_MUSTACHE_OPCODE	0X0E

void SET_GRIP_VELOCITY(SerialTalks &inst, Deserializer &input, Serializer &output);

void WRITE_GRIP(SerialTalks& inst, Deserializer& input, Serializer& output);

void WRITE_DISPENSER(SerialTalks& inst, Deserializer& input, Serializer& output);

void IS_UP(SerialTalks& inst, Deserializer& input, Serializer& output);

void IS_DOWN(SerialTalks& inst, Deserializer& input, Serializer& output);

void GET_MOTOR_VELOCITY(SerialTalks& inst, Deserializer& input, Serializer& output);

void SET_MOTOR_VELOCITY(SerialTalks& inst, Deserializer& input, Serializer& output);

void OPEN_GRIP(SerialTalks &inst, Deserializer &input, Serializer &output); 

void GET_LEFT_MUSTACHE(SerialTalks &inst, Deserializer &input, Serializer &output); 

void GET_RIGHT_MUSTACHE(SerialTalks &inst, Deserializer &input, Serializer &output);
 
#endif
