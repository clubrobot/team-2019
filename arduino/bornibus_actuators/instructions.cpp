#include "instructions.h"
#include <Servo.h>
#include "../common/SerialTalks.h"
#include "../common/EndStop.h"
#include "PIN.h"

extern Servo gripper;
extern Servo pusher1;
extern Servo pusher2;
extern Servo arm;
extern EndStop endStop1;
extern EndStop endStop2;
extern EndStop endStop3;

void DISABLE(SerialTalks& talks, Deserializer& input, Serializer& output)
{
    pusher1.detach();
    pusher2.detach();
    arm.detach();

    gripper.write(90);
    delay(0.5);
    gripper.detach();
}

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

void GET_ENDSTOP1_STATE(SerialTalks &inst, Deserializer &input, Serializer &output){
	if(endStop1.getState()){
		output.write<int>(0);
	}else{
		output.write<int>(1);
	}
}
void GET_ENDSTOP2_STATE(SerialTalks &inst, Deserializer &input, Serializer &output){
	if(endStop2.getState()){
		output.write<int>(0);
	}else{
		output.write<int>(1);
	}
}

void GET_ENDSTOP3_STATE(SerialTalks &inst, Deserializer &input, Serializer &output){
	if(endStop3.getState()){
		output.write<int>(0);
	}else{
		output.write<int>(1);
	}
}