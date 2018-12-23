#include "MagneticCompas.h"
#include <Arduino.h>



void MagneticCompas::setOffset(int off){
    _offset = off;
}

int MagneticCompas::computeOrientation(int mag_x, int mag_y){
    _orientation = atan2(mag_y, mag_x) * 180 / M_PI;
    
    _orientation = _orientation >= 360 ?_orientation - 360 : _orientation;
    _orientation = _orientation < 0 ?_orientation + 360 : _orientation;
    
    _orientation  = calibrationTransform(_orientation);

    _orientation += _offset;

    _orientation = _orientation >= 360 ?_orientation - 360 : _orientation;
    _orientation = _orientation < 0 ?_orientation + 360 : _orientation;

    return _orientation;
}


void MagneticCompas::setOrientation(int position, int mag_x, int mag_y){
    int orientation = atan2(mag_y, mag_x) * 180 / M_PI;
    
    orientation = orientation >= 360 ?orientation - 360 : orientation;
    orientation = orientation < 0 ?orientation + 360 : orientation;
    
    orientation  = calibrationTransform(orientation);
    _offset = position-orientation;
}

int MagneticCompas::calibrationTransform(int x){
    int y;
    if(0 <= x && x < DEG_90)
        y = map(x, 0, DEG_90, 0, 90);
    else if (DEG_90 <= x && x < DEG_180)
        y = map(x, DEG_90, DEG_180, 90, 180);
    else if (DEG_180 <= x && x < DEG_270)
        y = map(x, DEG_180, DEG_270, 180, 270);
    else if (DEG_270 <= x && x < 360)
        y = map(x, DEG_270, 360, 270, 360);
    return y;
}

