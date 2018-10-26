#include "instructions.h"
#include "BMS.h"


// Global variables
extern BMS bms;

void GET_RSOC(SerialTalks& inst, Deserializer& input, Serializer& output)
{
    output.write<int>(bms.getRelativeSOC());
}

void GET_ASOC(SerialTalks& inst, Deserializer& input, Serializer& output)
{
    output.write<int>(bms.getAbsoluteSOC());
}

void GET_VOLTAGE(SerialTalks& inst, Deserializer& input, Serializer& output)
{
    output.write<int>(bms.getVoltage());
}

void GET_CYCLE_COUNT(SerialTalks& inst, Deserializer& input, Serializer& output)
{
    output.write<int>(bms.getCycleCount());
}

void GET_CURRENT(SerialTalks& inst, Deserializer& input, Serializer& output)
{
    output.write<int>(bms.getCurrent());
}

void GET_AVERAGE_CURRENT(SerialTalks& inst, Deserializer& input, Serializer& output)
{
    output.write<int>(bms.getAverageCurrent());
}

void GET_REMAINING_CAP(SerialTalks& inst, Deserializer& input, Serializer& output)
{
    output.write<int>(bms.getRemainingCapacity());
}

void GET_AVERAGE_TIME_EMPTY(SerialTalks& inst, Deserializer& input, Serializer& output)
{
    output.write<int>(bms.getAverageTimeRemainingToEmpty());
}

void GET_AVERAGE_TIME_FULL(SerialTalks& inst, Deserializer& input, Serializer& output)
{
    output.write<int>(bms.getAverageTimeRemainingToFull());
}
