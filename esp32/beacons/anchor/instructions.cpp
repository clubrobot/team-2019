#include "instructions.h"
#include "configuration.h"
#include "EEPROM.h"
#include "DW1000Ranging.h"
#include "../common/dataSync.h"
#include "../common/OLED_display.h"

extern boolean deviceConnected;
extern DataSync data;
extern OLEDdisplay display;

void UPDATE_ANCHOR_NUMBER(SerialTalks &talks, Deserializer &input, Serializer &output){
    talks.out << "update anchor number\n";
    byte number = input.read<byte>();
    EEPROM.write(EEPROM_NUM_ANCHOR,number);
    EEPROM.commit();
    // Restart to update current configuration
    talks.out << "restarting to update configuration\n";
    ESP.restart();
}


void UPDATE_ANTENNA_DELAY(SerialTalks &talks, Deserializer &input, Serializer &output){
    talks.out << "update antenna delay\n";
    int antennaDelay = input.read<uint16_t>();
    EEPROM.write(EEPROM_ANTENNA_DELAY, antennaDelay >> 8);
    EEPROM.write(EEPROM_ANTENNA_DELAY+1, antennaDelay % 256);
    EEPROM.commit();
    // Restart to update current configuration
    talks.out << "restarting to update configuration\n";
    //ESP.restart();
}

 
void CALIBRATION_ROUTINE(SerialTalks &talks, Deserializer &input, Serializer &output){
    talks.out << "calibration routine\n";
    int realDistance = input.read<uint16_t>();
    unsigned long timeoutDelay = input.read<unsigned int>();
    DW1000Ranging.startAutoCalibration(realDistance, timeoutDelay);
}

void UPDATE_COLOR(SerialTalks &talks, Deserializer &input, Serializer &output)
{
    talks.out << "changed color\n";
    data.color = (Color)input.read<uint16_t>();
    display.log(data.color == GREEN ? "green" : "orange");
}

void GET_COORDINATE(SerialTalks &talks, Deserializer &input, Serializer &output)
{
    int robotID = input.read<int16_t>();
    if(robotID < 0 || robotID >= MAX_TAG){
        int x = DW1000Ranging.getPosX();
        int y = DW1000Ranging.getPosY();
        output.write<uint16_t>(x);
        output.write<uint16_t>(y);
    } else {
        int x = DW1000Ranging.getPosX(TAG_SHORT_ADDRESS[robotID]);
        int y = DW1000Ranging.getPosY(TAG_SHORT_ADDRESS[robotID]);
        output.write<uint16_t>(x);
        output.write<uint16_t>(y);
    }
}


// return true if pannel connected, false otherwise
void GET_PANEL_STATUS(SerialTalks &talks, Deserializer &input, Serializer &output){
    output.write<bool>(deviceConnected);
}

void CHANGE_CHANNEL(SerialTalks &talks, Deserializer &input, Serializer &output)
{
    if (data.channel > LONGDATA_RANGE_LOWPOWER)
        data.channel = (Channel)((uint16_t)data.channel -1);
    else
        data.channel = LONGDATA_RANGE_ACCURACY;
}