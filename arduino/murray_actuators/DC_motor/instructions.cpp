#include "instructions.h"
#include "../../common/ShiftRegDCMotor.h"

extern ShiftRegDCMotorsDriver driver;
extern ShiftRegDCMotor motor_1;
extern ShiftRegDCMotor motor_2;

void RESET_DRIVER(SerialTalks& talks, Deserializer& input, Serializer& output)
{
	driver.reset();
}

void ENABLE_MOTOR1(SerialTalks& talks, Deserializer& input, Serializer& output)
{
	motor_1.enable();
}

void ENABLE_MOTOR2(SerialTalks& talks, Deserializer& input, Serializer& output)
{
	motor_2.enable();
}

void DISABLE_MOTOR1(SerialTalks& talks, Deserializer& input, Serializer& output)
{
	motor_1.disable();
}

void DISABLE_MOTOR2(SerialTalks& talks, Deserializer& input, Serializer& output)
{
	motor_2.disable();
}

void IS_ENABLED1(SerialTalks& talks, Deserializer& input, Serializer& output)
{
	output.write<bool>(motor_1.isEnabled());
}

void IS_ENABLED2(SerialTalks& talks, Deserializer& input, Serializer& output)
{
	output.write<bool>(motor_2.isEnabled());
}

void SET_VELOCITY1(SerialTalks& talks, Deserializer& input, Serializer& output)
{
	motor_1.setVelocity(input.read<float>());
}

void SET_VELOCITY2(SerialTalks& talks, Deserializer& input, Serializer& output)
{
	motor_2.setVelocity(input.read<float>());
}