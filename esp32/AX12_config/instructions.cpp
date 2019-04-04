#include <Arduino.h>
#include "instructions.h"

extern AX12 ax;

void ATTACH(SerialTalks &inst, Deserializer &input, Serializer &output)
{
    ax.attach(input.read<byte>());
}
void DETACH(SerialTalks &inst, Deserializer &input, Serializer &output)
{
    ax.detach();
}
void RESET(SerialTalks &inst, Deserializer &input, Serializer &output)
{
    ax.reset();
}
void PING_AX(SerialTalks &inst, Deserializer &input, Serializer &output)
{
    output.write<int>(ax.ping());
}
void SET_ID(SerialTalks &inst, Deserializer &input, Serializer &output)
{
    ax.setID(input.read<byte>());
}
void SET_BD(SerialTalks &inst, Deserializer &input, Serializer &output)
{
    ax.setID(input.read<long>());
}
void MOVE(SerialTalks &inst, Deserializer &input, Serializer &output)
{
    ax.move(input.read<float>());
}
void MOVE_SPEED(SerialTalks &inst, Deserializer &input, Serializer &output)
{
    ax.moveSpeed(input.read<float>(), input.read<float>());
}
void SET_ENDLESS_MODE(SerialTalks &inst, Deserializer &input, Serializer &output)
{
    ax.setEndlessMode(input.read<bool>());
}
void SET_TEMP_LIMIT(SerialTalks &inst, Deserializer &input, Serializer &output)
{
    ax.setTempLimit(input.read<byte>());
}
void SET_ANGLE_LIMIT(SerialTalks &inst, Deserializer &input, Serializer &output)
{
    ax.setAngleLimit(input.read<float>(), input.read<float>());
}
void SET_VOLTAGE_LIMIT(SerialTalks &inst, Deserializer &input, Serializer &output)
{
    ax.setVoltageLimit(input.read<byte>(), input.read<byte>());
}

void SET_MAX_TORQUE(SerialTalks &inst, Deserializer &input, Serializer &output)
{
    ax.setMaxTorque(input.read<int>());
}
void SET_MAX_TORQUE_RAM(SerialTalks &inst, Deserializer &input, Serializer &output)
{
    ax.setMaxTorqueRAM(input.read<int>());
}
void SET_SRL(SerialTalks &inst, Deserializer &input, Serializer &output)
{
    ax.setSRL(input.read<byte>());
}
void SET_RDT(SerialTalks &inst, Deserializer &input, Serializer &output)
{
    ax.setRDT(input.read<byte>());
}
void SET_LED_ALARM(SerialTalks &inst, Deserializer &input, Serializer &output)
{
    ax.setLEDAlarm(input.read<byte>());
}
void SET_SUTDOWN_ALARM(SerialTalks &inst, Deserializer &input, Serializer &output)
{
    ax.setShutdownAlarm(input.read<byte>());
}
void SET_CMARGIN(SerialTalks &inst, Deserializer &input, Serializer &output)
{
    ax.setCMargin(input.read<byte>(), input.read<byte>());
}
void SET_CSLOPE(SerialTalks &inst, Deserializer &input, Serializer &output)
{
    ax.setCSlope(input.read<byte>(), input.read<byte>());
}
void SET_PUNCH(SerialTalks &inst, Deserializer &input, Serializer &output)
{
    ax.setPunch(input.read<int>());
}
void READ_TEMPERATURE(SerialTalks &inst, Deserializer &input, Serializer &output)
{
    output.write<int>(ax.readTemperature());
}
void READ_VOLTAGE(SerialTalks &inst, Deserializer &input, Serializer &output)
{
    output.write<float>(ax.readVoltage());
}
void READ_POSITION(SerialTalks &inst, Deserializer &input, Serializer &output)
{
    output.write<float>(ax.readPosition());
}
void READ_SPEED(SerialTalks &inst, Deserializer &input, Serializer &output)
{
    output.write<float>(ax.readSpeed());
}
void READ_TORQUE(SerialTalks &inst, Deserializer &input, Serializer &output)
{
    output.write<int>(ax.readTorque());
}