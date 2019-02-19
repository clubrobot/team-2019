#ifndef __TRAJECTORYMANAGER_H
#define __TRAJECTORYMANAGER_H

#include <Arduino.h>
#include "Picker.h"
#include "ArmManager.h"
#include "thread_tools.h"
#include "arm_config.h"
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

        void move_directly(coords_t pos);
        bool is_arrived() const {return _arrived;}
        bool get_error() const {return _error;}

        Motor_state_t get_motor1_state() const {return m_motor1->get_state();}
        Motor_state_t get_motor2_state() const {return m_motor2->get_state();}
        Motor_state_t get_motor3_state() const {return m_motor3->get_state();}

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

        bool _arrived;
        bool _error;

        Mutex m_mutex;
};
}

#endif /* __TRAJECTORYMANAGER_H */