#include "BMS.h"

uint8_t BMS::readWordU1(uint8_t cmd){
    Wire.beginTransmission(bms_address);
    Wire.write(cmd);
    Wire.endTransmission(true);
    Wire.requestFrom(bms_address,1,true);
    uint8_t value = 0xFF;
    if(Wire.available()){
        value = Wire.read();
    }
    Wire.endTransmission();
    return value;
}

uint16_t BMS::readWordU2(uint8_t cmd){
    Wire.beginTransmission(bms_address);
    Wire.write(cmd);
    Wire.endTransmission(true);
    Wire.requestFrom(bms_address,2,true);
    uint16_t value;
    if(Wire.available() == 2){
        value = Wire.read();
        value |= (Wire.read() << 8);
    }
    Wire.endTransmission();
    return value;
}

uint8_t BMS::getRelativeSOC(){
    return readWordU1(RELATIVE_SOC);
}

uint8_t BMS::getAbsoluteSOC(){
    return readWordU1(ABSOLUTE_SOC);
}

uint16_t BMS::getVoltage(){
    return readWordU2(VOLTAGE);
}

uint16_t BMS::getCycleCount(){
    return readWordU2(CYCLE_COUNT);
}

int16_t BMS::getCurrent(){
    return (int16_t)readWordU2(CURRENT);
}

int16_t BMS::getAverageCurrent(){
    return (int16_t)readWordU2(AVERAGE_CURRENT);
}

uint16_t BMS::getRemainingCapacity(){
    return readWordU2(REMAINING_CAPACITY);
}

uint16_t BMS::getAverageTimeRemainingToEmpty(){
    return readWordU2(AVERAGE_TIME_TO_EMPTY);
}
uint16_t BMS::getAverageTimeRemainingToFull(){
    return readWordU2(AVERAGE_TIME_TO_FULL);
}
