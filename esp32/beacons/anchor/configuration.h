#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#define CURRENT_BEACON_NUMBER 0 // Numéro courant de la balise à uploader. compris entre 0 et MAX_ANCHORS

#define MAX_ANCHORS 4   // nombre maximum de balises fixes
#define MAX_TAG 2 // nombre maximum de trackers

const byte TAG_SHORT_ADDRESS[MAX_TAG] = {50, 51};
const byte MASTER_TAG_ADDRESS = 50;

const byte ANCHOR_SHORT_ADDRESS[MAX_ANCHORS] = {35, 36, 37, 38};
const uint16_t ANTENNA_DELAY[MAX_ANCHORS] = {16495, 16542, 16556, 16531};
const float Z_ANCHOR = 458.3;
const float Z_CENTRAL = 1036.3;
const float Z_TAG = 503.3;
const float Z_HEIGHT[MAX_ANCHORS] = {Z_ANCHOR, Z_ANCHOR, Z_ANCHOR, Z_CENTRAL};

/**
 * BLE Server
 */
#define BEACON_BLE_ADDRESS (ANCHOR_SHORT_ADDRESS[3])
#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

/**
 * EEPROM addresses
 */

#define EEPROM_BASE_ADDRESS 49

#define EEPROM_NUM_ANCHOR       EEPROM_BASE_ADDRESS         // 1 octet
#define EEPROM_ANTENNA_DELAY    (EEPROM_NUM_ANCHOR+1)       // 2 octets
//#define EEPROM_REPLY_DELAY    (EEPROM_ANTENNA_DELAY+2)      // 2 octet
//#define EEPROM_LONG_ADDRESS     (EEPROM_SHORT_ADDRESS+2)    // 16 octets


#endif