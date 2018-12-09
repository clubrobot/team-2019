#ifndef __COLORSENSOR_H__
#define __COLORSENSOR_H__

class ColorSensor {
    public :
        bool activated;
        void setup();
        int getRed(void);
        int getGreen(void);
        int getBlue(void);
};

#endif