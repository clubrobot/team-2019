#include <Arduino.h>
#include <Servo.h>

#include "PIN.h"
#include "instructions.h"
#include "../../common/SerialTalks.h"

/* rename servo with real actuator name */
Servo servo_1;
Servo servo_2;
Servo servo_3;
Servo servo_4;
Servo servo_5;
Servo servo_6;

void setup()
{
	Serial.begin(SERIALTALKS_BAUDRATE);
    talks.begin(Serial);

    talks.bind(_WRITE_SERVO1_OPCODE, WRITE_SERVO1);
    talks.bind(_GET_SERVO1_OPCODE, GET_SERVO1);

    talks.bind(_WRITE_SERVO2_OPCODE, WRITE_SERVO2);
    talks.bind(_GET_SERVO2_OPCODE, GET_SERVO2);

    talks.bind(_WRITE_SERVO3_OPCODE, WRITE_SERVO3);
    talks.bind(_GET_SERVO3_OPCODE, GET_SERVO3);

    talks.bind(_WRITE_SERVO4_OPCODE, WRITE_SERVO4);
    talks.bind(_GET_SERVO4_OPCODE, GET_SERVO4);

    talks.bind(_WRITE_SERVO5_OPCODE, WRITE_SERVO5);
    talks.bind(_GET_SERVO5_OPCODE, GET_SERVO5);

    talks.bind(_WRITE_SERVO6_OPCODE, WRITE_SERVO6);
    talks.bind(_GET_SERVO6_OPCODE, GET_SERVO6);

    pinMode(SERVO1, OUTPUT);
    pinMode(SERVO2, OUTPUT);
	pinMode(SERVO3, OUTPUT);
	pinMode(SERVO4, OUTPUT);
	pinMode(SERVO5, OUTPUT);
	pinMode(SERVO6, OUTPUT);

	servo_1.attach(SERVO1);
	servo_2.attach(SERVO2);
	servo_3.attach(SERVO3);
	servo_4.attach(SERVO4);
	servo_5.attach(SERVO5);
	servo_6.attach(SERVO6);
}

void loop()
{
	talks.execute();
}