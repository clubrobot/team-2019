#ifndef __COLORSENSOR_H__
#define __COLORSENSOR_H__

class ButtonCard {
    public :
        void setup();
        void setLed(int, bool);
        bool readButton(int);
        bool readTirette();
        bool readEmergency();
        bool readMode();
};

#endif