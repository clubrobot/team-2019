#ifndef __POSITIONCONTROLLER_H__
#define __POSITIONCONTROLLER_H__

#include "PeriodicProcess.h"
#include "Odometry.h"
#include "thread_tools.h"


class AbstractMoveStrategy;

class PositionController : public PeriodicProcess
{
public:

	PositionController() : m_linVelKp(1), m_angVelKp(1), m_linVelMax(1000), m_angVelMax(2 * M_PI){}

	void setPosInput   (const Position& posInput);
	void setPosSetpoint(const Position& posSetpoint);

	void setThetaSetpoint(float theta);

	float getLinVelSetpoint() const;
	float getAngVelSetpoint() const;

	void setVelTunings(float linVelKp, float angVelKp);
	void setVelLimits(float linVelMax, float angVelMax);
	void setPosThresholds(float linPosThreshold, float angPosThreshold);

	void setMoveStrategy(AbstractMoveStrategy& moveStrategy);

	bool getPositionReached();

	float getLinVelKp() const; 
	float getAngVelKp() const;
	float getLinVelMax() const;
	float getAngVelMax() const;
	float getLinPosThreshold() const;
	float getAngPosThreshold() const;

	void load(int address);
	void save(int address) const;

private:

	virtual void process(float timestep);
	virtual void onProcessEnabling();

	// IO
	Position m_posInput;
	Position m_posSetpoint;

	Mutex m_mutex;

	float m_linVelSetpoint;
	float m_angVelSetpoint;

	// Engineering control tunings
	float m_linVelKp;
	float m_angVelKp;
	float m_linVelMax;
	float m_angVelMax;
	float m_linPosThreshold;
	float m_angPosThreshold;

	// Strategy Design Pattern
	AbstractMoveStrategy* m_moveStrategy;

	friend class AbstractMoveStrategy;
};

class AbstractMoveStrategy
{
protected:

	virtual void computeVelSetpoints(float timestep) = 0;
	virtual bool getPositionReached() = 0;

	const Position& getPosInput()    const {return m_context->m_posInput;}
	const Position& getPosSetpoint() const {return m_context->m_posSetpoint;}

	void setVelSetpoints(float linVelSetpoint, float angVelSetpoint){m_context->m_linVelSetpoint = linVelSetpoint; m_context->m_angVelSetpoint = angVelSetpoint;}

	float getLinVelKp() const {return m_context->m_linVelKp;}
	float getAngVelKp() const {return m_context->m_angVelKp;}
	float getLinVelMax() const {return m_context->m_linVelMax;}
	float getAngVelMax() const {return m_context->m_angVelMax;}
	float getLinPosThreshold() const {return m_context->m_linPosThreshold;}
	float getAngPosThreshold() const {return m_context->m_angPosThreshold;}

private:

	PositionController* m_context;

	friend class PositionController;
};

#endif // __POSITIONCONTROLLER_H__
