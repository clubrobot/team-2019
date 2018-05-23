#include <Arduino.h>
#include "../../common/SoftwareSerial.h"
#include "../../common/RobotArm.h"
#include "../../common/ShiftRegAX12.h"
#include "instructions.h"
#include "PIN.h"

extern RobotArm arm;
extern Servo beeActivator;

void BEGIN(SerialTalks &inst, Deserializer &input, Serializer &output)
{
	arm.attach(2,1,3,SERVO1);

	arm.begin();

}

void SET_POSITION(SerialTalks &inst, Deserializer &input, Serializer &output)
{
	float x 	= input.read<float>();
	float y 	= input.read<float>();
	float z 	= input.read<float>();
	float d 	= input.read<float>();
	float z_o 	= input.read<int>();

	arm.ReachPosition(x,y,z*10,d,z_o);
}

void SET_X(SerialTalks &inst, Deserializer &input, Serializer &output)
{
	arm.set_x(input.read<float>());
}

void SET_Y(SerialTalks &inst, Deserializer &input, Serializer &output)
{
	arm.set_y(input.read<float>());
}

void SET_Z(SerialTalks &inst, Deserializer &input, Serializer &output)
{
	arm.set_z(input.read<float>());
}

void SET_THETA(SerialTalks &inst, Deserializer &input, Serializer &output)
{
	arm.set_theta(input.read<float>());
}

void SET_SPEED(SerialTalks &inst, Deserializer &input, Serializer &output)
{
	arm.set_speed(input.read<float>());
}

void GET_POSITION(SerialTalks &inst, Deserializer &input, Serializer &output)
{
	output.write<float>(arm.get_A2());

	output.write<float>(arm.get_A1());

	output.write<float>(arm.get_A3());
}

void GET_POSITION_THEO(SerialTalks &inst, Deserializer &input, Serializer &output)
{
	output.write<float>(arm.get_A2theo());

	output.write<float>(arm.get_A1theo());

	output.write<float>(arm.get_A3theo());
}

void SET_ANGLES(SerialTalks &inst, Deserializer &input, Serializer &output)
{
	// float x = input.read<float>();
	// float y = input.read<float>();
	// float z = input.read<float>();

	// // 	// send pos to AX12 servos
	// servoax.attach(1);
	// servoax.moveSpeed((float)x,50);

	// servoax.attach(2);
	// servoax.moveSpeed((float)y, 50);

	// servoax.attach(3);
	// servoax.moveSpeed((float)z, 50);
}

void OPEN_GRIPPER(SerialTalks &inst, Deserializer &input, Serializer &output)
{
	arm.open_gripper();
}

void CLOSE_GRIPPER(SerialTalks &inst, Deserializer &input, Serializer &output)
{
	arm.close_gripper();
}

void GET_EMERGENCY_STATE(SerialTalks &inst, Deserializer &input, Serializer &output){
	output.write<int>(digitalRead(INTER2));
}

void WRITE_BEEACTIVATOR(SerialTalks& inst, Deserializer& input, Serializer& output){
	int val = input.read<int>();
    if (val >= 0)
    {
        if (!beeActivator.attached())
        {
            beeActivator.attach(SERVO1);
        }
        beeActivator.write(val);
    }
    else if (val < 0)
    {
        beeActivator.detach();
    }
}
