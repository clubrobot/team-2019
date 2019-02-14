#include <Arduino.h>
#include "instructions.h"

static int a = 0;

void ON(SerialTalks &inst, Deserializer &input, Serializer &output)
{
    digitalWrite(2, HIGH);
}

void OFF(SerialTalks &inst, Deserializer &input, Serializer &output)
{
    digitalWrite(2, LOW);
}

void WRITE(SerialTalks &inst, Deserializer &input, Serializer &output)
{
    a = input.read<int>();
}

void READ(SerialTalks &inst, Deserializer &input, Serializer &output)
{
    output.write<int>(a);
}