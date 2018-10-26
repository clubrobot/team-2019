#include "instructions.h"
#include <Servo.h>

extern Servo servo_1;
extern Servo servo_2;
extern Servo servo_3;
extern Servo servo_4;
extern Servo servo_5;
extern Servo servo_6;

void WRITE_SERVO1(SerialTalks &inst, Deserializer &input, Serializer &output)
{
	servo_1.write(input.read<int>());
}

void GET_SERVO1(SerialTalks &inst, Deserializer &input, Serializer &output)
{
	output.write(servo_1.read());
}

void WRITE_SERVO2(SerialTalks &inst, Deserializer &input, Serializer &output)
{
	servo_2.write(input.read<int>());
}

void GET_SERVO2(SerialTalks &inst, Deserializer &input, Serializer &output)
{
	output.write(servo_2.read());
}

void WRITE_SERVO3(SerialTalks &inst, Deserializer &input, Serializer &output)
{
	servo_3.write(input.read<int>());
}

void GET_SERVO3(SerialTalks &inst, Deserializer &input, Serializer &output)
{
	output.write(servo_3.read());
}

void WRITE_SERVO4(SerialTalks &inst, Deserializer &input, Serializer &output)
{
	servo_4.write(input.read<int>());
}
void GET_SERVO4(SerialTalks &inst, Deserializer &input, Serializer &output)
{
	output.write(servo_4.read());
}

void WRITE_SERVO5(SerialTalks &inst, Deserializer &input, Serializer &output)
{
	servo_5.write(input.read<int>());
}

void GET_SERVO5(SerialTalks &inst, Deserializer &input, Serializer &output)
{
	output.write(servo_5.read());
}

void WRITE_SERVO6(SerialTalks &inst, Deserializer &input, Serializer &output)
{
	servo_6.write(input.read<int>());
}

void GET_SERVO6(SerialTalks &inst, Deserializer &input, Serializer &output)
{
	output.write(servo_6.read());
}

