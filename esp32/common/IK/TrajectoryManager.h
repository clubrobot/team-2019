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

typedef enum{
    ARRIVED     = 0X00,
    ON_THE_ROAD = 0X01,
    ERROR       = 0X02,
}status_t;

#define MAX_NUM_OF_BATCHED_MOVES 10

namespace IK
{

class TrajectoryManager
{

    public :

        TrajectoryManager() throw(){}
        void set_armManager(ArmManager& manager);
        void set_Motors(MotorWrapper& motors);

        void move_directly(coords_t pos);

        void update();

    private :

        double convert_deg(double theta);
        double convert_speed(double theta_speed);

        void addMoveBatch(MoveBatch mb);
        MoveBatch popMoveBatch();
        MoveBatch peekMoveBatch();

        ArmManager      *m_manager;
        MotorWrapper    *m_motors;
        Mutex            m_mutex;

        Queue<MoveBatch> _batchQueue = Queue<MoveBatch>(MAX_NUM_OF_BATCHED_MOVES);
};
}

#endif /* __TRAJECTORYMANAGER_H */