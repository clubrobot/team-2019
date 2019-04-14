#include <Arduino.h>
#include "Arduino.h"
#include "../common/ExperienceEffects.h"
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

ExperienceEffects experience(false);

long current_time = 0;
long last_time = 0;

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
  Serial.println("client cree");
  // Connect to the remote BLE Server.
  pClient->connect(pAddress);
  pClient->setClientCallbacks(new ClientCallbacks());
  Serial.println("callbacksok");
  // Obtain a reference to the service we are after in the remote BLE server.
  BLERemoteService *pRemoteService = pClient->getService(serviceUUID);
  if (pRemoteService == nullptr)
  {
    return false;
  }
  Serial.println("getService");
  // Obtain a reference to the characteristic in the service of the remote BLE server.
  pStartCharacteristic = pRemoteService->getCharacteristic(startCharUUID);
  if (pStartCharacteristic == nullptr) {
    Serial.print("Failed to find our characteristic UUID: ");
    Serial.println(startCharUUID.toString().c_str());
    return false;
  }
  Serial.println(" - Found our characteristic");
  if (experience.isElectron){
    pIsOnTopCharacteristic = pRemoteService->getCharacteristic(isOnTopUUID);
    if (pIsOnTopCharacteristic == nullptr) {
      Serial.print("Failed to find our characteristic UUID: ");
      Serial.println(isOnTopUUID.toString().c_str());
      return false;
    }
  }
  // Read the value of the characteristic.
  std::string value = pStartCharacteristic->readValue();
  Serial.print("The characteristic value was: ");
  Serial.println(value.c_str());

  //pRemoteCharacteristic->registerForNotify(notifyCallback);
  Serial.println("ended");
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
    Serial.print("BLE Advertised Device found: ");
    Serial.println(advertisedDevice.toString().c_str());
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
  experience.setup();
  BLEDevice::init("");
  Serial.println("init BLE");
  BLEScan *pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true);
  pBLEScan->start(15);

}

void loop()
{
  if (doConnect == true)
  {
    if (connectToServer(*pServerAddress)){
      connected = true;
      experience.connected();}
    doConnect = false;
  }
  if (digitalRead(GO_BACK)==LOW){
    experience.goBack();
    while(digitalRead(GO_BACK)==LOW);
    experience.motorStop();
  }
  if (connected){
    String result = pStartCharacteristic->readValue().c_str();
    Serial.println(result);
    if (result=="start\0"){
      Serial.println("ok");
      experience.start();
    }
  }
  if (digitalRead(INTERRUPT) && experience.getTimer()+TEMPS_MIN*1000 < millis() && experience.isElectron && connected){
    experience.stayOnTop();
    pIsOnTopCharacteristic->writeValue("top");
  }

  delay(500);
}
