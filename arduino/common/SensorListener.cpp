#include "SensorListener.h"

SensorListener::SensorListener() : m_sensor(0), m_interTimestep(0.0), m_internClock(), m_mesures()
{
    m_internClock.restart();
    for(int i=0;i<LISTENER_MAX_HISTORY;i++) m_oldStd[i] = m_oldVar[i] = -1;
}


void SensorListener::attach(UltrasonicSensor* sensor, int timestep)
{
    m_sensor  = sensor;
    m_interTimestep = timestep;
}

void SensorListener::process(float timestep)
{
    if(m_mesures.count()<LISTENER_MAX_MESURE) m_mesures.enqueue(m_sensor->getMesure());

    if(m_internClock.getElapsedTime()>m_interTimestep*0.001)
    {
        //Compute std and var
        int nb_measure = m_mesures.count();
        float std = 0.0, var  =0.0;
        float m;
        for(int i=0;i<nb_measure;i++)
        {
            m = m_mesures.dequeue();
            if (m==(float) NULL)
            {
                nb_measure = i+1;
                break;
            }
            
            std += m;
            m_mesures.enqueue(m);
        }
        std /= nb_measure;
        for(int i=0;i<nb_measure;i++)
        {
            m = m_mesures.dequeue();
            if (m==(float) NULL)
            {
                var=0;
                break;
            }
            var += pow((m-std),2);
        }
        var /= nb_measure;

        storeStd(std);
        storeVar(var);


        m_internClock.restart();
    }



}

void SensorListener::storeStd(float std)
{
    for(int i=(LISTENER_MAX_HISTORY-1);i>=1;i--) m_oldStd[i] = m_oldStd[i-1];

    m_oldStd[0] = std;

}


void SensorListener::storeVar(float var)
{
    for(int i=(LISTENER_MAX_HISTORY-1);i>=1;i--) m_oldVar[i] = m_oldVar[i-1];

    m_oldVar[0] = var;

}


float SensorListener::getStd(int delta){ return m_oldStd[delta];}

float SensorListener::getVar(int delta){ return m_oldVar[delta];}
