#ifndef __DIFFERENTIALCONTROLLER_H__
#define __DIFFERENTIALCONTROLLER_H__

#include "PeriodicProcess.h"
#include "PID.h"
#include "thread_tools.h"


class AbstractMotor
{
public:

	virtual ~AbstractMotor(){}

	virtual void setVelocity(float velocity) = 0;
	virtual float getMaxVelocity() const = 0;
};

class DifferentialController : public PeriodicProcess
{
public:

	DifferentialController() : m_linSetpoint(0), m_angSetpoint(0), m_axleTrack(1){}

	void setInputs   (float linInput,    float angInput);
	void setSetpoints(float linSetpoint, float angSetpoint);

	void setAxleTrack(float axleTrack);

	void setWheels(AbstractMotor& leftWheel, AbstractMotor& rightWheel){m_leftWheel = &leftWheel; m_rightWheel = &rightWheel;}
	
	void setPID(PID& linPID, PID& angPID){m_linPID = &linPID; m_angPID = &angPID;}

	float getLinSetpoint() const;
	float getAngSetpoint() const;

	float getLinOutput() const;
	float getAngOutput() const;

	float getAxleTrack() const;

	void load(int address);
	void save(int address) const;

protected:

	virtual void process(float timestep);
	virtual void onProcessEnabling();

	float m_linInput; // in w/e unit
	float m_angInput; // in w/e unit
	float m_linSetpoint; // in w/e unit
	float m_angSetpoint; // in w/e unit
	float m_axleTrack; // in mm

	float m_linVelOutput;
	float m_angVelOutput;

	AbstractMotor* m_leftWheel;
	AbstractMotor* m_rightWheel;
	PID* m_linPID;
	PID* m_angPID;

	Mutex m_mutex;
};

#endif // __DIFFERENTIALCONTROLLER_H__
