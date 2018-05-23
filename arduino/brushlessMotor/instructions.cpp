#include "instructions.h"
#include "../common/BrushlessMotor.h"


// Global variables
extern BrushlessMotor Motor;



void GET_VELOCITY(SerialTalks& inst, Deserializer& input, Serializer& output)
{
    output.write<float>(Motor.getVelocity());
}

void GET_CONSTANT(SerialTalks& inst, Deserializer& input, Serializer& output)
{
    output.write<float>(Motor.getConstant());
}

void GET_WHEELRADIUS(SerialTalks& inst, Deserializer& input, Serializer& output)
{
    output.write<float>(Motor.getWheelRadius());
}

void GET_MAX_VELOCITY(SerialTalks& inst, Deserializer& input, Serializer& output)
{
    output.write<float>(Motor.getMaxVelocity());
}

void IS_ENABLED(SerialTalks& inst, Deserializer& input, Serializer& output)
{
    output.write<bool>(Motor.isEnabled());
}

void SET_VELOCITY(SerialTalks& inst, Deserializer& input, Serializer& output)
{
    Motor.setVelocity(input.read<float>());
}

void SET_CONSTANT(SerialTalks& inst, Deserializer& input, Serializer& output)
{
    Motor.setConstant(input.read<float>());
}

void SET_WHEELRADIUS(SerialTalks& inst, Deserializer& input, Serializer& output)
{
    Motor.setWheelRadius(input.read<float>());
}

void ENABLE(SerialTalks& inst, Deserializer& input, Serializer& output)
{
    Motor.enable();
}

void DISABLE(SerialTalks& inst, Deserializer& input, Serializer& output)
{
    Motor.disable();
}

void LOAD(SerialTalks& inst, Deserializer& input, Serializer& output)
{
    Motor.load(input.read<int>());
}

void SAVE(SerialTalks& inst, Deserializer& input, Serializer& output)
{
    Motor.save(input.read<int>());
}
