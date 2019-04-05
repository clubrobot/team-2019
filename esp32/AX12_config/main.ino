#include <Arduino.h>
#include "../common/SerialTalks.h"
#include "../common/AX12/AX12.h"
#include "instructions.h"

AX12 ax;

void setup()
{
	//Starting SerialTalks
	Serial.begin(SERIALTALKS_BAUDRATE);
    talks.begin(Serial);

    talks.bind(ATTACH_OPCODE, ATTACH);
    talks.bind(DETACH_OPCODE, DETACH);
    talks.bind(RESET_OPCODE, RESET);

    talks.bind(PING_AX_OPCODE, PING_AX);

    talks.bind(SET_ID_OPCODE, SET_ID);
    talks.bind(SET_BD_OPCODE, SET_BD);

    talks.bind(MOVE_OPCODE, MOVE);
    talks.bind(MOVE_SPEED_OPCODE, MOVE_SPEED);

    talks.bind(SET_ENDLESS_MODE_OPCODE, SET_ENDLESS_MODE);

    talks.bind(SET_TEMP_LIMIT_OPCODE, SET_TEMP_LIMIT);
    talks.bind(SET_ANGLE_LIMIT_OPCODE, SET_ANGLE_LIMIT);
    talks.bind(SET_VOLTAGE_LIMIT_OPCODE, SET_VOLTAGE_LIMIT);
    talks.bind(SET_MAX_TORQUE_OPCODE, SET_MAX_TORQUE);
    talks.bind(SET_MAX_TORQUE_RAM_OPCODE, SET_MAX_TORQUE_RAM);
    talks.bind(SET_SRL_OPCODE, SET_SRL);
    talks.bind(SET_RDT_OPCODE, SET_RDT);
    talks.bind(SET_LED_ALARM_OPCODE, SET_LED_ALARM);
    talks.bind(SET_SUTDOWN_ALARM_OPCODE, SET_SUTDOWN_ALARM);
    talks.bind(SET_CMARGIN_OPCODE, SET_CMARGIN);
    talks.bind(SET_CSLOPE_OPCODE, SET_CSLOPE);
    talks.bind(SET_PUNCH_OPCODE, SET_PUNCH);

    talks.bind(READ_TEMPERATURE_OPCODE, READ_TEMPERATURE);
    talks.bind(READ_VOLTAGE_OPCODE, READ_VOLTAGE);
    talks.bind(READ_POSITION_OPCODE, READ_POSITION);
    talks.bind(READ_SPEED_OPCODE, READ_SPEED);
    talks.bind(READ_TORQUE_OPCODE, READ_TORQUE);

    AX12::SerialBegin(1000000, 5);

}
void loop()
{  
    talks.execute();
}