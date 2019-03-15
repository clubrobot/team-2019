#ifndef __VELOCITYCONTROLLER_H__
#define __VELOCITYCONTROLLER_H__

#include "DifferentialController.h"

#include <math.h>

#define ENABLE_VELOCITYCONTROLLER_LOGS 0 // for debug purposes
#define VELOCITYCONTROLLER_LOGS_TIMESTEP 50e-3 // mm


class VelocityController : public DifferentialController
{
public:

	VelocityController() : m_rampLinVelSetpoint(0), m_rampAngVelSetpoint(0), m_maxLinAcc(INFINITY), m_maxLinDec(INFINITY), m_maxAngAcc(INFINITY), m_maxAngDec(INFINITY), m_spinShutdown(true){}

	void setMaxAcc(float maxLinAcc, float maxAngAcc){m_maxLinAcc = maxLinAcc; m_maxAngAcc = maxAngAcc;}
	void setMaxDec(float maxLinDec, float maxAngDec){m_maxLinDec = maxLinDec; m_maxAngDec = maxAngDec;}
	void setSpinShutdown(bool spinShutdown){m_spinShutdown = spinShutdown;}

	float getMaxLinAcc() const {return m_maxLinAcc;}
	float getMaxAngAcc() const {return m_maxAngAcc;}
	float getMaxLinDec() const {return m_maxLinDec;}
	float getMaxAngDec() const {return m_maxAngDec;}
	bool getSpinShutdown() const {return m_spinShutdown;}

	void load(int address);
	void save(int address) const;

protected:

	float genRampSetpoint(float stepSetpoint, float input, float rampSetpoint, float maxAcc, float maxDec, float timestep);

	virtual void process(float timestep);
	virtual void onProcessEnabling();

	float m_rampLinVelSetpoint; // in mm/s (no longer w/e unit)
	float m_rampAngVelSetpoint; // in rad/s (no longer w/e unit)
	float m_maxLinAcc; // always positive, in mm/s^2
	float m_maxLinDec; // always positive, in mm/s^2
	float m_maxAngAcc; // always positive, in rad/s^2
	float m_maxAngDec; // always positive, in rad/s^2
	bool m_spinShutdown;

#if ENABLE_VELOCITYCONTROLLER_LOGS
	friend class VelocityControllerLogs;
#endif // ENABLE_VELOCITYCONTROLLER_LOGS
};

#if ENABLE_VELOCITYCONTROLLER_LOGS
class VelocityControllerLogs : public PeriodicProcess
{
public:

	void setController(const VelocityController& controller){m_controller = &controller;}

protected:

	virtual void process(float timestep);

	const VelocityController* m_controller;
};
#endif // ENABLE_VELOCITYCONTROLLER_LOGS

#endif // __VELOCITYCONTROLLER_H__
