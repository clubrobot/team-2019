#include <Arduino.h>
#include "Arduino.h"
#include "../common/Experience.h"
#include "../common/TaskManager.h"
#include <BLEDevice.h>
#include <BLEClient.h>
#include <iostream>

#define BUILTIN_LED 2

#define ELECTRON_SERVICE_UUID       "9089189e-353f-44ec-9d84-a767862d3f6e"
#define ELECTRON_START_UUID         "413a617d-beb5-4932-9b3e-0cc79a1d56d6"

// The remote service we wish to connect to.
static BLEUUID serviceUUID(ELECTRON_SERVICE_UUID);

// The characteristic of the remote service we are interested in.
static BLEUUID startUUID(ELECTRON_START_UUID);

static BLEAddress *pServerAddress;

static BLEScan *pBLEScan;

static bool doConnect = false;
static bool connected = false;

BLERemoteCharacteristic *pStartCharacteristic;

Experience experience(true);

static void secondary_loop(void * parameters);
bool connectToServer(BLEAddress pAddress);

class ClientCallbacks : public BLEClientCallbacks
{
    void onDisconnect(BLEClient *pClient)
    {
        doConnect = false;
        connected = false;
        digitalWrite(BUILTIN_LED, LOW);
        pBLEScan->start(15,true);
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

    experience.setTimestep(0.1);
    experience.enable();
    experience.updateAnimation();

    BLEDevice::init("INSA_ELECTRON");

    pBLEScan = BLEDevice::getScan();
    pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
    pBLEScan->setActiveScan(true);
    pBLEScan->setInterval(100);
    pBLEScan->setWindow(99);  // less or equal setInterval value
    pBLEScan->start(15,true);
    
}

void loop()
{
    experience.update();
    if (doConnect == true)
    {
        if (connectToServer(*pServerAddress))
        {
            connected = true;
            digitalWrite(BUILTIN_LED, HIGH);
            experience.connected();
        }
        doConnect = false;
    }

    if (connected)
    {
        String result = pStartCharacteristic->readValue().c_str();
        
        if (result=="ON\0")
        {
            experience.start();
        }
    }

    vTaskDelay( 200 / portTICK_PERIOD_MS );   /* include 10 ms delay for better task management by ordonnancer */
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
    return true;
}