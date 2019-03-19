#include <Arduino.h>
#include "../common/Pickle.h"
#include "../common/SerialTalks.h"
#include "../common/ExperienceEffects.h"
#include "instructions.h"
#include <BLEDevice.h>
#include <BLEClient.h>

// The remote service we wish to connect to.
static BLEUUID serviceUUID("4fafc201-1fb5-459e-8fcc-c5c9c331914b");
// The characteristic of the remote service we are interested in.
static BLEUUID charUUID("beb5483e-36e1-4688-b7f5-ea07361b26a8");

static BLEAddress *pServerAddress;
static boolean doConnect = false;
static boolean connected = false;
static BLERemoteCharacteristic *pRemoteCharacteristic;

class ClientCallbacks : public BLEClientCallbacks
{
    void onDisconnect(BLEClient *pClient)
    {
        ESP.restart(); // TODO : find a better way to handle disconnections
    }

    void onConnect(BLEClient *pClient) {}
};

//TCPTalks talk("CLUB_ROBOT", "zigouigoui", "192.168.1.17", 26656);

ExperienceEffects Animation;

long current_time = 0;
long last_time = 0;

bool connectToServer(BLEAddress pAddress)
{
    BLEClient *pClient = BLEDevice::createClient();

    // Connect to the remote BLE Server.
    pClient->connect(pAddress);
    pClient->setClientCallbacks(new ClientCallbacks());

    // Obtain a reference to the service we are after in the remote BLE server.
    BLERemoteService *pRemoteService = pClient->getService(serviceUUID);
    if (pRemoteService == nullptr)
    {
        return false;
    }

    // Obtain a reference to the characteristic in the service of the remote BLE server.
    pRemoteCharacteristic = pRemoteService->getCharacteristic(charUUID);
    if (pRemoteCharacteristic == nullptr)
    {
        return false;
    }
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
            Serial.println("Connecting ... ");
        }
    } // onResult
};    // MyAdvertisedDeviceCallbacks

void setup()
{
    Serial.begin(115200);
    pinMode(2, OUTPUT);
    digitalWrite(2, HIGH);
    talks.begin(Serial);
    talks.bind(PING_OPCODE, PING);
    talks.bind(SET_START_OPCODE, SET_START);
    talks.bind(GET_START_OPCODE, GET_START);
    talks.bind(SET_TASK_DEBUG, SET_TASK);

}

void loop()
{
    talks.execute();
}
