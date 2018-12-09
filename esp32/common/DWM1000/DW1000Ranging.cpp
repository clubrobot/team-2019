/*
 * Copyright (c) 2015 by Thomas Trojer <thomas@trojer.net> and Leopold Sayous <leosayous@gmail.com>
 * Decawave DW1000 library for arduino.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * @file DW1000Ranging.h
 * Arduino global library (source file) working with the DW1000 library 
 * for the Decawave DW1000 UWB transceiver IC.
 *
 * @TODO
 * - remove or debugmode for //Serial.print
 * - move strings to flash to reduce ram usage
 * - do not safe duplicate of pin settings
 * - maybe other object structure
 * - use enums instead of preprocessor constants
 */


#include "DW1000Ranging.h"
#include "DW1000Device.h"

DW1000RangingClass DW1000Ranging;


//other devices we are going to communicate with which are on our network:
DW1000Device DW1000RangingClass::_networkDevices[MAX_DEVICES];
byte         DW1000RangingClass::_currentAddress[8];
byte         DW1000RangingClass::_currentShortAddress[2];
byte         DW1000RangingClass::_lastSentToShortAddress[2];
volatile uint8_t DW1000RangingClass::_networkDevicesNumber = 0; // TODO short, 8bit?
int16_t      DW1000RangingClass::_lastDistantDevice    = 0; // TODO short, 8bit?
DW1000Mac    DW1000RangingClass::_globalMac;

//other tags in the network
DW1000Device DW1000RangingClass::_tagDevices[MAX_TAG_DEVICES];
byte DW1000RangingClass::_masterTagShortAddress[2];
volatile uint8_t DW1000RangingClass::_tagDevicesNumber = 0;
boolean DW1000RangingClass::_isMasterTag = false;
boolean DW1000RangingClass::_isEnabled = _isMasterTag; // is true at start for master tag
int DW1000RangingClass::_enabledTagNumber = 0;
boolean DW1000RangingClass::_waitingSyncAck = false;

//module type (anchor or tag)
int16_t      DW1000RangingClass::_type; // TODO enum??

// message flow state
volatile byte    DW1000RangingClass::_expectedMsgId;

// range filter
volatile boolean DW1000RangingClass::_useRangeFilter = false;
uint16_t DW1000RangingClass::_rangeFilterValue = 15;

// message sent/received state
volatile boolean DW1000RangingClass::_sentAck     = false;
volatile boolean DW1000RangingClass::_receivedAck = false;

// protocol error state
boolean          DW1000RangingClass::_protocolFailed = false;

// timestamps to remember
int32_t            DW1000RangingClass::timer           = 0;
int16_t            DW1000RangingClass::counterForBlink = 0; // TODO 8 bit?
int16_t            DW1000RangingClass::counterForinactivity = 0;
int16_t			   DW1000RangingClass::counterForSync  = 0;

// for auto calibration
boolean DW1000RangingClass::_calibrate = false;
int DW1000RangingClass::_realDistance = 0;
unsigned long DW1000RangingClass::_startCalibrationTime = 0;
unsigned long DW1000RangingClass::_calibrationTimeOut = 0;

// trilateration
float DW1000RangingClass::_pos_x[MAX_TAG_DEVICES] = {-1000, -1000, -1000};
float DW1000RangingClass::_pos_y[MAX_TAG_DEVICES] = {-1000, -1000, -1000};

// others
uint8_t DW1000RangingClass::_dataSyncSize = 0;
void *DW1000RangingClass::_dataSync = NULL;

// data buffer
byte          DW1000RangingClass::data[LEN_DATA];
// reset line to the chip
uint8_t   DW1000RangingClass::_RST;
uint8_t   DW1000RangingClass::_SS;
// watchdog and reset period
uint32_t  DW1000RangingClass::_lastActivity;
uint32_t  DW1000RangingClass::_resetPeriod;
// reply times (same on both sides for symm. ranging)
uint16_t  DW1000RangingClass::_replyDelayTimeUS;
//timer delay
uint16_t  DW1000RangingClass::_timerDelay;
// ranging counter (per second)
uint16_t DW1000RangingClass::_rangingFrameRate = 0;
uint16_t DW1000RangingClass::_successRangingCount = 0;
uint32_t DW1000RangingClass::_rangingCountPeriod = 1000;
uint32_t DW1000RangingClass::_frameRateStartTime = millis();
//Here our handlers
void (* DW1000RangingClass::_handleNewRange)(void) = 0;
void (* DW1000RangingClass::_handleBlinkDevice)(DW1000Device*) = 0;
void (* DW1000RangingClass::_handleNewDevice)(DW1000Device*) = 0;
void (* DW1000RangingClass::_handleInactiveAncDevice)(DW1000Device*) = 0;
void (* DW1000RangingClass::_handleInactiveTagDevice)(DW1000Device*) = 0;
void (* DW1000RangingClass::_handleCalibration)(int,int) = 0;
void (* DW1000RangingClass::_handleDataSync)() = 0;
void (* DW1000RangingClass::_handleNewChannel)(uint16_t) = 0;

/* ###########################################################################
 * #### Debug ################################################################
 * ######################################################################### */

void DW1000RangingClass::log(String text)
{
#if DEBUG
	Serial.print(_isMasterTag);
	Serial.print(" ");
	Serial.print(_isEnabled);
	Serial.print(" ");
	Serial.print(_waitingSyncAck);
	Serial.print(" ");
	Serial.print(_networkDevicesNumber);
	Serial.print(" ");
	Serial.print(_tagDevicesNumber);
	Serial.print(" : ");
	Serial.println(text);
#endif
}


/* ###########################################################################
 * #### Init and end #######################################################
 * ######################################################################### */

void DW1000RangingClass::initCommunication(uint8_t myRST, uint8_t mySS, uint8_t myIRQ, int8_t sck, int8_t miso, int8_t mosi)
{
	// reset line to the chip
	_RST              = myRST;
	_SS               = mySS;
	_resetPeriod      = DEFAULT_RESET_PERIOD;
	// reply times (same on both sides for symm. ranging)
	_replyDelayTimeUS = DEFAULT_REPLY_DELAY_TIME;
	//we set our timer delay
	_timerDelay       = DEFAULT_TIMER_DELAY;
	
	
	DW1000.begin(myIRQ, sck, miso, mosi, myRST);
	DW1000.select(mySS);
}


void DW1000RangingClass::configureNetwork(uint16_t deviceAddress, uint16_t networkId, const byte mode[]) {
	// general configuration
	DW1000.newConfiguration();
	DW1000.setDefaults();
	DW1000.setDeviceAddress(deviceAddress);
	DW1000.setNetworkId(networkId);
	DW1000.enableMode(mode);
	DW1000.commitConfiguration();
	
}

void DW1000RangingClass::generalStart() {
	// attach callback for (successfully) sent and received messages
	DW1000.attachSentHandler(handleSent);
	DW1000.attachReceivedHandler(handleReceived);
	// anchor starts in receiving mode, awaiting a ranging poll message
	
	
	if(DEBUG) {
		// DEBUG monitoring
		//Serial.println("DW1000-arduino");
		// initialize the driver
		
		
		//Serial.println("configuration..");
		// DEBUG chip info and registers pretty printed
		char msg[90];
		DW1000.getPrintableDeviceIdentifier(msg);
		//Serial.print("Device ID: ");
		//Serial.println(msg);
		DW1000.getPrintableExtendedUniqueIdentifier(msg);
		//Serial.print("Unique ID: ");
		//Serial.print(msg);
		char string[6];
		sprintf(string, "%02X:%02X", _currentShortAddress[0], _currentShortAddress[1]);
		//Serial.print(" short: ");
		//Serial.println(string);
		
		DW1000.getPrintableNetworkIdAndShortAddress(msg);
		//Serial.print("Network ID & Device Address: ");
		//Serial.println(msg);
		DW1000.getPrintableDeviceMode(msg);
		//Serial.print("Device mode: ");
		//Serial.println(msg);
	}
	
	
	// anchor starts in receiving mode, awaiting a ranging poll message
	receiver();
}


void DW1000RangingClass::startAsAnchor(char address[], const byte mode[], const byte anchorAddress) {
	//save the address
	DW1000.convertToByte(address, _currentAddress);
	//write the address on the DW1000 chip
	DW1000.setEUI(address);
	//Serial.print("device address: ");
	//Serial.println(address);
	_currentShortAddress[0] = anchorAddress;
	_currentShortAddress[1] = 0;
	
	//we configur the network for mac filtering
	//(device Address, network ID, frequency)
	DW1000Ranging.configureNetwork(_currentShortAddress[0]*256+_currentShortAddress[1], 0xDECA, mode);
	
	//general start:
	generalStart();
	
	//defined type as anchor
	_type = ANCHOR;
	
	//Serial.println("### ANCHOR ###");
	
}

void DW1000RangingClass::startAsTag(char address[], const byte mode[], const byte tagAddress, const bool isMasterTag) {
	//save the address
	DW1000.convertToByte(address, _currentAddress);
	//write the address on the DW1000 chip
	DW1000.setEUI(address);
	//Serial.print("device address: ");
	//Serial.println(address);
	_currentShortAddress[0] = tagAddress;
	_currentShortAddress[1] = 0;

	//we configur the network for mac filtering
	//(device Address, network ID, frequency)
	DW1000Ranging.configureNetwork(_currentShortAddress[0]*256+_currentShortAddress[1], 0xDECA, mode);
	
	_isMasterTag = isMasterTag;
	_isEnabled = _isMasterTag;
	generalStart();
	//defined type as tag
	_type = TAG;
	
	//Serial.println("### TAG ###");
}

boolean DW1000RangingClass::addNetworkDevices(DW1000Device* device, boolean shortAddress) {
	boolean   addDevice = true;
	//we test our network devices array to check
	//we don't already have it
	for(uint8_t i = 0; i < _networkDevicesNumber; i++) {
		if(_networkDevices[i].isAddressEqual(device) && !shortAddress) {
			//the device already exists
			addDevice = false;
			return false;
		}
		else if(_networkDevices[i].isShortAddressEqual(device) && shortAddress) {
			//the device already exists
			addDevice = false;
			return false;
		}
		
	}
	
	if(addDevice) {
		String s = "add network device : ";
		s += device->getShortAddress();
		log(s);
		device->setRange(0);
		memcpy(&_networkDevices[_networkDevicesNumber], device, sizeof(DW1000Device));
		_networkDevices[_networkDevicesNumber].setIndex(_networkDevicesNumber);
		_networkDevicesNumber++;
		return true;
	}
	
	return false;
}

boolean DW1000RangingClass::addNetworkDevices(DW1000Device* device) {
	boolean addDevice = true;
	//we test our network devices array to check
	//we don't already have it
	for(uint8_t i = 0; i < _networkDevicesNumber; i++) {
		if(_networkDevices[i].isAddressEqual(device) && _networkDevices[i].isShortAddressEqual(device)) {
			//the device already exists
			addDevice = false;
			return false;
		}
		
	}
	
	if(addDevice) {
		/*if(_type == ANCHOR) //for now let's start with 1 TAG
		{
			_networkDevicesNumber = 0;
		}*/
		String s = "add network device : ";
		s += device->getShortAddress();
		log(s);

		memcpy(&_networkDevices[_networkDevicesNumber], device, sizeof(DW1000Device));
		_networkDevices[_networkDevicesNumber].setIndex(_networkDevicesNumber);
		_networkDevicesNumber++;
		return true;
	}
	
	return false;
}

boolean DW1000RangingClass::addTagDevices(DW1000Device *device, boolean shortAddress)
{
	boolean addDevice = true;
	//we test our network devices array to check
	//we don't already have it
	for (uint8_t i = 0; i < _tagDevicesNumber; i++)
	{
		if (_tagDevices[i].isAddressEqual(device) && !shortAddress)
		{
			//the device already exists
			addDevice = false;
			return false;
		}
		else if (_tagDevices[i].isShortAddressEqual(device) && shortAddress)
		{
			//the device already exists
			addDevice = false;
			return false;
		}
	}

	if (addDevice)
	{
		String s = "add new tag : ";
		s += device->getShortAddress();
		log(s);

		device->setRange(0);
		memcpy(&_tagDevices[_tagDevicesNumber], device, sizeof(DW1000Device));
		_tagDevices[_tagDevicesNumber].setIndex(_tagDevicesNumber);
		_tagDevicesNumber++;
		return true;
	}

	return false;
}

boolean DW1000RangingClass::addTagDevices(DW1000Device *device)
{
	boolean addDevice = true;
	//we test our network devices array to check
	//we don't already have it
	for (uint8_t i = 0; i < _tagDevicesNumber; i++)
	{
		if (_tagDevices[i].isAddressEqual(device) && _tagDevices[i].isShortAddressEqual(device))
		{
			//the device already exists
			addDevice = false;
			return false;
		}
	}

	if (addDevice)
	{
		String s = "add new tag : ";
		s += device->getShortAddress();
		log(s);

		memcpy(&_tagDevices[_tagDevicesNumber], device, sizeof(DW1000Device));
		_tagDevices[_tagDevicesNumber].setIndex(_tagDevicesNumber);
		_tagDevicesNumber++;
		return true;
	}

	return false;
}

void DW1000RangingClass::removeNetworkDevices(int16_t index) {

	log("Remove anchor because of inactivity");
	//if we have just 1 element
	if(_networkDevicesNumber == 1) {
		_networkDevicesNumber = 0;
	}
	else if(index == _networkDevicesNumber-1) //if we delete the last element
	{
		_networkDevicesNumber--;
	}
	else {
		//we translate all the element wich are after the one we want to delete.
		for(int16_t i = index; i < _networkDevicesNumber-1; i++) { // TODO 8bit?
			memcpy(&_networkDevices[i], &_networkDevices[i+1], sizeof(DW1000Device));
			_networkDevices[i].setIndex(i);
		}
		_networkDevicesNumber--;
	}
}

void DW1000RangingClass::removeTagDevices(int16_t index)
{
	log("Remove tag because of inactivity");
	//if we have just 1 element
	if (_tagDevicesNumber == 1)
	{
		_tagDevicesNumber = 0;
	}
	else if (index == _tagDevicesNumber - 1) //if we delete the last element
	{
		_tagDevicesNumber--;
	}
	else
	{
		//we translate all the element wich are after the one we want to delete.
		for (int16_t i = index; i < _tagDevicesNumber - 1; i++)
		{ // TODO 8bit?
			memcpy(&_tagDevices[i], &_tagDevices[i + 1], sizeof(DW1000Device));
			_tagDevices[i].setIndex(i);
		}
		_tagDevicesNumber--;
	}
}

/* ###########################################################################
 * #### Setters and Getters ##################################################
 * ######################################################################### */

//setters
void DW1000RangingClass::setReplyTime(uint16_t replyDelayTimeUs) { _replyDelayTimeUS = replyDelayTimeUs; }

void DW1000RangingClass::setResetPeriod(uint32_t resetPeriod) { _resetPeriod = resetPeriod; }


DW1000Device* DW1000RangingClass::searchDistantDevice(byte shortAddress[]) {
	//we compare the 2 bytes address with the others
	for(uint16_t i = 0; i < _networkDevicesNumber; i++) { // TODO 8bit?
		if(memcmp(shortAddress, _networkDevices[i].getByteShortAddress(), 2) == 0) {
			//we have found our device !
			return &_networkDevices[i];
		}
	}
	for(uint16_t i = 0; i < _tagDevicesNumber; i++) { // TODO 8bit?
		if(memcmp(shortAddress, _tagDevices[i].getByteShortAddress(), 2) == 0) {
			//we have found our device !
			return &_tagDevices[i];
		}
	}
	
	return NULL;
}

DW1000Device* DW1000RangingClass::getDistantDevice() {
	//we get the device which correspond to the message which was sent (need to be filtered by MAC address)
	
	return &_networkDevices[_lastDistantDevice];
	
}


/* ###########################################################################
 * #### Public methods #######################################################
 * ######################################################################### */

void DW1000RangingClass::checkForReset() {
	uint32_t curMillis = millis();
	if(!_sentAck && !_receivedAck) {
		// check if inactive
		if(curMillis-_lastActivity > _resetPeriod) {
			resetInactive();
		}
		return; // TODO cc
	}
}

void DW1000RangingClass::checkForInactiveDevices() {
	for(uint8_t i = 0; i < _networkDevicesNumber; i++) {
		if(_networkDevices[i].isInactive()) {
			if(_handleInactiveAncDevice != 0) {
				(*_handleInactiveAncDevice)(&_networkDevices[i]);
			}
			//we need to delete the device from the array:
			removeNetworkDevices(i);
			
		}
	}
	for(uint8_t i = 0; i < _tagDevicesNumber; i++) {
		if(_tagDevices[i].isInactive()) {
			if(_handleInactiveTagDevice != 0) {
				(*_handleInactiveTagDevice)(&_tagDevices[i]);
			}
			if (i == _enabledTagNumber - 1)
				_waitingSyncAck = false;
			//we need to delete the device from the array:
			removeTagDevices(i);
			
		}
	}
}

// TODO check return type
int16_t DW1000RangingClass::detectMessageType(byte datas[]) {
	if(datas[0] == FC_1_BLINK) {
		return BLINK;
	}
	else if(datas[0] == FC_1 && datas[1] == FC_2) {
		//we have a long MAC frame message (ranging init)
		return datas[LONG_MAC_LEN];
	}
	else if(datas[0] == FC_1 && datas[1] == FC_2_SHORT) {
		//we have a short mac frame message (poll, range, range report, etc..)
		return datas[SHORT_MAC_LEN];
	}
}

void DW1000RangingClass::loop() {
	//we check if needed to reset !
	checkForReset();
	uint32_t time = millis(); // TODO other name - too close to "timer"
	if(time-timer > _timerDelay) {
		timer = time;
		timerTick();
	}
	
	// frameRate update
	updateRangingCounter();

	if(_sentAck) {
		_sentAck = false;
		
		// TODO cc
		int messageType = detectMessageType(data);
		
		if(messageType != POLL_ACK && messageType != POLL && messageType != RANGE)
			return;
		
		//A msg was sent. We launch the ranging protocole when a message was sent
		if(_type == ANCHOR) {
			if(messageType == POLL_ACK) {
				DW1000Device* myDistantDevice = searchDistantDevice(_lastSentToShortAddress);
				
				DW1000.getTransmitTimestamp(myDistantDevice->timePollAckSent);
			}
		}
		else if(_type == TAG) {
			if(messageType == POLL) {
				DW1000Time timePollSent;
				DW1000.getTransmitTimestamp(timePollSent);
				//if the last device we send the POLL is broadcast:
				if(_lastSentToShortAddress[0] == 0xFF && _lastSentToShortAddress[1] == 0xFF) {
					//we save the value for all the devices !
					for(uint16_t i = 0; i < _networkDevicesNumber; i++) {
						_networkDevices[i].timePollSent = timePollSent;
					}
				}
				else {
					//we search the device associated with the last send address
					DW1000Device* myDistantDevice = searchDistantDevice(_lastSentToShortAddress);
					//we save the value just for one device
					myDistantDevice->timePollSent = timePollSent;
				}
			}
			else if(messageType == RANGE) {
				DW1000Time timeRangeSent;
				DW1000.getTransmitTimestamp(timeRangeSent);
				//if the last device we send the POLL is broadcast:
				if(_lastSentToShortAddress[0] == 0xFF && _lastSentToShortAddress[1] == 0xFF) {
					//we save the value for all the devices !
					for(uint16_t i = 0; i < _networkDevicesNumber; i++) {
						_networkDevices[i].timeRangeSent = timeRangeSent;
					}
				}
				else {
					//we search the device associated with the last send address
					DW1000Device* myDistantDevice = searchDistantDevice(_lastSentToShortAddress);
					//we save the value just for one device
					myDistantDevice->timeRangeSent = timeRangeSent;
				}
				
			}
		}
		
	}
	
	//check for new received message
	if(_receivedAck) {
		_receivedAck = false;
		/*Serial.print("Network devices : ");
		Serial.println(_networkDevicesNumber);
		for(int i = 0; i< _networkDevicesNumber;i++){
			uint16_t address = _networkDevices[i].getShortAddress();
			Serial.println(address);
		}
		Serial.print("Tag devices : ");
		Serial.println(_tagDevicesNumber);
		for (int i = 0; i < _tagDevicesNumber; i++)
		{
			uint16_t address = _tagDevices[i].getShortAddress();
			Serial.println(address);
		}*/
		//we read the datas from the modules:
		// get message and parse
		DW1000.getData(data, LEN_DATA);
		
		int messageType = detectMessageType(data);
		
		//we have just received a BLINK message from tag
		if(messageType == BLINK && _type == ANCHOR) {
			byte address[8];
			byte shortAddress[2];
			_globalMac.decodeBlinkFrame(data, address, shortAddress);
			//we create a new device with th tag
			DW1000Device myTag(address, shortAddress);
			
			if(addNetworkDevices(&myTag)) {
				if(_handleBlinkDevice != 0) {
					(*_handleBlinkDevice)(&myTag);
				}
				//we reply by the transmit ranging init message
				transmitRangingInit(&myTag);
				noteActivity();
			}
			_expectedMsgId = POLL;
		}
		else if (messageType == BLINK && _type == TAG) //we have just received a BLINK message from another tag in the network
		{
			byte address[8];
			byte shortAddress[2];
			_globalMac.decodeBlinkFrame(data, address, shortAddress);
			//we crate a new device with th tag
			DW1000Device myTag(address, shortAddress);

			if (addTagDevices(&myTag))
			{
				if (_handleBlinkDevice != 0)
				{
					(*_handleBlinkDevice)(&myTag);
				}

				noteActivity();
			}
		}
		else if(messageType == RANGING_INIT && _type == TAG) {
			log("RANGING INIT");
			byte address[2];
			_globalMac.decodeLongMACFrame(data, address);
			//we crate a new device with the anchor
			DW1000Device myAnchor(address, true);
			
			if(addNetworkDevices(&myAnchor, true)) {
				if(_handleNewDevice != 0) {
					(*_handleNewDevice)(&myAnchor);
				}
			}
			
			noteActivity();
		}
		else {
			//we have a short mac layer frame !
			byte address[2];
			_globalMac.decodeShortMACFrame(data, address);
			
			
			
			//we get the device which correspond to the message which was sent (need to be filtered by MAC address)
			DW1000Device* myDistantDevice = searchDistantDevice(address);

			if (((_networkDevicesNumber != 0) || (_tagDevicesNumber != 0)) && (myDistantDevice == NULL))
			{
				//Serial.println("Not found");
				//we don't have the short address of the device in memory
				/*
				//Serial.print("unknown: ");
				//Serial.print(address[0], HEX);
				//Serial.print(":");
				//Serial.println(address[1], HEX);
				*/
				return;
			}
			
			if(myDistantDevice == NULL){
				log("NULL ptr instead of device !");
				return;
			}
				
			
			//then we proceed to range protocole
			if(_type == ANCHOR) {
				if(messageType == CHANGE_CHANNEL_MODE){
					log("CHANGE_CHANNEL_MODE");
					uint16_t channel;
					memcpy(&channel, data + SHORT_MAC_LEN + 1, 2);
					String s = "change channel to channel ";
					s+= channel;
					log(s);
					if(_handleNewChannel != 0)
						(*_handleNewChannel)(channel);
					return;
				}else if(messageType != _expectedMsgId) {
					// unexpected message, start over again (except if already POLL)

					String s = "IGNORED Unexppected msg : ";
					s+=messageType;
					s+=" expected : ";
					s+=_expectedMsgId;
					log(s);
					_protocolFailed = true;
				}
				if(messageType == POLL) {
					String s = "POLL : ";
					//we receive a POLL which is a broacast message
					//we need to grab info about it
					int16_t numberDevices = 0;
					memcpy(&numberDevices, data+SHORT_MAC_LEN+1, 1);
					
					for(uint16_t i = 0; i < numberDevices; i++) {
						//we need to test if this value is for us:
						//we grab the mac address of each devices:
						byte shortAddress[2];
						memcpy(shortAddress, data+SHORT_MAC_LEN+2+i*4, 2);
						
						//we test if the short address is our address
						if(shortAddress[0] == _currentShortAddress[0] && shortAddress[1] == _currentShortAddress[1]) {
							//we grab the replytime wich is for us
							uint16_t replyTime;
							memcpy(&replyTime, data+SHORT_MAC_LEN+2+i*4+2, 2);
							//we configure our replyTime;
							_replyDelayTimeUS = replyTime;
							
							// on POLL we (re-)start, so no protocol failure
							_protocolFailed = false;
							
							DW1000.getReceiveTimestamp(myDistantDevice->timePollReceived);
							//we note activity for our device:
							myDistantDevice->noteActivity();
							//we indicate our next receive message for our ranging protocole
							_expectedMsgId = RANGE;
							transmitPollAck(myDistantDevice);
							noteActivity();
							s+="success";
							log(s);
							return;
						}
						
					}
					s+="fail";
					log(s);
					
					
				}
				else if(messageType == RANGE) {
					log("RANGE");
					//we receive a RANGE which is a broacast message
					//we need to grab info about it
					uint8_t numberDevices = 0;
					memcpy(&numberDevices, data+SHORT_MAC_LEN+1, 1);
					
					
					for(uint8_t i = 0; i < numberDevices; i++) {
						//we need to test if this value is for us:
						//we grab the mac address of each devices:
						byte shortAddress[2];
						memcpy(shortAddress, data+SHORT_MAC_LEN+2+i*27, 2);
						
						//we test if the short address is our address
						if(shortAddress[0] == _currentShortAddress[0] && shortAddress[1] == _currentShortAddress[1]) {
							//we grab the replytime wich is for us
							DW1000.getReceiveTimestamp(myDistantDevice->timeRangeReceived);
							noteActivity();
							_expectedMsgId = POLL;
							
							if(!_protocolFailed) {
								
								myDistantDevice->timePollSent.setTimestamp(data+SHORT_MAC_LEN+4+27*i);
								myDistantDevice->timePollAckReceived.setTimestamp(data+SHORT_MAC_LEN+9+27*i);
								myDistantDevice->timeRangeSent.setTimestamp(data+SHORT_MAC_LEN+14+27*i);
								memcpy(&_pos_x[myDistantDevice->getIndex()], data + 19 + 27 * i + SHORT_MAC_LEN, 4);
								memcpy(&_pos_y[myDistantDevice->getIndex()], data + 23 + 27 * i + SHORT_MAC_LEN, 4);
								// (re-)compute range as two-way ranging is done
								DW1000Time myTOF;
								computeRangeAsymmetric(myDistantDevice, &myTOF); // CHOSEN RANGING ALGORITHM
								
								float distance = myTOF.getAsMeters();
								
								if (_useRangeFilter) {
									//Skip first range
									if (myDistantDevice->getRange() != 0.0f) {
										distance = filterValue(distance, myDistantDevice->getRange(), _rangeFilterValue);
									}
								}
								
								myDistantDevice->setRXPower(DW1000.getReceivePower());
								myDistantDevice->setRange(distance);
								
								myDistantDevice->setFPPower(DW1000.getFirstPathPower());
								myDistantDevice->setQuality(DW1000.getReceiveQuality());
								
								//we send the range to TAG
								transmitRangeReport(myDistantDevice);
								
								//we have finished our range computation. We send the corresponding handler
								_lastDistantDevice = myDistantDevice->getIndex();
								if(_calibrate && _handleCalibration != 0){
									(*_handleCalibration)(_realDistance, (int)(distance*1000));
								} else if(_handleNewRange != 0) {
									(*_handleNewRange)();
								}
								if(_calibrate && (millis() -_startCalibrationTime > _calibrationTimeOut)){
									_calibrate = false;
								}

								//update fo the frameRate
								rangingSuccess();
								
							}
							else {
								transmitRangeFailed(myDistantDevice);
							}
							
							
							return;
						}
						
					}
					
					
				}
			}
			else if(_type == TAG) {
				// get message and parse
				if (messageType == CHANGE_CHANNEL_MODE)
				{
					log("CHANGE_CHANNEL_MODE");
					uint16_t channel;
					memcpy(&channel, data + SHORT_MAC_LEN + 1, 2);
					String s = "change channel to channel ";
					s += channel;
					log(s);
					if (_handleNewChannel != 0)
						(*_handleNewChannel)(channel);
					return;
				}
				else if (messageType != _expectedMsgId)
				{

					//not needed ?
					 if(!_isEnabled) {
						String s = "IGNORED Unexppected msg : ";
						s+=messageType;
						s+=" expected : ";
						s+=_expectedMsgId;
						log(s);
						return;
					} else {
						// unexpected message, start over again
						String s = "Unexppected msg : ";
						s += messageType;
						s += " expected : ";
						s += _expectedMsgId;
						log(s);
						return;
					}
				}
				if(messageType == TAG_SYNC ){
					log("TAG_SYNC");
					//we have just received a SYNCH message from the master tag
					if (myDistantDevice != NULL){
						memcpy(_masterTagShortAddress, myDistantDevice->getByteShortAddress(), 2);
						_isEnabled = true;
					}
					//transmitPoll(NULL);
					//_expectedMsgId = POLL_ACK;
					if (myDistantDevice != NULL)
						myDistantDevice->noteActivity();

					noteActivity();
				}
				if (messageType == TAG_SYNC_END)
				{
					log("TAG_SYNC_END");
					//TODO : get position data from tag
					_waitingSyncAck = false;
					_enabledTagNumber++;
					if (_enabledTagNumber > _tagDevicesNumber)
					{
						_enabledTagNumber = 0;
						_isEnabled = true;
					}
					if (myDistantDevice != NULL)
						myDistantDevice->noteActivity();

					noteActivity();
				}
				if (_isEnabled && messageType == POLL_ACK)
				{
					String s = "POLL_ACK from";
					uint16_t addr = 0;
					if(myDistantDevice != NULL)
						addr = myDistantDevice->getShortAddress();
					log(s+addr);

					DW1000.getReceiveTimestamp(myDistantDevice->timePollAckReceived);
					//we note activity for our device:
					myDistantDevice->noteActivity();
					
					//in the case the message come from our last device:
					if(myDistantDevice->getIndex() == _networkDevicesNumber-1) {
						_expectedMsgId = RANGE_REPORT;
						//and transmit the next message (range) of the ranging protocole (in broadcast)
						transmitRange(NULL);
					}
				}
				else if (_isEnabled && messageType == RANGE_REPORT)
				{

					float curRange;
					memcpy(&curRange, data+1+SHORT_MAC_LEN, 4);
					float curRXPower;
					memcpy(&curRXPower, data+5+SHORT_MAC_LEN, 4);

					// we receive dataSync
					memcpy(&_dataSyncSize, data + 9 + SHORT_MAC_LEN, 1);
					if (_dataSyncSize > 0 && _dataSync != 0)
						memcpy(_dataSync, data + 10 + SHORT_MAC_LEN, _dataSyncSize);
					if (_handleDataSync != 0)
					{
						(*_handleDataSync)();
					}

					if (_useRangeFilter) {
						//Skip first range
						if (myDistantDevice->getRange() != 0.0f) {
							curRange = filterValue(curRange, myDistantDevice->getRange(), _rangeFilterValue);
						}
					}

					//we have a new range to save !
					myDistantDevice->setRange(curRange);
					myDistantDevice->setRXPower(curRXPower);
					
					
					//We can call our handler !
					//we have finished our range computation. We send the corresponding handler
					_lastDistantDevice = myDistantDevice->getIndex();
					if(_lastDistantDevice== _networkDevicesNumber-1){
						_isEnabled = false;
						if(_isMasterTag){
							_enabledTagNumber++;
							if (_enabledTagNumber > _tagDevicesNumber){
								_enabledTagNumber = 0;
								_isEnabled = true;
							}
						} else {
							transmitTagSyncEnd();
						}		
					}
					if(_handleNewRange != 0) {
						(*_handleNewRange)();
					}
					rangingSuccess();
				}
				else if(messageType == RANGE_FAILED) {
					//not needed as we have a timer;
					return;
					_expectedMsgId = POLL_ACK;
				}
			}
		}
		
	}
}

void DW1000RangingClass::useRangeFilter(boolean enabled) {
	_useRangeFilter = enabled;
}

void DW1000RangingClass::setRangeFilterValue(uint16_t newValue) {
	if (newValue < 2) {
		_rangeFilterValue = 2;
	}else{
		_rangeFilterValue = newValue;
	}
}


/* ###########################################################################
 * #### Private methods and Handlers for transmit & Receive reply ############
 * ######################################################################### */


void DW1000RangingClass::handleSent() {
	// status change on sent success
	_sentAck = true;
}

void DW1000RangingClass::handleReceived() {
	// status change on received success
	_receivedAck = true;
}


void DW1000RangingClass::noteActivity() {
	// update activity timestamp, so that we do not reach "resetPeriod"
	_lastActivity = millis();
}

void DW1000RangingClass::resetInactive() {
	//if inactive
	if(_type == ANCHOR) {
		log("RESET INACTIVE");
		_expectedMsgId = POLL;
		receiver();
	}
	noteActivity();
}

void DW1000RangingClass::timerTick() {
	//Serial.println("timerTick");
	if(_networkDevicesNumber > 0 && counterForBlink != 0) {	
		if(_type == TAG) {
			
			//send a brodcast poll
			if(_isEnabled){
				if(_expectedMsgId == POLL_ACK){ // timeout : no response to poll msg
					log("TIMEOUT : no respond to poll msg");
					if(_isMasterTag){
						_isEnabled = false;
						_enabledTagNumber++;
						if (_enabledTagNumber > _tagDevicesNumber)
						{
							_enabledTagNumber = 0;
							_isEnabled = true;
							transmitPoll(NULL);
							_expectedMsgId = POLL_ACK;
						} else {
							transmitTagSync(&_tagDevices[_enabledTagNumber - 1]);
							_waitingSyncAck = true;
							counterForSync = 0;
							_expectedMsgId = TAG_SYNC_END;
						}
					}else {	// Slave tag
						_isEnabled = false;
						transmitTagSyncEnd();
						_expectedMsgId = TAG_SYNC;
					}
				}else {
					transmitPoll(NULL);
					_expectedMsgId = POLL_ACK;
				}	
			} else if (_isMasterTag) {
				if(!_waitingSyncAck){
					transmitTagSync(&_tagDevices[_enabledTagNumber - 1]);
					_waitingSyncAck = true;
					counterForSync = 0;
					_expectedMsgId = TAG_SYNC_END;
				}	
			} else {
				_expectedMsgId = TAG_SYNC;
			}
		}
	}
	else if (_tagDevicesNumber > 0 && counterForBlink != 0)
	{
		if(_type == TAG) {
			if(_isMasterTag){
				if (!_waitingSyncAck)
				{
					if(_isEnabled){
						_isEnabled = false;
						_enabledTagNumber++;
						if (_enabledTagNumber > _tagDevicesNumber)
						{
							_enabledTagNumber = 0;
							_isEnabled = true;
						}
					}else{
						transmitTagSync(&_tagDevices[_enabledTagNumber - 1]);
						_waitingSyncAck = true;
						counterForSync = 0;
						_expectedMsgId = TAG_SYNC_END;
					}
				}
			}
			else	// Slave tag
			{
				if(_isEnabled){
					_isEnabled = false;
					transmitTagSyncEnd();
					_expectedMsgId = TAG_SYNC;
				} else {
					_expectedMsgId = TAG_SYNC;
				}
			}
		}
	}
	else if(counterForBlink == 0) {
		counterForinactivity++;
		if(_type == TAG) {
			transmitBlink();
		}
		
		//check for inactive devices if we are a TAG or ANCHOR
		if(counterForinactivity >= 2){
			checkForInactiveDevices();
			counterForinactivity = 0;
		}
	}
	if(_waitingSyncAck){
		counterForSync++;
	}
	if(counterForSync>4){
		_waitingSyncAck = false;
		counterForSync = 0;
		_isEnabled = false;
		_enabledTagNumber++;
		if (_enabledTagNumber > _tagDevicesNumber)
		{
			_enabledTagNumber = 0;
			_isEnabled = true;
		}
	}

	counterForBlink++;
	if(counterForBlink > 20) {
		counterForBlink = 0;
	}
}


void DW1000RangingClass::copyShortAddress(byte address1[], byte address2[]) {
	*address1     = *address2;
	*(address1+1) = *(address2+1);
}

/* ###########################################################################
 * #### Methods for ranging protocole   ######################################
 * ######################################################################### */

void DW1000RangingClass::transmitInit() {
	DW1000.newTransmit();
	DW1000.setDefaults();
}


void DW1000RangingClass::transmit(byte datas[]) {
	DW1000.setData(datas, LEN_DATA);
	DW1000.startTransmit();
}


void DW1000RangingClass::transmit(byte datas[], DW1000Time time) {
	DW1000.setDelay(time);
	DW1000.setData(data, LEN_DATA);
	DW1000.startTransmit();
}

void DW1000RangingClass::transmitBlink() {
	log("transmitBlink");
	transmitInit();
	_globalMac.generateBlinkFrame(data, _currentAddress, _currentShortAddress);
	transmit(data);
}

void DW1000RangingClass::transmitRangingInit(DW1000Device* myDistantDevice) {
	log("transmitRangingInit");
	transmitInit();
	//we generate the mac frame for a ranging init message
	_globalMac.generateLongMACFrame(data, _currentShortAddress, myDistantDevice->getByteAddress());
	//we define the function code
	data[LONG_MAC_LEN] = RANGING_INIT;
	
	copyShortAddress(_lastSentToShortAddress, myDistantDevice->getByteShortAddress());
	
	transmit(data);
}

void DW1000RangingClass::transmitPoll(DW1000Device* myDistantDevice) {
	log("transmitPoll");
	transmitInit();
	
	if(myDistantDevice == NULL) {
		//we need to set our timerDelay:
		_timerDelay = DEFAULT_TIMER_DELAY+(uint16_t)(_networkDevicesNumber*3*DEFAULT_REPLY_DELAY_TIME/1000);
		
		byte shortBroadcast[2] = {0xFF, 0xFF};
		_globalMac.generateShortMACFrame(data, _currentShortAddress, shortBroadcast);
		data[SHORT_MAC_LEN]   = POLL;
		//we enter the number of devices
		data[SHORT_MAC_LEN+1] = _networkDevicesNumber;
		
		for(uint8_t i = 0; i < _networkDevicesNumber; i++) {
			//each devices have a different reply delay time.
			_networkDevices[i].setReplyTime((2*i+1)*DEFAULT_REPLY_DELAY_TIME);
			//we write the short address of our device:
			memcpy(data+SHORT_MAC_LEN+2+4*i, _networkDevices[i].getByteShortAddress(), 2);
			
			//we add the replyTime
			uint16_t replyTime = _networkDevices[i].getReplyTime();
			memcpy(data+SHORT_MAC_LEN+2+2+4*i, &replyTime, 2);
			
		}
		
		copyShortAddress(_lastSentToShortAddress, shortBroadcast);
		
	}
	else {
		//we redefine our default_timer_delay for just 1 device;
		_timerDelay = DEFAULT_TIMER_DELAY;
		
		_globalMac.generateShortMACFrame(data, _currentShortAddress, myDistantDevice->getByteShortAddress());
		
		data[SHORT_MAC_LEN]   = POLL;
		data[SHORT_MAC_LEN+1] = 1;
		uint16_t replyTime = myDistantDevice->getReplyTime();
		memcpy(data+SHORT_MAC_LEN+2, &replyTime, sizeof(uint16_t)); // todo is code correct?
		
		copyShortAddress(_lastSentToShortAddress, myDistantDevice->getByteShortAddress());
	}
	
	transmit(data);
}


void DW1000RangingClass::transmitPollAck(DW1000Device* myDistantDevice) {
	log("transmitPollAck");
	transmitInit();
	_globalMac.generateShortMACFrame(data, _currentShortAddress, myDistantDevice->getByteShortAddress());
	data[SHORT_MAC_LEN] = POLL_ACK;
	// delay the same amount as ranging tag
	DW1000Time deltaTime = DW1000Time(_replyDelayTimeUS, DW1000Time::MICROSECONDS);
	copyShortAddress(_lastSentToShortAddress, myDistantDevice->getByteShortAddress());
	transmit(data, deltaTime);
}

void DW1000RangingClass::transmitRange(DW1000Device* myDistantDevice) {
	log("transmitRange");
	//transmit range need to accept broadcast for multiple anchor
	transmitInit();
	
	
	if(myDistantDevice == NULL) {
		//we need to set our timerDelay:
		_timerDelay = DEFAULT_TIMER_DELAY+(uint16_t)(_networkDevicesNumber*3*DEFAULT_REPLY_DELAY_TIME/1000);
		
		byte shortBroadcast[2] = {0xFF, 0xFF};
		_globalMac.generateShortMACFrame(data, _currentShortAddress, shortBroadcast);
		data[SHORT_MAC_LEN]   = RANGE;
		//we enter the number of devices
		data[SHORT_MAC_LEN+1] = _networkDevicesNumber;
		
		// delay sending the message and remember expected future sent timestamp
		DW1000Time deltaTime     = DW1000Time(DEFAULT_REPLY_DELAY_TIME, DW1000Time::MICROSECONDS);
		DW1000Time timeRangeSent = DW1000.setDelay(deltaTime);
		
		for(uint8_t i = 0; i < _networkDevicesNumber; i++) {
			//we write the short address of our device:
			memcpy(data+SHORT_MAC_LEN+2+27*i, _networkDevices[i].getByteShortAddress(), 2);
			
			
			//we get the device which correspond to the message which was sent (need to be filtered by MAC address)
			_networkDevices[i].timeRangeSent = timeRangeSent;
			_networkDevices[i].timePollSent.getTimestamp(data+SHORT_MAC_LEN+4+27*i);
			_networkDevices[i].timePollAckReceived.getTimestamp(data+SHORT_MAC_LEN+9+27*i);
			_networkDevices[i].timeRangeSent.getTimestamp(data+SHORT_MAC_LEN+14+27*i);
			memcpy(data + 19 + 27*i + SHORT_MAC_LEN, &_pos_x[0], 4);
			memcpy(data + 23 + 27*i + SHORT_MAC_LEN, &_pos_y[0], 4);
		}
		
		copyShortAddress(_lastSentToShortAddress, shortBroadcast);
		
	}
	else {
		_globalMac.generateShortMACFrame(data, _currentShortAddress, myDistantDevice->getByteShortAddress());
		data[SHORT_MAC_LEN] = RANGE;
		// delay sending the message and remember expected future sent timestamp
		DW1000Time deltaTime = DW1000Time(_replyDelayTimeUS, DW1000Time::MICROSECONDS);
		//we get the device which correspond to the message which was sent (need to be filtered by MAC address)
		myDistantDevice->timeRangeSent = DW1000.setDelay(deltaTime);
		myDistantDevice->timePollSent.getTimestamp(data+1+SHORT_MAC_LEN);
		myDistantDevice->timePollAckReceived.getTimestamp(data+6+SHORT_MAC_LEN);
		myDistantDevice->timeRangeSent.getTimestamp(data+11+SHORT_MAC_LEN);
		memcpy(data + 16 + SHORT_MAC_LEN, &_pos_x[0], 4);
		memcpy(data + 20 + SHORT_MAC_LEN, &_pos_y[0], 4);
		copyShortAddress(_lastSentToShortAddress, myDistantDevice->getByteShortAddress());
	}
	
	
	transmit(data);
}


void DW1000RangingClass::transmitRangeReport(DW1000Device* myDistantDevice) {
	log("transmitRangeReport");
	transmitInit();
	_globalMac.generateShortMACFrame(data, _currentShortAddress, myDistantDevice->getByteShortAddress());
	data[SHORT_MAC_LEN] = RANGE_REPORT;
	// write final ranging result
	float curRange   = myDistantDevice->getRange();
	float curRXPower = myDistantDevice->getRXPower();
	//We add the Range and then the RXPower
	memcpy(data+1+SHORT_MAC_LEN, &curRange, 4);
	memcpy(data+5+SHORT_MAC_LEN, &curRXPower, 4);
	// we send dataSync
	memcpy(data+9+SHORT_MAC_LEN, &_dataSyncSize, 1);
	if(_dataSyncSize>0 && _dataSync != 0)
		memcpy(data + 10 + SHORT_MAC_LEN, _dataSync, _dataSyncSize);

	copyShortAddress(_lastSentToShortAddress, myDistantDevice->getByteShortAddress());
	transmit(data, DW1000Time(_replyDelayTimeUS, DW1000Time::MICROSECONDS));
}

void DW1000RangingClass::transmitRangeFailed(DW1000Device* myDistantDevice) {
	log("transmitRangeFailed");
	transmitInit();
	_globalMac.generateShortMACFrame(data, _currentShortAddress, myDistantDevice->getByteShortAddress());
	data[SHORT_MAC_LEN] = RANGE_FAILED;
	
	copyShortAddress(_lastSentToShortAddress, myDistantDevice->getByteShortAddress());
	transmit(data);
}

void DW1000RangingClass::receiver() {
	DW1000.newReceive();
	DW1000.setDefaults();
	// so we don't need to restart the receiver manually
	DW1000.receivePermanently(true);
	DW1000.startReceive();
}

void DW1000RangingClass::transmitTagSync(DW1000Device *myDistantDevice)
{
	log("transmitTagSync");
	transmitInit();
	_globalMac.generateShortMACFrame(data, _currentShortAddress, myDistantDevice->getByteShortAddress());
	data[SHORT_MAC_LEN] = TAG_SYNC;
	copyShortAddress(_lastSentToShortAddress, myDistantDevice->getByteShortAddress());
	transmit(data);
}

void DW1000RangingClass::transmitTagSyncEnd()
{
	DW1000Device *masterTagDevice = searchDistantDevice(_masterTagShortAddress);
	if (masterTagDevice != NULL){
		log("transmitTagSyncEnd");
		transmitInit();
		_globalMac.generateShortMACFrame(data, _currentShortAddress, masterTagDevice->getByteShortAddress());
		data[SHORT_MAC_LEN] = TAG_SYNC_END;
		copyShortAddress(_lastSentToShortAddress, masterTagDevice->getByteShortAddress());
		transmit(data);
	} else {
		log("transmitTagSyncEnd failed");
	}
	
}

/* ###########################################################################
 * #### Methods for range computation and corrections  #######################
 * ######################################################################### */


void DW1000RangingClass::computeRangeAsymmetric(DW1000Device* myDistantDevice, DW1000Time* myTOF) {
	// asymmetric two-way ranging (more computation intense, less error prone)
	DW1000Time round1 = (myDistantDevice->timePollAckReceived-myDistantDevice->timePollSent).wrap();
	DW1000Time reply1 = (myDistantDevice->timePollAckSent-myDistantDevice->timePollReceived).wrap();
	DW1000Time round2 = (myDistantDevice->timeRangeReceived-myDistantDevice->timePollAckSent).wrap();
	DW1000Time reply2 = (myDistantDevice->timeRangeSent-myDistantDevice->timePollAckReceived).wrap();
	
	myTOF->setTimestamp((round1*round2-reply1*reply2)/(round1+round2+reply1+reply2));
	/*
	//Serial.print("timePollAckReceived ");myDistantDevice->timePollAckReceived.print();
	//Serial.print("timePollSent ");myDistantDevice->timePollSent.print();
	//Serial.print("round1 "); //Serial.println((long)round1.getTimestamp());
	
	//Serial.print("timePollAckSent ");myDistantDevice->timePollAckSent.print();
	//Serial.print("timePollReceived ");myDistantDevice->timePollReceived.print();
	//Serial.print("reply1 "); //Serial.println((long)reply1.getTimestamp());
	
	//Serial.print("timeRangeReceived ");myDistantDevice->timeRangeReceived.print();
	//Serial.print("timePollAckSent ");myDistantDevice->timePollAckSent.print();
	//Serial.print("round2 "); //Serial.println((long)round2.getTimestamp());
	
	//Serial.print("timeRangeSent ");myDistantDevice->timeRangeSent.print();
	//Serial.print("timePollAckReceived ");myDistantDevice->timePollAckReceived.print();
	//Serial.print("reply2 "); //Serial.println((long)reply2.getTimestamp());
	 */
}


/* FOR DEBUGGING*/
void DW1000RangingClass::visualizeDatas(byte datas[]) {
	char string[60];
	sprintf(string, "%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X",
					datas[0], datas[1], datas[2], datas[3], datas[4], datas[5], datas[6], datas[7], datas[8], datas[9], datas[10], datas[11], datas[12], datas[13], datas[14], datas[15]);
	//Serial.println(string);
}



/* ###########################################################################
 * #### Utils  ###############################################################
 * ######################################################################### */

float DW1000RangingClass::filterValue(float value, float previousValue, uint16_t numberOfElements) {
	
	float k = 2.0f / ((float)numberOfElements + 1.0f);
	return (value * k) + previousValue * (1.0f - k);
}

void DW1000RangingClass::rangingSuccess(){
	updateRangingCounter();
	_successRangingCount++;
}

void DW1000RangingClass::updateRangingCounter()
{
	if (millis() - _frameRateStartTime > _rangingCountPeriod)
	{
		_frameRateStartTime = millis();
		_rangingFrameRate = (uint16_t)((float)_successRangingCount / ((float)_rangingCountPeriod/1000.0f));
		_successRangingCount = 0;
	}
}

/* ###########################################################################
 * #### Auto calibration  ####################################################
 * ######################################################################### */

void DW1000RangingClass::startAutoCalibration(int realDistance, unsigned long timeOut){
	_realDistance = realDistance;
	_calibrate = true;
	_startCalibrationTime = millis();
	_calibrationTimeOut = timeOut;
}

void DW1000RangingClass::stopCalibration(){
	_calibrate = false;
}

/* ###########################################################################
 * #### trilateration  ######################################################
 * ######################################################################### */

float DW1000RangingClass::getPosX(uint16_t shortAddress)
{
	byte byteShortAddress[2];
	memcpy(byteShortAddress,&shortAddress,2);
	DW1000Device *tag = searchDistantDevice(byteShortAddress);
	if(tag == NULL)
		return -2000;
	return _pos_x[tag->getIndex()];
}

float DW1000RangingClass::getPosX(){
	return _pos_x[0];
}

float DW1000RangingClass::getPosY()
{
	return _pos_y[0];
}

float DW1000RangingClass::getPosY(uint16_t shortAddress)
{
	byte byteShortAddress[2];
	memcpy(byteShortAddress, &shortAddress, 2);
	DW1000Device *tag = searchDistantDevice(byteShortAddress);
	if (tag == NULL)
		return -2000;
	return _pos_y[tag->getIndex()];
}

void DW1000RangingClass::setPosX(float &x, uint8_t index)
{
	_pos_x[index] = x;
}

void DW1000RangingClass::setPosY(float &y, uint8_t index)
{
	_pos_y[index] = y;
}

// dataSync
void DW1000RangingClass::setDataSyncSize(uint8_t dataSize)
{
	_dataSyncSize = dataSize;
	
}

void DW1000RangingClass::setDataSync(void *data)
{
	_dataSync = data;
}

void DW1000RangingClass::transmitChangeChannel(uint16_t channel)
{
	log("transmitChangeChannel");
	transmitInit();
	byte shortBroadcast[2] = {0xFF, 0xFF};
	_globalMac.generateShortMACFrame(data, _currentShortAddress, shortBroadcast);
	data[SHORT_MAC_LEN] = CHANGE_CHANNEL_MODE;
	memcpy(data + SHORT_MAC_LEN + 1,&channel,2);
	copyShortAddress(_lastSentToShortAddress, shortBroadcast);
	transmit(data);
}