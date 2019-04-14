#include <Arduino.h>
#include "../common/SerialTalks.h"
#include "instructions.h"
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>

#define SERVICE_UUID      "6a54cace-89bf-4e0b-a9f9-7f78a7aab1ef"
#define STARTCHAR_UUID    "bc4878c0-426a-45ed-b5db-2a9e1a1e43d7"
#define ISONTOP_UUID      "561a3414-8b23-462e-ab59-1cdb47905789"

BLECharacteristic *pStartCharacteristic;
BLECharacteristic *pIsOnTopCharacteristic;
boolean deviceConnected = false;

class MyServerCallbacks : public BLEServerCallbacks
{
  void onConnect(BLEServer *pServer)
  {
    deviceConnected = true;
    BLEDevice::startAdvertising();

  };

  void onDisconnect(BLEServer *pServer)
  {
    deviceConnected = false;
  }
};

class MyCallbacks : public BLECharacteristicCallbacks{
  void onWrite(BLECharacteristic *mCharacteristic){
    std::string rxValue = mCharacteristic->getValue();
    int dataLength = rxValue.length();
    Serial.print("Received value (" );
    Serial.print( dataLength );
    Serial.println( " bytes)");
    Serial.print("Received : ");
    char rxData[dataLength];
    for(int i = 0; i < dataLength; ++i){
      rxData[i] = rxValue[i];
      Serial.print(rxData[i]);
    }
    Serial.println("");
    //Serial.println(rxData);
  }
};

void setup()
{


  BLEDevice::init("Server");
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());
  BLEService *pService = pServer->createService(SERVICE_UUID);
  pStartCharacteristic = pService->createCharacteristic(STARTCHAR_UUID, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_NOTIFY);
  pIsOnTopCharacteristic = pService->createCharacteristic(ISONTOP_UUID, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_NOTIFY);

  pStartCharacteristic->setCallbacks(new MyCallbacks());
  pIsOnTopCharacteristic->setCallbacks(new MyCallbacks());
  pStartCharacteristic->setValue("0");
  pIsOnTopCharacteristic->setValue("0");
  pService->start();
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();

  Serial.begin(SERIALTALKS_BAUDRATE);
  talks.begin(Serial);

  talks.bind(START_OPCODE, START);
  talks.bind(ISONTOP_OPCODE, ISONTOP);


}

void loop()
{
   talks.execute();

}
