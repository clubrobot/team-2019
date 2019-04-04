#include <Arduino.h>
#include "MoveBatch.h"

MoveBatch::MoveBatch()
{
    _mutex.acquire();

    isActive = false;
    moveDuration = 1.0;

    for (uint8_t i = 0; i < MAX_JOINTS; i++)
    {
        batch[i].isActive = false;
        batch[i].position = 150;
        batch[i].vel.push_back(0);
        batch[i].time.push_back(0);
    }

    _mutex.release();
}

void MoveBatch::addMove(uint8_t id, float pos)
{
    _mutex.acquire();
    isActive = true;

    if (id < MAX_JOINTS)
    {
        batch[id].isActive = true;
        batch[id].position = pos;
    }
    _mutex.release();
}

void MoveBatch::addVelocityProfile(uint8_t id, vector<float> vel, vector<float> time)
{
    _mutex.acquire();
    isActive = true;

    if (id < MAX_JOINTS)
    {
        batch[id].vel  = vel;
        batch[id].time = time;
    }
    _mutex.release();
}

void MoveBatch::addDuration(float time)
{
    _mutex.acquire();
    moveDuration = time;
    _mutex.release();
}
