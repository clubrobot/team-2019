/**
 * 
 * @todo
 *  - move strings to flash (less RAM consumption)
 *  - fix deprecated convertation form string to char* startAsAnchor
 *  - give example description
 */

#include "pin.h"
#include "configuration.h"
#include <EEPROM.h>

#include <SPI.h>
#include "DW1000Ranging.h"
#include "DW1000.h"
#include "../common/dataSync.h"

#include "SSD1306.h"
#include <Wire.h>

#include "../../common/SerialTalks.h"
#include "instructions.h"

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>
#include "../common/OLED_display.h"


OLEDdisplay display(0x3C, PIN_SDA, PIN_SCL);

byte currentBeaconNumber = 1;
boolean calibrationRunning = false;
DataSync data = {GREEN, LONGDATA_RANGE_ACCURACY};

// BLE variables
BLECharacteristic *pCharacteristic;
boolean deviceConnected = false;

class MyServerCallbacks : public BLEServerCallbacks
{
  void onConnect(BLEServer *pServer)
  {
    //  Serial.println("connected");
    deviceConnected = true;
    display.log("panneau connecté");
  };

  void onDisconnect(BLEServer *pServer)
  {
    //  Serial.println("disconnected");
    deviceConnected = false;
    display.log("panneau déconnecté");
  }
};

void loopCore0(void *pvParameters) // loop on core 0
{
  for (;;)
  {
    display.update();
    talks.execute();
    delay(10);
  }
}

void newRange()
{
  DW1000Ranging.setRangeFilterValue(5);

  String toDisplay;

  if(calibrationRunning==true){
    display.log("timeOut");
  } else {
    // get master tag coordinates
    float x = DW1000Ranging.getPosX(TAG_SHORT_ADDRESS[0]) / 10;
    float y = DW1000Ranging.getPosY(TAG_SHORT_ADDRESS[0]) / 10;
    toDisplay = "(";
    toDisplay += (int)x;
    toDisplay += ", ";
    toDisplay += (int)y;
    toDisplay += ")\n";
    // get slave tag coordinates
    x = DW1000Ranging.getPosX(TAG_SHORT_ADDRESS[1]) / 10;
    y = DW1000Ranging.getPosY(TAG_SHORT_ADDRESS[1]) / 10;
    toDisplay += "(";
    toDisplay += (int)x;
    toDisplay += ", ";
    toDisplay += (int)y;
    toDisplay += ")\n";
  }

  display.displayMsg(Text(toDisplay, 3, 64, 0));
  digitalWrite(PIN_LED_OK, HIGH);
  digitalWrite(PIN_LED_FAIL, LOW);
}

void calibration(int realDistance, int mesure){
  static int lastErrors[3] = {100,100,100};
  static int errorIndex = 0;
  static uint16_t antennaDelay ;
  if(calibrationRunning==false){
    antennaDelay = DW1000.getAntennaDelay();
    calibrationRunning = true;
  }
  DW1000Ranging.setRangeFilterValue(15);
  mesure = sqrt(mesure * mesure - ((Z_HEIGHT[currentBeaconNumber] - Z_TAG) * (Z_HEIGHT[currentBeaconNumber] - Z_TAG))); // projection dans le plan des tags
  
  lastErrors[errorIndex++] = realDistance - mesure;
  
  if(errorIndex > 2){
    errorIndex = 0;
    int meanError = (lastErrors[0] + lastErrors[1] + lastErrors[2]) / 3;

    if (abs(meanError) < 1)
    { // end of calibration
      DW1000Ranging.stopCalibration();
      calibrationRunning = false;
      lastErrors[0] = 100;
      lastErrors[1] = 100;
      lastErrors[2] = 100;
      //EEPROM.write(EEPROM_ANTENNA_DELAY, antennaDelay >> 8);    // TODO bug potentiel ici (valeurs incohérentes à la relecture de l'eeprom)
      //EEPROM.write(EEPROM_ANTENNA_DELAY + 1, antennaDelay % 256);
      //EEPROM.commit();
      //ESP.restart();
    } else if (meanError < 0) {
      antennaDelay++;
      DW1000Class::setAntennaDelay(antennaDelay);
    } else {
      antennaDelay--;
      DW1000Class::setAntennaDelay(antennaDelay);
    }
  }
  
  String toDisplay = "target: ";
  toDisplay += realDistance;
  toDisplay += "mm\nmesure: ";
  toDisplay += mesure;
  toDisplay += "mm\ndelay: ";
  toDisplay += antennaDelay;
  display.displayMsg(Text(toDisplay, 6, 64, 0));
}

void newBlink(DW1000Device *device)
{
  int networkNumber = DW1000Ranging.getNetworkDevicesNumber();
  int tagNumber = DW1000Ranging.getTagDevicesNumber();

  String toDisplay = "ANC : ";
  toDisplay += networkNumber;
  toDisplay += "\nTAG : ";
  toDisplay += tagNumber;

  display.displayMsg(Text(toDisplay, 3, 64, 0));

  digitalWrite(PIN_LED_OK, HIGH);
  digitalWrite(PIN_LED_FAIL, LOW);
}

void inactiveDevice(DW1000Device *device)
{
  int networkNumber = DW1000Ranging.getNetworkDevicesNumber()-1;
  int tagNumber = DW1000Ranging.getTagDevicesNumber();

  String toDisplay = "ANC : ";
  toDisplay += networkNumber;
  toDisplay += "\nTAG : ";
  toDisplay += tagNumber;

  display.displayMsg(Text(toDisplay, 3, 64, 0));

  if (tagNumber + networkNumber == 0)
  {
    digitalWrite(PIN_LED_OK, LOW);
    digitalWrite(PIN_LED_FAIL, HIGH);
  }
}

void handleNewChannel(uint16_t channel){
  switch (channel)
  {
  case LONGDATA_RANGE_LOWPOWER:
  {
    DW1000Ranging.startAsAnchor("82:17:FC:87:0D:71:DC:75", DW1000.MODE_LONGDATA_RANGE_LOWPOWER, ANCHOR_SHORT_ADDRESS[currentBeaconNumber]);
    String s = "Channel ";
    s += channel;
    display.displayMsg(Text(s, 8, 64, 0));
  }
  break;
  case SHORTDATA_FAST_LOWPOWER:
  {
    DW1000Ranging.startAsAnchor("82:17:FC:87:0D:71:DC:75", DW1000.MODE_SHORTDATA_FAST_LOWPOWER, ANCHOR_SHORT_ADDRESS[currentBeaconNumber]);
    String s = "Channel ";
    s += channel;
    display.displayMsg(Text(s, 8, 64, 0));
  }
  break;
  case LONGDATA_FAST_LOWPOWER:
  {
    DW1000Ranging.startAsAnchor("82:17:FC:87:0D:71:DC:75", DW1000.MODE_LONGDATA_FAST_LOWPOWER, ANCHOR_SHORT_ADDRESS[currentBeaconNumber]);
   String s = "Channel ";
    s += channel;
    display.displayMsg(Text(s, 8, 64, 0));
  }
  break;
  case SHORTDATA_FAST_ACCURACY:
  {
    DW1000Ranging.startAsAnchor("82:17:FC:87:0D:71:DC:75", DW1000.MODE_SHORTDATA_FAST_ACCURACY, ANCHOR_SHORT_ADDRESS[currentBeaconNumber]);
    String s = "Channel ";
    s += channel;
    display.displayMsg(Text(s, 8, 64, 0));
  }
  break;
  case LONGDATA_FAST_ACCURACY:
  {
    DW1000Ranging.startAsAnchor("82:17:FC:87:0D:71:DC:75", DW1000.MODE_LONGDATA_FAST_ACCURACY, ANCHOR_SHORT_ADDRESS[currentBeaconNumber]);
   String s = "Channel ";
    s += channel;
    display.displayMsg(Text(s, 8, 64, 0));
  }
  break;
  case LONGDATA_RANGE_ACCURACY:
  {
    DW1000Ranging.startAsAnchor("82:17:FC:87:0D:71:DC:75", DW1000.MODE_LONGDATA_RANGE_ACCURACY, ANCHOR_SHORT_ADDRESS[currentBeaconNumber]);
    String s = "Channel ";
    s += channel;
    display.displayMsg(Text(s, 8, 64, 0));
  }
  break;
  default:
    String s = "Error\nchannel ";
    s += channel;
    display.displayMsg(Text(s, 8, 64, 0));
    break;
  }
}

void setup() {
  
  Serial.begin(SERIALTALKS_BAUDRATE);
  talks.begin(Serial);
  
  talks.bind(UPDATE_ANCHOR_NUMBER_OPCODE, UPDATE_ANCHOR_NUMBER);
  talks.bind(UPDATE_ANTENNA_DELAY_OPCODE, UPDATE_ANTENNA_DELAY);
  talks.bind(CALIBRATION_ROUTINE_OPCODE, CALIBRATION_ROUTINE);
  talks.bind(UPDATE_COLOR_OPCODE, UPDATE_COLOR);
  talks.bind(GET_COORDINATE_OPCODE,GET_COORDINATE);
  talks.bind(GET_PANEL_STATUS_OPCODE, GET_PANEL_STATUS);
  talks.bind(CHANGE_CHANNEL_OPCODE, CHANGE_CHANNEL);

  /*if (!EEPROM.begin(EEPROM_SIZE))   // Already done in serialtalks lib
  {
    //Serial.println("failed to initialise EEPROM");
    delay(1000000);
  }*/

  #if 0
  EEPROM.write(EEPROM_NUM_ANCHOR, currentBeaconNumber);
  EEPROM.commit();
  #endif
      currentBeaconNumber = EEPROM.read(EEPROM_NUM_ANCHOR);

  // init communication
  DW1000Ranging.initCommunication(PIN_UWB_RST, PIN_SPICSN, PIN_IRQ, PIN_SPICLK, PIN_SPIMISO, PIN_SPIMOSI); //Reset, CS, IRQ pin
  DW1000Ranging.attachNewRange(newRange);
  DW1000Ranging.attachBlinkDevice(newBlink);
  DW1000Ranging.attachInactiveAncDevice(inactiveDevice);  // TODO : rename func
  DW1000Ranging.attachAutoCalibration(calibration);
  DW1000Ranging.attachNewChannel(handleNewChannel);

  unsigned int replyTime;
  switch (currentBeaconNumber){
    case 0 :
      replyTime = 7000;
      break;
    case 1:
      replyTime = 21000;
      break;
    case 2:
      replyTime = 35000;
      break;
    case 3:
      replyTime = 49000;
      break;
  }
  
  DW1000Ranging.setReplyTime(replyTime);
  //Enable the filter to smooth the distance
  DW1000Ranging.useRangeFilter(true);
  DW1000Ranging.setRangeFilterValue(5);

  int antennaDelay = ANTENNA_DELAY[currentBeaconNumber];
  #if 0
  EEPROM.write(EEPROM_ANTENNA_DELAY, antennaDelay >> 8);
  EEPROM.write(EEPROM_ANTENNA_DELAY + 1, antennaDelay % 256);
  EEPROM.commit();
  #endif
 // antennaDelay = (EEPROM.read(EEPROM_ANTENNA_DELAY) << 8) + EEPROM.read(EEPROM_ANTENNA_DELAY+1);
  DW1000Class::setAntennaDelay(antennaDelay); //16384 for tag, approximately 16530 for anchors

  //we start the module as an anchor
  DW1000Ranging.startAsAnchor("82:17:FC:87:0D:71:DC:75", DW1000.MODE_LONGDATA_RANGE_ACCURACY, ANCHOR_SHORT_ADDRESS[currentBeaconNumber]);

  display.init();
  display.flipScreenVertically();
  display.setTextAlignment(TEXT_ALIGN_CENTER );

  xTaskCreatePinnedToCore(
      loopCore0,   /* Function to implement the task */
      "loopCore0", /* Name of the task */
      10000,       /* Stack size in words */
      NULL,        /* Task input parameter */
      0,           /* Priority of the task */
      NULL,        /* Task handle. */
      0);          /* Core where the task should run */

  pinMode(PIN_LED_FAIL,OUTPUT);
  pinMode(PIN_LED_OK,OUTPUT);
  digitalWrite(PIN_LED_OK,HIGH);
  digitalWrite(PIN_LED_FAIL,HIGH);

  display.displayMsg(Text("SYNC", 3, 64, 0));

  // Start BLE Server only if this is the supervisor anchor
  if (ANCHOR_SHORT_ADDRESS[currentBeaconNumber] == BEACON_BLE_ADDRESS)
  {
    DW1000Ranging.setDataSync(&data);
    DW1000Ranging.setDataSyncSize(sizeof(data));
    display.log(data.color==GREEN?"green":"orange");

    BLEDevice::init("srv");
    BLEServer *pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks());
    BLEService *pService = pServer->createService(SERVICE_UUID);
    pCharacteristic = pService->createCharacteristic(
        CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_READ |
            BLECharacteristic::PROPERTY_WRITE |
            BLECharacteristic::PROPERTY_NOTIFY);
    pCharacteristic->setValue("insa rennes");
    pService->start();
    BLEAdvertising *pAdvertising = pServer->getAdvertising();
    pAdvertising->addServiceUUID(pService->getUUID());
    pAdvertising->start();

  } else {
    String s = "ANCHOR\n";
    s += currentBeaconNumber;
    display.displayMsg(Text(s, 4, 64, 0));
  }
}

void loop() {
  DW1000Ranging.loop();
}


