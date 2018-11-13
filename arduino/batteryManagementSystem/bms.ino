#include <Arduino.h>
#include "../common/SerialTalks.h"
#include "instructions.h"
#include "BMS.h"

BMS bms = BMS(BMS_DEVICE_ADDRESS);

void setup() {
	Serial.begin(SERIALTALKS_BAUDRATE);
    talks.begin(Serial);
  	talks.bind(GET_RSOC_OPCODE, GET_RSOC);
	talks.bind(GET_ASOC_OPCODE, GET_ASOC);
	talks.bind(GET_VOLTAGE_OPCODE, GET_VOLTAGE);
	talks.bind(GET_CYCLE_COUNT_OPCODE, GET_CYCLE_COUNT);
	talks.bind(GET_CURRENT_OPCODE, GET_CURRENT);
	talks.bind(GET_AVERAGE_CURRENT_OPCODE, GET_AVERAGE_CURRENT);
	talks.bind(GET_REMAINING_CAP_OPCODE, GET_REMAINING_CAP);
	talks.bind(GET_AVERAGE_TIME_EMPTY_OPCODE, GET_AVERAGE_TIME_EMPTY);
	talks.bind(GET_AVERAGE_TIME_FULL_OPCODE, GET_AVERAGE_TIME_FULL);
}

void loop() {
	talks.execute();
}
