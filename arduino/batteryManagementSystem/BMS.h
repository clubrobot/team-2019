#ifndef _BMS_H_
#define _BMS_H_

#include <Arduino.h>
#include "../common/Wire.h"
#include "SBSCommands.h"


class BMS {
public:
    BMS(uint8_t addr) : bms_address(addr) {
        Wire.begin();
    }

    uint8_t getRelativeSOC();
    uint8_t getAbsoluteSOC();
    uint16_t getVoltage();
    uint16_t getCycleCount();
    int16_t getCurrent();
    int16_t getAverageCurrent();
    uint16_t getRemainingCapacity();
    uint16_t getAverageTimeRemainingToEmpty();
    uint16_t getAverageTimeRemainingToFull();

private:
    uint8_t bms_address;

    uint8_t readWordU1(uint8_t cmd);
    uint16_t readWordU2(uint8_t cmd);

};

#endif
