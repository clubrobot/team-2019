#include <Arduino.h>
#include "../common/SoftwareSerial.h"
#include "../common/RobotArm.h"
#include "../common/ShiftRegAX12.h"
#include "instructions.h"
#include "PIN.h"

extern ShiftRegAX12 servoax;

void BEGIN(SerialTalks &inst, Deserializer &input, Serializer &output)
{
	servoax.attach(1);
	servoax.setSRL(1); // Respond only to READ_DATA instructions
	servoax.setLEDAlarm(32); // max torque only
	servoax.setShutdownAlarm(32); // max torque only
	servoax.setMaxTorque(1023);
	servoax.setEndlessMode(OFF);
	servoax.hold(OFF);
}

void SET_ANGLES(SerialTalks &inst, Deserializer &input, Serializer &output)
{
	// float y = input.read<float>();
	// float z = input.read<float>();	

		// send pos to AX12 servos

	servoax.setMaxTorqueRAM(1023);
	servoax.move(input.read<float>());

	// servoax.attach(2);
	// servoax.moveSpeed((float)y, 50);

	// servoax.attach(3);
	// servoax.moveSpeed((float)z, 50);
}

