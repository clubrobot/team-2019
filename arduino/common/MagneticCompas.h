#ifndef __MAGNETICCOMPAS_H__
#define __MAGNETICCOMPAS_H__
/*
    Compute the orientation off a chip using Earth magnetic field. 
    A 0 degree orientation means the x value is pointed toward the North.
    This was created for a LMS303 sensor.

    In Rennes the offset for North is -0.29 rad

*/

//calibrationValue
#define DEG_90 140
#define DEG_180 216
#define DEG_270 279


class MagneticCompas
{

  private:
    int _orientation;
    int _offset;
    
    int calibrationTransform(int orientation);

  public:
    void setOffset(int off);

    //Return the orientation of the X axis in degree
    int computeOrientation(int mag_x, int mag_y);

    //Set an offset so those data return position degree
    void setOrientation(int position, int mag_x, int mag_y);
};

#endif // __MAGNETICCOMPAS_H__
