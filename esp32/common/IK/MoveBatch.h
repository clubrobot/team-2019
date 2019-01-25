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
    double position;
    vector<double> vel;
    vector<double> time;
}MoveCommand_t;

class MoveBatch
{
    public:
        MoveBatch();
        
        void addMove(uint8_t id, double pos);
        void addVelocityProfile(uint8_t id, vector<double> vel, vector<double> time);
        void addDuration(double time);

        bool   is_active() const {return isActive;}
        double get_duration() const {return moveDuration;}

        MoveCommand_t batch[MAX_JOINTS];
    private:
        bool isActive;
        double moveDuration;

        Mutex m_mutex;
        
};

#endif