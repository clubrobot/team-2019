#ifndef __BRUSHLESSMOTOR_H__
#define __BRUSHLESSMOTOR_H__

#include <Arduino.h>
#include <Servo.h>
#include "SerialTalks.h"
#include "PeriodicProcess.h"


#define MIN_PULSEWIDTH           1000     // the shortest pulse sent to a ESC  
#define MAX_PULSEWIDTH           2000     // the longest pulse sent to a ESC
#define MIN_VELOCITY             0
#define MAX_VELOCITY             100

class BrushlessMotor: public PeriodicProcess {

public:
	BrushlessMotor(): m_enabled(false), m_velocity(0), processingStartup(false){
        setTimestep(5);
    }

	void attach(int PIN);
    void detach();

	void enableStartup();
	void disableStartup();
    void updateStartup();
    void enableMotor();
    void disableMotor();
    int setVelocity(int velocity);
	int setPulsewidth(int pulsewidth);
    void forcePulsewidth(int pulsewidth);
    void update();
    void startupProcess();

    float getVelocity() const {return map(m_velocity,0,180,0,100);}
    bool  isEnabled() const {return m_enabled;}

    int readMicroseconds();

private:
    Servo m_esc;
    bool  m_enabled ;
    int m_velocity; // in %
    bool processingStartup;


protected:
    virtual void process(float timestep);
};

#endif // __BRUSHLESSMOTOR_H__
