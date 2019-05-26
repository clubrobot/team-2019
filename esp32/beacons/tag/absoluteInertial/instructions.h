#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include "../common/SerialTalks.h"

#define GET_ACCELERATION_OPCODE             0x10
#define GET_MAGNETIC_OPCODE                 0x11
#define GET_ORIENTATION_OPCODE              0x12
#define GET_GYROSCOPE_OPCODE                0x13
#define GET_LINEAR_ACCELERATION_OPCODE      0x14
#define GET_TEMPERATURE_OPCODE              0x15
#define GET_STATE_OPCODE                    0x16
#define READ_BYTE_OPCODE                    0x17
#define IS_CONNECTED_OPCODE                 0X18
#define BEGIN_COM_OPCODE                    0x19
#define READ_LEN_OPCODE                     0X20
#define GET_ABSOLUTE_ACCELERATION_OPCODE    0x21
#define SET_ORIENTATION_OPCODE              0x22

// Instructions prototypes

void GET_ACCELERATION(SerialTalks &talks, Deserializer &input, Serializer &output);
void GET_MAGNETIC(SerialTalks &talks, Deserializer &input, Serializer &output);
void GET_ORIENTATION(SerialTalks &talks, Deserializer &input, Serializer &output);
void GET_GYROSCOPE(SerialTalks &talks, Deserializer &input, Serializer &output);
void GET_LINEAR_ACCELERATION(SerialTalks &talks, Deserializer &input, Serializer &output);
void GET_TEMPERATURE(SerialTalks &talks, Deserializer &input, Serializer &output);
void GET_STATE(SerialTalks &talks, Deserializer &input, Serializer &output);
void READ_BYTE(SerialTalks &talks, Deserializer &input, Serializer &output);
void IS_CONNECTED(SerialTalks &talks, Deserializer &input, Serializer &output);
void BEGIN_COM(SerialTalks &talks, Deserializer &input, Serializer &output);
void READ_LEN(SerialTalks &talks, Deserializer &input, Serializer &output);
void GET_ABSOLUTE_ACCELERATION(SerialTalks &talks, Deserializer &input, Serializer &output);
void SET_ORIENTATION(SerialTalks &talks, Deserializer &input, Serializer &output);


#endif //INSTRUCTIONS_H