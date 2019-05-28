#include <Arduino.h>
#include "../common/SerialTalks.h"
#include "../common/TaskManager.h"
#include "instructions.h"
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>

#define BUILTIN_LED 2

/* used for multiple device connection */
#define SERVICE_UUID            "9089189e-353f-44ec-9d84-a767862d3f6e"

#define START_UUID              "413a617d-beb5-4932-9b3e-0cc79a1d56d6"

#define STATE_EXPERIENCE_UUID   "dd603e58-bc55-4231-8dfc-52db9e91ba76"
#define STATE_ELECTRON_UUID     "cae98ae4-9c16-4426-98eb-50b3d4473d5c"

BLECharacteristic *pStartCharacteristic;

BLECharacteristic *pStateExpCharacteristic;
BLECharacteristic *pStateElecCharacteristic;

boolean deviceConnected = false;

TaskManager task_manager;

static void secondary_loop(void * parameters);

class MyServerCallbacks : public BLEServerCallbacks
{
    void onConnect(BLEServer *pServer)
    {
        deviceConnected = true;
        
        digitalWrite(BUILTIN_LED, HIGH);
        BLEDevice::startAdvertising();
    };

    void onDisconnect(BLEServer *pServer)
    {
        digitalWrite(BUILTIN_LED, LOW);
        deviceConnected = false;
    }
};

class MyCallback : public BLECharacteristicCallbacks
{
    void onRead(BLECharacteristic* pCharacteristic) 
    {
        // Do something before the read completes
    }
    void onWrite(BLECharacteristic* pCharacteristic) 
    {
        // Do something because a new value was written.
    }
};

void setup()
{
    pinMode(BUILTIN_LED, OUTPUT);

    BLEDevice::init("INSA_RENNES");
    BLEServer *pServer = BLEDevice::createServer();

    BLEService *pService = pServer->createService(SERVICE_UUID);

    pServer->setCallbacks(new MyServerCallbacks());

    pStartCharacteristic = pService->createCharacteristic(START_UUID,     \
                                            BLECharacteristic::PROPERTY_READ            \
                                            | BLECharacteristic::PROPERTY_WRITE         \
                                            | BLECharacteristic::PROPERTY_NOTIFY);

    
    pStateExpCharacteristic = pService->createCharacteristic(STATE_EXPERIENCE_UUID,       \
                                            BLECharacteristic::PROPERTY_READ            \
                                            | BLECharacteristic::PROPERTY_WRITE         \
                                            | BLECharacteristic::PROPERTY_NOTIFY);

    pStateElecCharacteristic = pService->createCharacteristic(STATE_ELECTRON_UUID,       \
                                            BLECharacteristic::PROPERTY_READ            \
                                            | BLECharacteristic::PROPERTY_WRITE         \
                                            | BLECharacteristic::PROPERTY_NOTIFY);


    pStartCharacteristic->setCallbacks(new MyCallbacks());
    // pStateExpCharacteristic->setCallbacks(new MyCallbacks());
    // pStateElecCharacteristic->setCallbacks(new MyCallbacks());

    pStartCharacteristic->setValue("0");
    // pStateExpCharacteristic->setValue("0");
    // pStateElecCharacteristic->setValue("0");

    pService->start();
    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();

    pAdvertising->addServiceUUID(SERVICE_UUID);
    pAdvertising->setScanResponse(true);
    pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
    pAdvertising->setMinPreferred(0x12);

    BLEDevice::startAdvertising();

    pStartCharacteristic->setValue("OFF\0");

    // pStateExpCharacteristic->setValue("STOP\0");
    // pStateElecCharacteristic->setValue("STOP\0");

    Serial.begin(SERIALTALKS_BAUDRATE);
    talks.begin(Serial);

    talks.bind(IS_CONNECTED_OPCODE, IS_CONNECTED);
    talks.bind(START_OPCODE, START);

    talks.bind(EXPERIENCE_STATE_OPCODE, EXPERIENCE_STATE);
    talks.bind(ELECTRON_STATE_OPCODE, ELECTRON_STATE);
}

void loop()
{
    talks.execute();
    vTaskDelay( 10 / portTICK_PERIOD_MS );
}

static void secondary_loop(void * parameters)
{
    BLEDevice::startAdvertising();
    vTaskDelay( 500 / portTICK_PERIOD_MS );
}