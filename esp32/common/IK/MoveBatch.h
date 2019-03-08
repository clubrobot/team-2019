#ifndef __MOVEBATCH_H
#define __MOVEBATCH_H

#include <Arduino.h>
#include <vector>
#include <iostream>
#include "thread_tools.h"

#define MAX_JOINTS 3

using namespace std;

typedef struct 
{
    bool isActive;
    float position;
    vector<float> vel;
    vector<float> time;
}MoveCommand_t;

class MoveBatch
{
    public:
        MoveBatch();
        
        void addMove(uint8_t id, float pos);
        void addVelocityProfile(uint8_t id, vector<float> vel, vector<float> time);
        void addDuration(float time);

        bool   is_active() const {return isActive;}
        float get_duration() const {return moveDuration;}

        MoveCommand_t batch[MAX_JOINTS];
    private:
        bool isActive;
        float moveDuration;

        Mutex _mutex;
        
};

#endif