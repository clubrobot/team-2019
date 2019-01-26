#include "instructions.h"
#include <Servo.h>
#include "../common/SerialTalks.h"

extern Servo gripper;
extern Servo pusher1;
extern Servo pusher2;
extern Servo arm;

void SET_POSITION_GRIPPER(SerialTalks &inst, Deserializer &input, Serializer &output){
	gripper.write(input.read<int>());
}

void SET_POSITION_PUSHERS(SerialTalks &inst, Deserializer &input, Serializer &output){
	pusher1.write(input.read<int>());
	pusher2.write(input.read<int>());
}

void SET_POSITION_ARM(SerialTalks &inst, Deserializer &input, Serializer &output){
	arm.write(input.read<int>());
}