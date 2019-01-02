#ifndef __MOVEBATCH_H
#define __MOVEBATCH_H

#include <Arduino.h>

#define MAX_JOINTS 3

typedef struct 
{
    bool isActive;
    int32_t position;
}MoveCommand_t;

class MoveBatch
{
    public:
        MoveBatch();
        void addMove(uint8_t id, double pos);
        MoveCommand_t batch[MAX_JOINTS];
        bool is_active() const {return isActive;}
        double get_duration() const {return moveDuration};
    private:
        bool isActive;
        double moveDuration;
};

#endif