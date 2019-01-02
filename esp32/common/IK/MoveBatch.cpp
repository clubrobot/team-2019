#include <Arduino.h>
#include "MoveBatch.h"

MoveBatch::MoveBatch()
{
    isActive = false;
    moveDuration = 1.0;
    isInterBatch = false;

    for (uint8_t i = 0; i < MAX_JOINTS; i++)
    {
        batch[i] = {false, 150};
    }

    for (uint8_t i = 0; i < MAX_JOINTS; i++)
    {
        inter_batch[i] = {false, 150};
    }
}

void MoveBatch::addMove(uint8_t id, double pos)
{
    isActive = true;

    if (id < MAX_JOINTS)
    {
        batch[id] = {true, pos};
    }
}

void MoveBatch::addInterMove(uint8_t id, double pos)
{
    isInterBatch = true;

    if (id < MAX_JOINTS)
    {
        inter_batch[id] = {true, pos};
    }
}

void MoveBatch::addDuration(double time)
{
    moveDuration = time;
}
