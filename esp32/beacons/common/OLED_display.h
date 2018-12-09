#ifndef OLED_DISPLAY_H
#define OLED_DISPLAY_H

#include "Arduino.h"
#include "SSD1306.h"
#include <Wire.h>

#define NB_MSG_MAX 5
#define DEFAULT_DURATION 10000
#define DEFAULT_PERSISTENCE 2500

class OLEDdisplay;

class Text {
public:
  Text(const String text = "",const int id = -1, const int16_t x = 0, const int16_t y = 0) :_id(id), _x(x), _y(y), _value(text) {}
  void clear();

private:
    int16_t _x;
    int16_t _y;
    String _value;
    int _id;

    friend class OLEDdisplay;
};

class OLEDdisplay : public SSD1306{
public:
  OLEDdisplay(uint8_t address, uint8_t pin_sda, uint8_t pin_scl) : SSD1306(address, pin_sda, pin_scl),
                                                                   _msgPersistence(DEFAULT_PERSISTENCE),
                                                                   _index(0)
    { 
        _displayStartTime = millis();
        _updateNeeded = true;
    }

  void drawString(int16_t x, int16_t y, String text);

  void update();
  void log(String text);
  void displayMsg(Text msg);
  void clearMsg(int id);
  void clearAll();

private:
  unsigned long _displayStartTime;
  unsigned long _msgPersistence;

  Text _log;
  Text _msg[NB_MSG_MAX];
  int _msgNumber; // number of msg
  int _index;
  boolean _updateNeeded;

};

#endif