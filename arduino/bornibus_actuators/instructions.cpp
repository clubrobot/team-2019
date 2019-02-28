#include "instructions.h"
#include <Servo.h>
#include "../common/SerialTalks.h"
#include "PIN.h"

extern Servo gripper;
extern Servo pusher1;
extern Servo pusher2;
extern Servo arm;

void SET_POSITION_GRIPPER(SerialTalks &inst, Deserializer &input, Serializer &output){
	if(!gripper.attached()){gripper.attach(SERVO1);}
	gripper.write(input.read<int>());
}

void SET_POSITION_PUSHERS(SerialTalks &inst, Deserializer &input, Serializer &output){
	if(!pusher1.attached()){pusher1.attach(SERVO3);}
	if(!pusher2.attached()){pusher2.attach(SERVO4);}
	pusher1.write(input.read<int>());
	pusher2.write(input.read<int>());
}

void SET_POSITION_ARM(SerialTalks &inst, Deserializer &input, Serializer &output){
	if(!arm.attached()){arm.attach(SERVO2);}
	arm.write(input.read<int>());
}