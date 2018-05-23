//
// Created by Arthur Rönisch on 03/04/2018.
//

#ifndef BATTERYMANAGEMENTSYSTEM_SMBUS_H
#define BATTERYMANAGEMENTSYSTEM_SMBUS_H

#define SDA_PORT PORTC
#define SDA_PIN 4

#define SCL_PORT PORTC
#define SCL_PIN 5

#define I2C_SLOWMODE 1

#include <SoftI2CMaster.h>

uint8_t sI2CDeviceAddress;


void TogglePin(uint8_t aPinNr);
void BlinkLedForever(int LED_PIN, int aBinkDelay);
int scanForAttachedI2CDevice(void);
bool checkForAttachedI2CDevice(uint8_t aStandardDeviceAddress);
template<typename TEMP> bool writeDataFlash(uint16_t address, TEMP data);
uint32_t readU32FromManufacturerAccess(uint16_t aCommand);
uint8_t readBlockDataFlash(uint8_t aCommand, uint8_t* aDataBufferPtr, uint8_t aDataBufferLength);
void writeWord(uint8_t addr, uint16_t aCommand);
uint8_t readBlockFromManufacturerBlockAccess(uint16_t aCommand, uint8_t* aDataBufferPtr, uint8_t aDataBufferLength);
uint8_t readBlock(uint8_t aCommand, uint8_t* aDataBufferPtr, uint8_t aDataBufferLength);
uint16_t readWordFromManufacturerAccess(uint16_t aCommand);
uint16_t readWord(uint8_t aFunction);
bool initSMBus(uint8_t addr, int ledPIN);
void writeWordToManufacturerAccess(uint16_t word);

bool initSMBus(uint8_t addr, int ledPIN){
    bool tI2CSucessfullyInitialized = i2c_init();
    if (tI2CSucessfullyInitialized) {
        Serial.println(F("I2C initalized sucessfully"));
    }
    else {
        Serial.println(F("I2C pullups missing"));
        BlinkLedForever(ledPIN, 100);
    }
    Serial.flush();

    /*
    * Check for I2C device and blink until device attached
    */
    if (!checkForAttachedI2CDevice(addr)) {
        int tDeviceAttached;
        do {
            tDeviceAttached = scanForAttachedI2CDevice();
            delay(500);
            TogglePin(ledPIN);
        } while (tDeviceAttached < 0);
    }
    digitalWrite(ledPIN,1);
    sI2CDeviceAddress = addr;
}

uint16_t readWord(uint8_t aFunction) {
    i2c_stop();
    i2c_start((sI2CDeviceAddress << 1) | I2C_WRITE);
    i2c_write(aFunction);
    i2c_rep_start((sI2CDeviceAddress << 1) | I2C_READ);
    uint8_t tLSB = i2c_read(false);
    uint8_t tMSB = i2c_read(true);
    i2c_stop();
    return (uint16_t) tLSB | (((uint16_t) tMSB) << 8);
}

uint16_t readWordFromManufacturerAccess(uint16_t aCommand) {
    i2c_stop();
    i2c_start((sI2CDeviceAddress << 1) | I2C_WRITE);
    i2c_write(MANUFACTURER_ACCESS);
    // Write manufacturer command word
    i2c_rep_start((sI2CDeviceAddress << 1) | I2C_WRITE);
    i2c_write(aCommand);
    i2c_write(aCommand >> 8);
    i2c_stop();
    // Read manufacturer result word
    return readWord(MANUFACTURER_ACCESS);
}

uint8_t readBlock(uint8_t aCommand, uint8_t* aDataBufferPtr, uint8_t aDataBufferLength) {
    i2c_stop();
    i2c_start((sI2CDeviceAddress << 1) + I2C_WRITE);
    i2c_write(aCommand);
    i2c_rep_start((sI2CDeviceAddress << 1) + I2C_READ);

    // First read length of data
    uint8_t tLengthOfData = i2c_read(false);
    if (tLengthOfData > aDataBufferLength) {
        tLengthOfData = aDataBufferLength;
    }

    // then read data
    uint8_t tIndex;
    for (tIndex = 0; tIndex < tLengthOfData - 1; tIndex++) {
        aDataBufferPtr[tIndex] = i2c_read(false);
    }
    // Read last byte with "true"
    aDataBufferPtr[tIndex++] = i2c_read(true);

    i2c_stop();
    return tLengthOfData;
}

uint8_t readBlockFromManufacturerBlockAccess(uint16_t aCommand, uint8_t* aDataBufferPtr, uint8_t aDataBufferLength) {
    writeWord(MANUFACTURER_BLOCK_ACCESS, aCommand);
    return readBlock(MANUFACTURER_BLOCK_ACCESS, aDataBufferPtr, aDataBufferLength);
}

void writeWord(uint8_t addr, uint16_t aCommand) {
    i2c_stop();
    i2c_start((sI2CDeviceAddress << 1) | I2C_WRITE);
    i2c_write(addr);
    // Write manufacturer command word
    i2c_write(aCommand);
    i2c_write(aCommand >> 8);
    i2c_stop();
}

void writeWordToManufacturerAccess(uint16_t word){
    writeWord(MANUFACTURER_ACCESS, word);
}

/*void writeBlock(uint8_t, uint8_t* aDataBufferPtr, uint8_t aDataBufferLength){

}*/

uint8_t readBlockDataFlash(uint8_t aCommand, uint8_t* aDataBufferPtr, uint8_t aDataBufferLength) {
    i2c_start((sI2CDeviceAddress << 1) + I2C_WRITE);
    i2c_write(aCommand & 0xFF);
    i2c_write(aCommand >> 8);
    i2c_rep_start((sI2CDeviceAddress << 1) + I2C_READ);

    // First read length of data
    uint8_t tLengthOfData = i2c_read(false);
    if (tLengthOfData > aDataBufferLength) {
        tLengthOfData = aDataBufferLength;
    }

    // then read data
    uint8_t tIndex;
    for (tIndex = 0; tIndex < tLengthOfData - 1; tIndex++) {
        aDataBufferPtr[tIndex] = i2c_read(false);
    }
    // Read last byte with "true"
    aDataBufferPtr[tIndex++] = i2c_read(true);

    i2c_stop();
    return tLengthOfData;
}

uint32_t readU32FromManufacturerAccess(uint16_t aCommand) {
    i2c_start((sI2CDeviceAddress << 1) | I2C_WRITE);
    i2c_write(MANUFACTURER_ACCESS);
    // Write manufacturer command word
    i2c_rep_start((sI2CDeviceAddress << 1) | I2C_WRITE);
    i2c_write(aCommand);
    i2c_write(aCommand >> 8);
    i2c_stop();
    // Read manufacturer result word
    i2c_start((sI2CDeviceAddress << 1) | I2C_WRITE);
    i2c_write(MANUFACTURER_ACCESS);
    i2c_rep_start((sI2CDeviceAddress << 1) | I2C_READ);
    uint8_t u1LSB = i2c_read(false);
    uint8_t u1MSB = i2c_read(false);
    uint8_t u2LSB = i2c_read(false);
    uint8_t u2MSB = i2c_read(true);
    i2c_stop();
    return ((uint32_t) u1LSB << 16) | ((uint32_t) u1MSB << 24) | (uint32_t) u2LSB | ((uint32_t) u2MSB << 8);
}

/*template<typename TEMP> bool writeDataFlash(uint16_t address, TEMP data){
    bool ack = false;

    i2c_stop();
    i2c_start((sI2CDeviceAddress << 1) | I2C_WRITE); //start communication with slave address and write command
    i2c_write(MANUFACTURER_BLOCK_ACCESS);			 //send command

    if (typeid(TEMP) == typeid(uint8_t)){
        i2c_write((uint8_t)3); //send byte count

        i2c_write((uint8_t)address & 0xFF);

        i2c_write((uint8_t)address >> 8);

        ack = i2c_write((uint8_t)data);

    } else if (typeid(TEMP) == typeid(uint16_t)){
        i2c_write((uint8_t)6); //send byte count

        i2c_write((uint8_t)address & 0xFF);

        i2c_write((uint8_t)address >> 8);

        i2c_write((uint8_t)data & 0xFF);
        ack = i2c_write((uint8_t)data >> 8);

    } else if (typeid(TEMP) == typeid(uint32_t)){
        i2c_write((uint8_t)6); //send byte count

        i2c_write(address & 0xFF);

        i2c_write(address >> 8);

        i2c_write((uint8_t)data & 0xFF);
        i2c_write((uint8_t)data >> 8);
        i2c_write((uint8_t)data >> 16);
        ack = i2c_write((uint8_t)data >> 24);

    } else if (typeid(TEMP) == typeid(int8_t)){
        i2c_write((uint8_t)3); //send byte count

        i2c_write((uint8_t)address & 0xFF);

        i2c_write((uint8_t)address >> 8);

        ack = i2c_write((uint8_t)data);

    } else if (typeid(TEMP) == typeid(int16_t)){
        i2c_write((uint8_t)6); //send byte count

        i2c_write((uint8_t)address & 0xFF);

        i2c_write((uint8_t)address >> 8);

        i2c_write((uint8_t)data & 0xFF);
        ack = i2c_write((uint8_t)data >> 8);
    } else if (typeid(TEMP) == typeid(int32_t)){
        i2c_write((uint8_t)6); //send byte count

        i2c_write((uint8_t)address & 0xFF);

        i2c_write((uint8_t)address >> 8);

        i2c_write((uint8_t)data & 0xFF);
        i2c_write((uint8_t)data >> 8);
        i2c_write((uint8_t)data >> 16);
        ack = i2c_write((uint8_t)data >> 24);
    }

    i2c_stop();
    return ack;
}*/

bool checkForAttachedI2CDevice(uint8_t aStandardDeviceAddress) {
    bool tOK = i2c_start(aStandardDeviceAddress << 1 | I2C_WRITE);
    i2c_stop();
    if (tOK) {
        Serial.print(F("Found attached I2C device at 0x"));
        Serial.println(aStandardDeviceAddress, HEX);
        sI2CDeviceAddress = SBM_DEVICE_ADDRESS;
        return true;
    }
    else {
        return false;
    }
}

int scanForAttachedI2CDevice(void) {
    int tFoundAdress = -1;
    for (uint8_t i = 0; i < 127; i++) {
        bool ack = i2c_start(i << 1 | I2C_WRITE);
        if (ack) {
            Serial.print(F("Found I2C device attached at address: 0x"));
            Serial.println(i, HEX);
            tFoundAdress = i;
        }
        i2c_stop();
    }
    if (tFoundAdress < 0) {
        Serial.println(F("Found no attached I2C device"));
    }
    else {
        sI2CDeviceAddress = tFoundAdress;
    }
    return tFoundAdress;
}

void BlinkLedForever(int LED_PIN, int aBinkDelay) {
    do {
        digitalWrite(LED_PIN, HIGH);
        delay(aBinkDelay);
        digitalWrite(LED_PIN, LOW);
        delay(aBinkDelay);
    } while (true);
}

void TogglePin(uint8_t aPinNr) {
    if (digitalRead(aPinNr) == HIGH) {
        digitalWrite(aPinNr, LOW);
    }
    else {
        digitalWrite(aPinNr, HIGH);
    }
}

#endif //BATTERYMANAGEMENTSYSTEM_SMBUS_H
