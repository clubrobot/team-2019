#ifndef __TRAJECTORYMANAGER_H
#define __TRAJECTORYMANAGER_H

#include <Arduino.h>
#include "Picker.h"
#include "ArmManager.h"
#include "thread_tools.h"
#include "constants.h"
#include "MoveBatch.h"
#include "Queue.h"
#include "PeriodicProcess.h"

#define MAX_NUM_OF_BATCHED_MOVES 50

namespace IK
{

class TrajectoryManager : public PeriodicProcess
{

    public :

        TrajectoryManager() throw():_isExecutingBatch(false),_arrived(false),_error(false){}

        void set_armManager(ArmManager& manager);
        void set_Motors(MotorWrapper& motor1, MotorWrapper& motor2, MotorWrapper& motor3);
        void set_timestep(float timestep);

        float get_timestep() const;

        void init();

        void move_directly(Coords pos);
        bool is_arrived() const;

        void load(int address);
        void save(int address) const;

    private :
        virtual void process(float timestep);

        float convert_deg(float theta);
        float convert_speed(float theta_speed);

        void addMoveBatch(MoveBatch mb);

        MoveBatch popMoveBatch();
        MoveBatch peekMoveBatch();

        ArmManager      *_manager;

        MotorWrapper    *_motor1;
        MotorWrapper    *_motor2;
        MotorWrapper    *_motor3;

        Queue<MoveBatch> _batchQueue = Queue<MoveBatch>(MAX_NUM_OF_BATCHED_MOVES);

        bool _isExecutingBatch;

        bool _arrived;
        bool _error;

        float _timestep;

        Mutex _mutex;
};
}

#endif /* __TRAJECTORYMANAGER_H */