#include <Arduino.h>
#include "MoveBatch.h"

MoveBatch::MoveBatch()
{
    isActive = false;
    //moveDuration = 1.0;

    for (uint8_t i = 0; i < MAX_JOINTS; i++)
    {
        batch[i] = {false, 0};
    }
}

void MoveBatch::addMove(uint8_t id, int32_t pos)
{
    isActive = true;

    if (id < MAX_NUM_OF_STEPPERS)
    {
        batch[id] = {true, pos};
    }
}