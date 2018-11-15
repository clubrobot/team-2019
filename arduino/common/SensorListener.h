#ifndef __SENSOR_LISTENER_H__
#define __SENSOR_LISTENER_H__
#include "UltrasonicSensor.h"
#include "Clock.h"
#include "QueueArray.h"
#include "PeriodicProcess.h"

#define LISTENER_MAX_MESURE 25
#define LISTENER_MAX_HISTORY 4


class SensorListener : public PeriodicProcess
{
    public:
        SensorListener();

        void attach(UltrasonicSensor* sensor, int timestep);

        void process(float timestep);

        float getStd(int delta);
        float getVar(int delta);


    private:
    
        void storeStd(float std);
        void storeVar(float var);

        UltrasonicSensor* m_sensor;
        int m_interTimestep; // ms
        Clock m_internClock;
        QueueArray<float> m_mesures;
        float m_oldStd[LISTENER_MAX_HISTORY];
        float m_oldVar[LISTENER_MAX_HISTORY];


};

#endif