#ifndef __ACCELERATIONCONTROLLER_H__
#define __ACCELERATIONCONTROLLER_H__

#include "../common/Clock.h"
#include "../common/LSM303.h"
#include "../common/MagneticCompas.h"

class AccelerationController{

public : 
	typedef struct{
		int x;
		int y;
		int orientation;
	}ProjectedAccelSum;

	AccelerationController();

	void computeProjection(int xAccel, int yAccel, int orientation);
	void resetMean();
	int getMeanAcceleration_X();
	int getMeanAcceleration_Y();
	int getOrientation();

private : 

	ProjectedAccelSum _fieldproject;
	int _nbMes;

};

#endif