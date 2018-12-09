#ifndef __INSTRUCTIONS_H__
#define __INSTRUCTIONS_H__

#include "../../common/SerialTalks.h"

#define _WRITE_SERVO1_OPCODE 0X11
#define _GET_SERVO1_OPCODE 	0X12

#define _WRITE_SERVO2_OPCODE 0X13
#define _GET_SERVO2_OPCODE 	0X14

#define _WRITE_SERVO3_OPCODE 0X15
#define _GET_SERVO3_OPCODE 	0X16

#define _WRITE_SERVO4_OPCODE 0X17
#define _GET_SERVO4_OPCODE 	0X18

#define _WRITE_SERVO5_OPCODE 0X19
#define _GET_SERVO5_OPCODE 	0X1A

#define _WRITE_SERVO6_OPCODE 0X1B
#define _GET_SERVO6_OPCODE 	0X1C

void WRITE_SERVO1(SerialTalks &inst, Deserializer &input, Serializer &output);
void GET_SERVO1(SerialTalks &inst, Deserializer &input, Serializer &output);

void WRITE_SERVO2(SerialTalks &inst, Deserializer &input, Serializer &output);
void GET_SERVO2(SerialTalks &inst, Deserializer &input, Serializer &output);

void WRITE_SERVO3(SerialTalks &inst, Deserializer &input, Serializer &output);
void GET_SERVO3(SerialTalks &inst, Deserializer &input, Serializer &output);

void WRITE_SERVO4(SerialTalks &inst, Deserializer &input, Serializer &output);
void GET_SERVO4(SerialTalks &inst, Deserializer &input, Serializer &output);

void WRITE_SERVO5(SerialTalks &inst, Deserializer &input, Serializer &output);
void GET_SERVO5(SerialTalks &inst, Deserializer &input, Serializer &output);

void WRITE_SERVO6(SerialTalks &inst, Deserializer &input, Serializer &output);
void GET_SERVO6(SerialTalks &inst, Deserializer &input, Serializer &output);

#endif //__INSTRUCTIONS_H__

