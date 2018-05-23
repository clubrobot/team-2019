#include "../common/PeriodicProcess.h"
#include "../common/DCMotor.h"
#include "../common/Clock.h"
#include "SafePosition.h"

#define TIMETOPOSITION 3

SafePosition::SafePosition(DCMotor* motor){
	m_motor = motor;
}
	
	
void SafePosition::toSafePosition(){
	m_motor->setVelocity(-3);
	disable();
	enable();
}

void SafePosition::process(float timestep){
	m_motor->setVelocity(0);
	disable();
}