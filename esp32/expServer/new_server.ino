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
#define ELECTRON_SERVICE_UUID       "9089189e-353f-44ec-9d84-a767862d3f6e"
#define EXPERIENCE_SERVICE_UUID     "865d8713-2bf1-4081-9bc1-f009c532a1c7"

#define ELECTRON_START_UUID         "413a617d-beb5-4932-9b3e-0cc79a1d56d6"
#define EXPERIENCE_START_UUID       "fbb86ffe-879f-4113-ac20-57b39d9b0f66"

#define ELECTRON_STATE_UUID         "cae98ae4-9c16-4426-98eb-50b3d4473d5c"

#define EXPERIENCE_STATE_UUID       "dd603e58-bc55-4231-8dfc-52db9e91ba76"

BLEServer *pServer;

BLECharacteristic *pStartElectronCharacteristic;
BLECharacteristic *pStartExperienceCharacteristic;

BLECharacteristic *pStateElectronCharacteristic;
BLECharacteristic *pStateExperienceCharacteristic;

bool deviceConnected     = false;
bool previouslyConnected = false;

TaskManager task_manager;

static void secondary_loop(void * parameters);

class MyServerCallbacks : public BLEServerCallbacks
{
    void onConnect(BLEServer *pServer)
    {
        deviceConnected = true;
        
        digitalWrite(BUILTIN_LED, HIGH);
        //BLEDevice::startAdvertising();
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

    /* serial talks config */
    Serial.begin(SERIALTALKS_BAUDRATE);
    talks.begin(Serial);

    talks.bind(IS_CONNECTED_OPCODE, IS_CONNECTED);
    talks.bind(START_OPCODE, START);

    talks.bind(EXPERIENCE_STATE_OPCODE, EXPERIENCE_STATE);
    talks.bind(ELECTRON_STATE_OPCODE, ELECTRON_STATE);

    BLEDevice::init("INSA_RENNES");
    pServer = BLEDevice::createServer();

    pServer->setCallbacks(new MyServerCallbacks());

    BLEService *pElectronService    = pServer->createService(ELECTRON_SERVICE_UUID);
    BLEService *pExperienceService  = pServer->createService(EXPERIENCE_SERVICE_UUID);

    /***************** Electron Characteristics *****************/
    pStartElectronCharacteristic = pElectronService->createCharacteristic(ELECTRON_START_UUID,    \
                                                            BLECharacteristic::PROPERTY_READ    | \
                                                            BLECharacteristic::PROPERTY_WRITE   | \
                                                            BLECharacteristic::PROPERTY_NOTIFY);

    pStateElectronCharacteristic = pElectronService->createCharacteristic(ELECTRON_STATE_UUID,    \
                                                            BLECharacteristic::PROPERTY_READ    | \
                                                            BLECharacteristic::PROPERTY_WRITE   | \
                                                            BLECharacteristic::PROPERTY_NOTIFY);

    /***************** Experience Characteristics *****************/
    pStartExperienceCharacteristic = pElectronService->createCharacteristic(EXPERIENCE_START_UUID,\
                                                            BLECharacteristic::PROPERTY_READ    | \
                                                            BLECharacteristic::PROPERTY_WRITE   | \
                                                            BLECharacteristic::PROPERTY_NOTIFY);

    pStateExperienceCharacteristic = pElectronService->createCharacteristic(EXPERIENCE_STATE_UUID,\
                                                            BLECharacteristic::PROPERTY_READ    | \
                                                            BLECharacteristic::PROPERTY_WRITE   | \
                                                            BLECharacteristic::PROPERTY_NOTIFY);

    pStartElectronCharacteristic->setCallbacks(new MyCallback());
    pStateElectronCharacteristic->setCallbacks(new MyCallback());

    pStartExperienceCharacteristic->setCallbacks(new MyCallback());
    pStateExperienceCharacteristic->setCallbacks(new MyCallback());

    pStartElectronCharacteristic->setValue("OFF\0");
    pStartExperienceCharacteristic->setValue("OFF\0");

    pStateElectronCharacteristic->setValue("STOP\0");
    pStartExperienceCharacteristic->setValue("STOP\0");

    pElectronService->start();
    pExperienceService->start();

  /*  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(ELECTRON_SERVICE_UUID);
    pAdvertising->addServiceUUID(EXPERIENCE_SERVICE_UUID);
    pAdvertising->setScanResponse(true);
    pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
    pAdvertising->setMinPreferred(0x12);
    BLEDevice::startAdvertising(); */

    /* configure advertising */
    pServer->getAdvertising()->addServiceUUID(ELECTRON_SERVICE_UUID);
    pServer->getAdvertising()->addServiceUUID(EXPERIENCE_SERVICE_UUID);
    pServer->getAdvertising()->setScanResponse(true);
    pServer->getAdvertising()->setMinPreferred(0x06);  // functions that help with iPhone connections issue
    pServer->getAdvertising()->setMinPreferred(0x12);

    /* start advertising */
    pServer->getAdvertising()->start();

    /* init task manager */
    task_manager.create_task(secondary_loop , NULL);
}


void loop()
{
    talks.execute();
    vTaskDelay( 10 / portTICK_PERIOD_MS );
}

static void secondary_loop(void * parameters)
{
    if(!deviceConnected && previouslyConnected)
    {
        delay(500); // give the bluetooth stack the chance to get things ready   
        pServer->startAdvertising();
        previouslyConnected = deviceConnected;
    }
    if (deviceConnected && !previouslyConnected) 
    {    
        previouslyConnected = deviceConnected;
    }
    vTaskDelay( 500 / portTICK_PERIOD_MS );
}