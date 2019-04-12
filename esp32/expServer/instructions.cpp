#include "instructions.h"
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

extern BLECharacteristic *pStartCharacteristic;
extern BLECharacteristic *pIsOnTopCharacteristic;

void START(SerialTalks &inst, Deserializer &input, Serializer &output)
{
    pStartCharacteristic->setValue("start\0");
}

void ISONTOP(SerialTalks &inst, Deserializer &input, Serializer &output)
{
    if(pIsOnTopCharacteristic->getValue().c_str()=="top"){output.write<bool>(1);}
		else{output.write<bool>(0);}
}
