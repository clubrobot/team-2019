#include <Arduino.h>
#include "Arduino.h"
#include "../common/ExperienceEffects.h"
#include "../common/TaskManager.h"
#include <BLEDevice.h>
#include <BLEClient.h>

#define BUILTIN_LED 2

#define EXPERIENCE_SERVICE_UUID     "865d8713-2bf1-4081-9bc1-f009c532a1c7"
#define EXPERIENCE_START_UUID       "fbb86ffe-879f-4113-ac20-57b39d9b0f66"
#define EXPERIENCE_STATE_UUID       "dd603e58-bc55-4231-8dfc-52db9e91ba76"

// The remote service we wish to connect to.
static BLEUUID serviceUUID(EXPERIENCE_SERVICE_UUID);

// The characteristic of the remote service we are interested in.
static BLEUUID startUUID(EXPERIENCE_START_UUID);
static BLEUUID stateUUID(EXPERIENCE_STATE_UUID);

static BLEAddress *pServerAddress;

static bool doConnect = false;
static bool connected = false;

BLERemoteCharacteristic *pStartCharacteristic;
BLERemoteCharacteristic *pStateCharacteristic;

ExperienceEffects experience(false);

TaskManager task_manager;

static void secondary_loop(void * parameters);
bool connectToServer(BLEAddress pAddress);

class ClientCallbacks : public BLEClientCallbacks
{
    void onDisconnect(BLEClient *pClient)
    {
        doConnect = false;
        connected = false;
        digitalWrite(BUILTIN_LED, LOW);
    }

    void onConnect(BLEClient *pClient) 
    {
        digitalWrite(BUILTIN_LED, HIGH);
    }
};
/**
* Scan for BLE servers and find the first one that advertises the service we are looking for.
*/
class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks
{
    /**
     * Called for each advertising BLE server.
     */
    void onResult(BLEAdvertisedDevice advertisedDevice)
    {
    // We have found a device, let us now see if it contains the service we are looking for.
        if (advertisedDevice.haveServiceUUID() && advertisedDevice.getServiceUUID().equals(serviceUUID))
        {
            advertisedDevice.getScan()->stop();

            pServerAddress = new BLEAddress(advertisedDevice.getAddress());
            doConnect = true;
        }
    } 
};

void setup()
{   
    pinMode(BUILTIN_LED, OUTPUT);

    Serial.begin(115200);

    /* init task manager */
    task_manager.create_task(secondary_loop , NULL);

    /* setup experience */
    experience.setup();

    BLEDevice::init("INSA_ELECTRON");

    BLEScan *pBLEScan = BLEDevice::getScan();
    pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
    pBLEScan->setActiveScan(true);
    pBLEScan->setInterval(100);
    pBLEScan->setWindow(99);  // less or equal setInterval value
    pBLEScan->start(5,true);
}

void loop()
{
    if (doConnect == true)
    {
        if (connectToServer(*pServerAddress))
        {
            connected = true;
            experience.connected();
        }
        
        doConnect = false;
    }

    if (connected)
    {
        String result = pStartCharacteristic->readValue().c_str();
        
        if (result=="ON\0")
        {
            pStartCharacteristic->writeValue("RUN\0");
            experience.start();
        }
    }

    if ((experience.getStart() == 1)&& experience.getTimer()+TEMPS_MIN*1000 < millis() && experience.isElectron && connected)
    {
        experience.stayOnTop();
        pStateCharacteristic->writeValue("RUN\0");
    }
    
    vTaskDelay( 200 / portTICK_PERIOD_MS );   /* include 10 ms delay for better task management by ordonnancer */
}

static void secondary_loop(void * parameters)
{
    while(1)
    {
        if((digitalRead(GO_BACK) == LOW) && (experience.getStart() == 0))
        {
            experience.goBack();
        }
        else if ((digitalRead(GO_FORWARD) == LOW) && (experience.getStart() == 0))
        {
            experience.goForward();
        }
        else if (experience.getStart() == 0)
        {
            experience.motorStop();
        }
        
        vTaskDelay( 200 / portTICK_PERIOD_MS );   /* include 10 ms delay for better task management by ordonnancer */
    }
}

bool connectToServer(BLEAddress pAddress)
{
    pinMode(BUILTIN_LED, OUTPUT);

    BLEClient *pClient = BLEDevice::createClient();

    pClient->connect(pAddress);
    pClient->setClientCallbacks(new ClientCallbacks());

    BLERemoteService *pRemoteService = pClient->getService(serviceUUID);
    if (pRemoteService == nullptr)
    {
        return false;
    }

    pStartCharacteristic = pRemoteService->getCharacteristic(startUUID);
    if (pStartCharacteristic == nullptr) 
    {
        return false;
    }

    pStateCharacteristic = pRemoteService->getCharacteristic(stateUUID); 
    if (pStateCharacteristic == nullptr) 
    {
        return false;
    }
    return true;
}