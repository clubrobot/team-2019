#include "instructions.h"
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>


extern BLECharacteristic *pStartEXPCharacteristic;
extern BLECharacteristic *pStartELECharacteristic;

extern boolean deviceConnected;

void IS_CONNECTED(SerialTalks &inst, Deserializer &input, Serializer &output)
{
    output.write<bool>(deviceConnected);
}

void START(SerialTalks &inst, Deserializer &input, Serializer &output)
{
    pStartEXPCharacteristic->setValue("ON\0");
    pStartEXPCharacteristic->notify(); 
    
    pStartELECharacteristic->setValue("ON\0");
    pStartELECharacteristic->notify(); 
}

void EXPERIENCE_STATE(SerialTalks &inst, Deserializer &input, Serializer &output)
{
    // if(pStateCharacteristic->getValue().c_str()[1]== '1')
    // {
    //     output.write<bool>(1);
    // }
    // else
    // {
    //     output.write<bool>(0);
    // }
}
void ELECTRON_STATE(SerialTalks &inst, Deserializer &input, Serializer &output)
{
    // if(pStateCharacteristic->getValue().c_str()[0]== '1')
    // {
    //     output.write<bool>(1);
    // }
    // else
    // {
    //     output.write<bool>(0);
    // }
}

