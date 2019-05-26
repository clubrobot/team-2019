#ifndef __ACCELERATIONCONTROLLER_H__
#define __ACCELERATIONCONTROLLER_H__

#include <Arduino.h>

class AccelerationController{

public : 
	typedef struct{
		int x;
		int y;
		int orientation;
	}ProjectedAccelSum;

	AccelerationController();

	void computeProjection(int xAccel, int yAccel, int zAccel, int orientation, int pitch, int roll);
	void resetMean();
	int getMeanAcceleration_X();
	int getMeanAcceleration_Y();
	int getOrientation();
	void setOrientation(int orientationToset, int mesure);

private : 

	ProjectedAccelSum _fieldproject;
	int _nbMes;
	int _offset;

};

#endif