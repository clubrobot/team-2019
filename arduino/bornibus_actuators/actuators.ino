#include <Arduino.h>
#include <Servo.h>
#include "../common/SerialTalks.h"
#include "../common/EndStop.h"
#include "PIN.h"
#include "instructions.h"

Servo gripper;
Servo pusher1;
Servo pusher2;
Servo arm;
EndStop endStop1;
EndStop endStop2;
EndStop endStop3;

void setup()
{
    endStop1.attach(SWITCH1);
    endStop2.attach(SWITCH2);
    endStop3.attach(DRIVER_RESET); // NO motor was needed so we used this PIN to add a new switch
    Serial.begin(SERIALTALKS_BAUDRATE);
    talks.begin(Serial);
	talks.bind(_SET_POSITION_PUSHERS_OPCODE, SET_POSITION_PUSHERS);
    talks.bind(_SET_POSITION_GRIPPER_OPCODE, SET_POSITION_GRIPPER);
    talks.bind(_SET_POSITION_ARM_OPCODE, SET_POSITION_ARM);
    talks.bind(_GET_ENDSTOP1_STATE_OPCODE, GET_ENDSTOP1_STATE);
    talks.bind(_GET_ENDSTOP2_STATE_OPCODE, GET_ENDSTOP2_STATE);
    talks.bind(_GET_ENDSTOP3_STATE_OPCODE, GET_ENDSTOP3_STATE);
    talks.bind(SERIALTALKS_DISCONNECT_OPCODE, DISABLE);

}

void loop()
{
    talks.execute();
}
