#include "AccelerationController.h" 
	


void AccelerationController::resetMean(){
	_fieldproject.x = 0;
	_fieldproject.y = 0;
	_nbMes = 0;
}

AccelerationController::AccelerationController(){
	_offset = 0;
	_fieldproject.x = 0;
	_fieldproject.y = 0;
	_fieldproject.orientation = 0;
	resetMean();
}

void AccelerationController::computeProjection(int xAccel, int yAccel, int zAccel, int orientation, int pitch, int roll){
	_fieldproject.orientation = ((360 - orientation) + _offset);
	float theta = _fieldproject.orientation * M_PI / 180;
	float pitchr = pitch * M_PI / 180;
	float rollr = roll * M_PI / 180;
	_fieldproject.x += xAccel * cos(theta) - yAccel * sin(theta) - zAccel*sin(pitchr);
	_fieldproject.y += yAccel * cos(theta) + xAccel * sin(theta) - zAccel*sin(rollr);
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

void AccelerationController::setOrientation(int orientationToset, int mesure){
	_offset = orientationToset - (360 - mesure);
	resetMean();
}