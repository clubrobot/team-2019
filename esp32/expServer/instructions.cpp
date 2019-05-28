#include "instructions.h"
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>


extern BLECharacteristic *pStartElectronCharacteristic;
extern BLECharacteristic *pStartExperienceCharacteristic;

extern BLECharacteristic *pStateElectronCharacteristic;
extern BLECharacteristic *pStateExperienceCharacteristic;

extern boolean deviceConnected;

void IS_CONNECTED(SerialTalks &inst, Deserializer &input, Serializer &output)
{
    output.write<bool>(deviceConnected);
}

void START(SerialTalks &inst, Deserializer &input, Serializer &output)
{
    pStartElectronCharacteristic->setValue("ON\0");
    pStartElectronCharacteristic->notify(); 

    pStartExperienceCharacteristic->setValue("ON\0");
    pStartExperienceCharacteristic->notify(); 
}

void EXPERIENCE_STATE(SerialTalks &inst, Deserializer &input, Serializer &output)
{
    if(pStateExperienceCharacteristic->getValue().c_str()== "RUN\0")
    {
        output.write<bool>(1);
    }
    else
    {
        output.write<bool>(0);
    }
}
void ELECTRON_STATE(SerialTalks &inst, Deserializer &input, Serializer &output)
{
    if(pStateElectronCharacteristic->getValue().c_str()== "RUN\0")
    {
        output.write<bool>(1);
    }
    else
    {
        output.write<bool>(0);
    }
}

