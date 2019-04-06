#include <Arduino.h>
#include "Arduino.h"
#include "../common/Pickle.h"
#include "../common/tcptalks.h"
#include "../common/ExperienceEffects.h"
#include "instructions.h"
#include <BLEDevice.h>
#include <BLEClient.h>
#include "../common/ESP32-Arduino-Servo-Library/src/Servo.h"
#include "../common/DCMotor.h"

// The remote service we wish to connect to.
static BLEUUID serviceUUID("4fafc201-1fb5-459e-8fcc-c5c9c331914b");
// The characteristic of the remote service we are interested in.
static BLEUUID charUUID("beb5483e-36e1-4688-b7f5-ea07361b26a8");

static BLEAddress *pServerAddress;
static boolean doConnect = false;
static boolean connected = false;
static BLERemoteCharacteristic *pRemoteCharacteristic;

String result = "";

DCMotor motor;
DCMotorsDriver driver;

TCPTalks talk("CLUB_ROBOT", "zigouigoui", "192.168.1.17", 26656);

ExperienceEffects Animation;

long current_time = 0;
long last_time = 0;

void start(){
  Animation.start();
  motor.enable();
}

static void notifyCallback(
  BLERemoteCharacteristic* pBLERemoteCharacteristic,
  uint8_t* pData,
  size_t length,
  bool isNotify) {
    Serial.print("Notify callback for characteristic ");
    Serial.print(pBLERemoteCharacteristic->getUUID().toString().c_str());
    Serial.print(" of data length ");
    Serial.println(length);
}

class ClientCallbacks : public BLEClientCallbacks
{
    void onDisconnect(BLEClient *pClient)
    {
        ESP.restart(); // TODO : find a better way to handle disconnections
    }

    void onConnect(BLEClient *pClient) {}
};

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
    pRemoteCharacteristic->registerForNotify(notifyCallback);
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
    talk.connect(500);
    talk.bind(PING_OPCODE, PING);
    talk.bind(SET_START_OPCODE, SET_START);
    talk.bind(GET_START_OPCODE, GET_START);
    talk.bind(SET_TASK_DEBUG, SET_TASK);
    motor.attach(27, 21, 0, 5000, 12);
    driver.attach(23, 0);
    driver.reset();
    Animation.setup();

    BLEDevice::init("");
    Serial.println("init BLE");
    // Retrieve a Scanner and set the callback we want to use to be informed when we
    // have detected a new device.  Specify that we want active scanning and start the
    // scan to run for 30 seconds.
    BLEScan *pBLEScan = BLEDevice::getScan();
    pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
    pBLEScan->setActiveScan(true);
    pBLEScan->start(15);

}

void loop()
{
    talk.execute();
    if((talk.is_connected() == false) && ((current_time - last_time)) > 500)
    {
        talk.connect(500);
        last_time = millis();
    }

    if (doConnect == true)
    {
        connectToServer(*pServerAddress);
        doConnect = false;
    }
    Serial.println(pRemoteCharacteristic->readValue());
}
