#include <Arduino.h>
#include "MoveBatch.h"

MoveBatch::MoveBatch()
{
    m_mutex.acquire();

    isActive = false;
    moveDuration = 1.0;

    for (uint8_t i = 0; i < MAX_JOINTS; i++)
    {
        batch[i].isActive = false;
        batch[i].position = 150;
        batch[i].vel.push_back(0);
        batch[i].time.push_back(0);
    }

    m_mutex.release();
}

void MoveBatch::addMove(uint8_t id, double pos)
{
    m_mutex.acquire();
    isActive = true;

    if (id < MAX_JOINTS)
    {
        batch[id].isActive = true;
        batch[id].position = pos;
    }
    m_mutex.release();
}

void MoveBatch::addVelocityProfile(uint8_t id, vector<double> vel, vector<double> time)
{
    m_mutex.acquire();
    isActive = true;

    if (id < MAX_JOINTS)
    {
        batch[id].vel  = vel;
        batch[id].time = time;
    }
    m_mutex.release();
}

void MoveBatch::addDuration(double time)
{
    m_mutex.acquire();
    moveDuration = time;
    m_mutex.release();
}
