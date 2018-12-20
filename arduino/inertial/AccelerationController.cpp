#include "AccelerationController.h" 
	


void AccelerationController::resetMean(){
	_fieldproject.x = 0;
	_fieldproject.y = 0;
	_nbMes = 0;
}

AccelerationController::AccelerationController(){
	resetMean();
}

void AccelerationController::computeProjection(int xAccel, int yAccel, int orientation){
	float theta = orientation * M_PI / 180;
	yAccel = -yAccel - 93;
	xAccel -=23;
	_fieldproject.x += xAccel * cos(theta) - yAccel * sin(theta);
	_fieldproject.y += yAccel * cos(theta) + xAccel * sin(theta);
	_fieldproject.orientation = orientation;
	_nbMes ++;
}

int AccelerationController::getMeanAcceleration_X(){
	if(_nbMes != 0)
		return _fieldproject.x/_nbMes;
	else return 0;
}
int AccelerationController::getMeanAcceleration_Y(){
	if(_nbMes != 0)
		return _fieldproject.y/_nbMes;
	else return 0;
}
int AccelerationController::getOrientation(){
	return _fieldproject.orientation;
}
