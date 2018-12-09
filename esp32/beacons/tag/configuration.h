#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#define CURRENT_BEACON_NUMBER 0 // Numéro courant de la balise à uploader. compris entre 0 et MAX_ANCHORS

#define MAX_TAG 2   // nombre maximum de trackers

const byte TAG_SHORT_ADDRESS[MAX_TAG] = {50, 51};
const byte MASTER_TAG_ADDRESS = 50;
//const uint16_t ANTENNA_DELAY[MAX_ANCHORS] = {16547, 16545, 16537, 16556};

const float Z_ANCHOR = 438.3;
const float Z_CENTRAL = 1016.3;
const float Z_TAG = 483.3;

const float X_MIN = 1;
const float X_MAX = 1999;
const float Y_MIN = 1;
const float Y_MAX = 2999;

/**
 * EEPROM addresses
 */

#define EEPROM_BASE_ADDRESS 49

#define EEPROM_NUM_TAG          EEPROM_BASE_ADDRESS         // 1 octet
//#define EEPROM_ANTENNA_DELAY    (EEPROM_NUM_ANCHOR+1)       // 2 octets
//#define EEPROM_REPLY_DELAY    (EEPROM_ANTENNA_DELAY+2)      // 2 octet
//#define EEPROM_LONG_ADDRESS     (EEPROM_SHORT_ADDRESS+2)    // 16 octets


#endif