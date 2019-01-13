#include <Arduino.h>
#include "MoveBatch.h"

MoveBatch::MoveBatch()
{
    isActive = false;
    moveDuration = 1.0;

    for (uint8_t i = 0; i < MAX_JOINTS; i++)
    {
        batch[i].isActive = false;
        batch[i].position = 150;
        batch[i].vel.push_back(0);
        batch[i].time.push_back(0);
    }
}

void MoveBatch::addMove(uint8_t id, double pos)
{
    isActive = true;

    if (id < MAX_JOINTS)
    {
        batch[id].isActive = true;
        batch[id].position = pos;
    }
}

void MoveBatch::addVelocityProfile(uint8_t id, vector<double> vel, vector<double> time)
{
    isActive = true;

    vector<double> tmp_v = vel;
    vector<double> tmp_t = time;

    batch[id].vel.clear();
    batch[id].time.clear();

    if (id < MAX_JOINTS)
    {
        batch[id].vel.insert(batch[id].vel.end(), tmp_v.begin(), tmp_v.end());
        batch[id].time.insert(batch[id].time.end(), tmp_t.begin(), tmp_t.end());
    }
}

void MoveBatch::addDuration(double time)
{
    moveDuration = time;
}
