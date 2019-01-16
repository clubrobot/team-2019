#ifndef __TRAJECTORYMANAGER_H
#define __TRAJECTORYMANAGER_H

#include <Arduino.h>
#include "Picker.h"
#include "ArmManager.h"
#include "thread_tools.h"
#include "arm_config.h"
#include "TaskManager.h"
#include "MoveBatch.h"
#include "Queue.h"
#include "PeriodicProcess.h"

typedef enum{
    ARRIVED     = 0X00,
    ON_THE_ROAD = 0X01,
    ERROR       = 0X02,
}status_t;

#define MAX_NUM_OF_BATCHED_MOVES 50

namespace IK
{

class TrajectoryManager : public PeriodicProcess
{

    public :

        TrajectoryManager() throw(){}
        void set_armManager(ArmManager& manager);
        void set_Motors(MotorWrapper& motor1, MotorWrapper& motor2, MotorWrapper& motor3);

        void move_directly(coords_t pos);

        void motor1_update(){m_motor1->process(0);}
        void motor2_update(){m_motor2->process(0);}
        void motor3_update(){m_motor3->process(0);}

        bool motor1_arrived() const {return m_motor1->arrived();}
        bool motor2_arrived() const {return m_motor2->arrived();}
        bool motor3_arrived() const {return m_motor3->arrived();}

    private :
        virtual void process(float timestep);

        double convert_deg(double theta);
        double convert_speed(double theta_speed);

        void addMoveBatch(MoveBatch mb);
        MoveBatch popMoveBatch();
        MoveBatch peekMoveBatch();

        ArmManager      *m_manager;

        MotorWrapper    *m_motor1;
        MotorWrapper    *m_motor2;
        MotorWrapper    *m_motor3;

        Queue<MoveBatch> _batchQueue = Queue<MoveBatch>(MAX_NUM_OF_BATCHED_MOVES);

        bool _isExecutingBatch;
        bool _interBatch;

        Mutex m_mutex;
};
}

#endif /* __TRAJECTORYMANAGER_H */