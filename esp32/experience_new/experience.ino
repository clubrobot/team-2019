#include <Arduino.h>
#include "Arduino.h"
#include "../common/ExperienceEffects.h"
#include "../common/TaskManager.h"
#include <BLEDevice.h>
#include <BLEClient.h>

// The remote service we wish to connect to.
static BLEUUID serviceUUID("6a54cace-89bf-4e0b-a9f9-7f78a7aab1ef");
// The characteristic of the remote service we are interested in.
static BLEUUID startCharUUID("bc4878c0-426a-45ed-b5db-2a9e1a1e43d7");
static BLEUUID isOnTopUUID("561a3414-8b23-462e-ab59-1cdb47905789");

static BLEAddress *pServerAddress;
static boolean doConnect = false;
static boolean connected = false;
static BLERemoteCharacteristic *pStartCharacteristic;
static BLERemoteCharacteristic *pIsOnTopCharacteristic;

ExperienceEffects experience(true);

TaskManager task_manager;

static TickType_t xDelay = 100 / portTICK_PERIOD_MS; // 100 ms task Delay

static void secondary_loop(void * parameters);


class ClientCallbacks : public BLEClientCallbacks
{
    void onDisconnect(BLEClient *pClient)
    {
        ESP.restart(); // TODO : find a better way to handle disconnections
    }

    void onConnect(BLEClient *pClient) 
    {
        experience.connected();
    }
};

bool connectToServer(BLEAddress pAddress)
{
    BLEClient *pClient = BLEDevice::createClient();

    pClient->connect(pAddress);
    pClient->setClientCallbacks(new ClientCallbacks());

    BLERemoteService *pRemoteService = pClient->getService(serviceUUID);
    if (pRemoteService == nullptr)
    {
        return false;
    }

    pStartCharacteristic = pRemoteService->getCharacteristic(startCharUUID);
    if (pStartCharacteristic == nullptr) 
    {
        return false;
    }

    if (experience.isElectron)
    {
        pIsOnTopCharacteristic = pRemoteService->getCharacteristic(isOnTopUUID);
        if (pIsOnTopCharacteristic == nullptr) 
        {
        return false;
        }
    }
  //pRemoteCharacteristic->registerForNotify(notifyCallback);
}
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
    Serial.begin(115200);

    /* init task manager */
    task_manager.create_task(secondary_loop , NULL);

    /* setup experience */
    experience.setup();

    BLEDevice::init("");
    BLEScan *pBLEScan = BLEDevice::getScan();
    pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
    pBLEScan->setActiveScan(true);
    pBLEScan->start(15);
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
        Serial.println(result);
        if (result=="start\0")
        {
            experience.start();
        }
    }

    if ((experience.getStart() == 1)&& experience.getTimer()+TEMPS_MIN*1000 < millis() && experience.isElectron && connected)
    {
        experience.stayOnTop();
        //pIsOnTopCharacteristic->writeValue("top");
    }
    
    vTaskDelay( xDelay );   /* include 10 ms delay for better task management by ordonnancer */
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
        
        vTaskDelay( xDelay );   /* include 10 ms delay for better task management by ordonnancer */
    }
}