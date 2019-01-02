#ifndef __MOVEBATCH_H
#define __MOVEBATCH_H

#include <Arduino.h>

#define MAX_JOINTS 3

typedef struct 
{
    bool isActive;
    double position;
}MoveCommand_t;

class MoveBatch
{
    public:
        MoveBatch();
        
        void addMove(uint8_t id, double pos);
        void addInterMove(uint8_t id, double pos);
        void addDuration(double time);

        bool   is_active() const {return isActive;}
        bool   is_interBatch() const {return isInterBatch;}
        double get_duration() const {return moveDuration;}

        

        MoveCommand_t batch[MAX_JOINTS];
        MoveCommand_t inter_batch[MAX_JOINTS];
    private:
        bool isActive;
        bool isInterBatch;
        double moveDuration;
        
};

#endif