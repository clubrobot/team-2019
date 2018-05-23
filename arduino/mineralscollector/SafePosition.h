//Classe utilisée pour garder les percuteurs en position pour qu'ils ne soient pas déformés 
#ifndef _SAFEPOSITION_H_
#define _SAFEPOSITION_H_

#include "../common/PeriodicProcess.h"
#include "../common/DCMotor.h"
#include "../common/Clock.h"

class SafePosition : public PeriodicProcess{
private:
	DCMotor* m_motor;

public:  
	SafePosition(DCMotor* motor);
	void toSafePosition();

protected:
	
	virtual void process(float timestep);
};
#endif