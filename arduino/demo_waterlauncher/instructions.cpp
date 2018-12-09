#include "instructions.h"
#include <Servo.h>
#include "PIN.h"
#include "../common/SerialTalks.h"
#include "../common/BrushlessMotor.h"

extern BrushlessMotor motor;
extern Servo inDoor;
extern Servo outDoor;
extern Servo trash;


void SET_MOTOR_VELOCITY(SerialTalks &inst, Deserializer &input, Serializer &output){
	motor.setVelocity(input.read<float>());
}

void WRITE_INDOOR(SerialTalks &inst, Deserializer &input, Serializer &output){
	inDoor.write(input.read<int>());
}

void WRITE_OUTDOOR(SerialTalks &inst, Deserializer &input, Serializer &output){
	outDoor.write(input.read<int>());
}

void WRITE_TRASH(SerialTalks &inst, Deserializer &input, Serializer &output){
	trash.write(input.read<int>());
}

void GET_MOTOR_VELOCITY(SerialTalks &inst, Deserializer &input, Serializer &output){
	output.write<float>(motor.getVelocity());
}