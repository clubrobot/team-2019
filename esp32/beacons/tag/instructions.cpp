#include "instructions.h"

extern float p[2]; // Target point

void GET_POSITION(SerialTalks &talks, Deserializer &input, Serializer &output)
{
    output.write<int16_t>((int16_t)p[0]);
    output.write<int16_t>((int16_t)p[1]);
}
