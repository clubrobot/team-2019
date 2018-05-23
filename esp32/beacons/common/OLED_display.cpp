#include "OLED_display.h"

void Text::clear(){
    _id = -1;
    _value = "";
}

void OLEDdisplay::drawString(int16_t x, int16_t y, String text){
    displayMsg(Text(text,-1,x,y));;
}

void OLEDdisplay::update()
{
    if(millis() - _displayStartTime >= _msgPersistence)
        _updateNeeded = true;

    if(_updateNeeded){
       _updateNeeded = false;
       clear();

       if (_msgNumber > 0)
       {
           if (millis() - _displayStartTime >= _msgPersistence)
           {
               _displayStartTime = millis();
               if (_index >= _msgNumber - 1)
                   _index = 0;
               else
                   _index++;
           }
           setFont(ArialMT_Plain_24);
           SSD1306::drawString(_msg[_index]._x, _msg[_index]._y, _msg[_index]._value);
        }

        setFont(ArialMT_Plain_10);
        SSD1306::drawString(_log._x, _log._y, _log._value);

        display();
    }
}

void OLEDdisplay::log(String text)
{
    if(text != _log._value){
        _log = Text(text, -1, 64, 50);
        _updateNeeded = true;
    }
}

void OLEDdisplay::displayMsg(Text msg)
{
    boolean idFound = false;
    if (_msgNumber < NB_MSG_MAX){
        for(int i = 0; i<_msgNumber && !idFound;i++){
            if(_msg[i]._id == msg._id && msg._id != -1){
                idFound = true;
                if (_msg[i]._value != msg._value || _msg[i]._x != msg._x || _msg[i]._y != msg._y)
                {
                    _msg[i] = msg;
                    if(i==_index){
                        _updateNeeded = true;
                    }
                }
            }
        }
        if(!idFound){
            _msg[_msgNumber] = msg;
            _msgNumber++;
        }
    }
}

void OLEDdisplay::clearMsg(int id)
{
    for(int i = 0; i< _msgNumber; ++i){
        if(_msg[i]._id == id){
            if(i == _msgNumber-1){
                _msg[i].clear();
            }else {
                _msg[i] = _msg[_msgNumber-1];
                _msg[_msgNumber-1].clear();
            }
            _msgNumber--;
            if(_index>= _msgNumber){
                _index = 0;
                _updateNeeded = true;
            }
            if(_index == i)
                _updateNeeded = true;
        }
    }
}

void OLEDdisplay::clearAll()
{
    for (int i = 0; i < _msgNumber; ++i)
    {
        _msg[i].clear();
    }
    _msgNumber = 0;
    _index = 0;
    _updateNeeded = true;
}