#include <Arduino.h>
#include "TrajectoryManager.h"
#include "Picker.h"
#include "ArmManager.h"

using namespace IK;
using namespace std;

#ifdef IK_LOG
    #define LOG_TRAJ(arg) cout << __TIME__<<" (TRAJ MANAGER)("<< __func__ << " , " << __LINE__ << ")\t\t: "<< arg <<endl;
#else
    #define LOG_TRAJ(arg) 
#endif

namespace IK
{

void TrajectoryManager::set_armManager(ArmManager& manager)
{
    m_mutex.acquire();
    m_manager = &manager;
    m_mutex.release();
}

void TrajectoryManager::set_Motors(MotorWrapper& motor1, MotorWrapper& motor2, MotorWrapper& motor3)
{
    m_mutex.acquire();
    m_motor1 = &motor1;
    m_motor2 = &motor2;
    m_motor3 = &motor3;
    m_mutex.release();
}

void TrajectoryManager::move_directly(coords_t pos)
{
    m_mutex.acquire();

    MoveBatch mb;

    /* set_parameters */
    coords_t end_coord     = pos;
    coords_t start_coord   = m_manager->get_tool();

    LOG_TRAJ("start : " << start_coord);
    LOG_TRAJ("end : " << end_coord );

    mb = m_manager->go_to(start_coord, end_coord);

    addMoveBatch(mb);
    m_mutex.release();
}

void TrajectoryManager::addMoveBatch(MoveBatch mb)
{
    LOG_TRAJ("ADD BATCH");
    _batchQueue.push(mb);
    _batchQueue.setResetPoint();
}

// NOTE: for debugging purposes.
MoveBatch TrajectoryManager::popMoveBatch()
{
    return _batchQueue.pop();
}

// NOTE: for debugging purposes.
MoveBatch TrajectoryManager::peekMoveBatch()
{
    return _batchQueue.peek();
}

void TrajectoryManager::process(float timestep)
{
    m_mutex.acquire();
    static double th1, th2, th3, th1_inter, th2_inter, th3_inter;
    if(!_isExecutingBatch)
    {
        if (_batchQueue.peek().is_active())
        {
            MoveBatch mb = _batchQueue.pop();

            if(mb.is_active())
            {
                th1 = convert_deg(mb.batch[0].position);
                th2 = convert_deg(mb.batch[1].position);
                th3 = convert_deg(mb.batch[2].position);

                cout << th1 << endl;
                cout << th2 << endl;
                cout << th2 << endl;

                cout << mb.batch[0].vel << endl;
                cout << mb.batch[0].time << endl;

                cout << mb.batch[1].vel << endl;
                cout << mb.batch[1].time << endl;

                cout << mb.batch[2].vel << endl;
                cout << mb.batch[2].time << endl;

                cout << "............" << endl;
            }

            //_isExecutingBatch = true;
        }
    }
    else
    {
        // static bool var1 = false;
        // if(!var1)
        // {
        //     m_motor1->setGoalPos(th1);
        //     m_motor2->setGoalPos(th2);
        //     m_motor3->setGoalPos(th3);
        //     var1 = true;
        // }
        // m_motor1->process(timestep);
        // m_motor2->process(timestep);
        // m_motor3->process(timestep);

        // if(m_motor1->arrived() && m_motor2->arrived() && m_motor1->arrived())
        // {
        //     _isExecutingBatch = false;
        //     var1 = false;
        // }
    }
    
    m_mutex.release();
}

double TrajectoryManager::convert_deg(double theta)
{
    double th;

    th =  theta * (180.0 / M_PI);

    return th;
}
double TrajectoryManager::convert_speed(double theta_speed)
{
    double speed;

    speed = theta_speed * ( 60.0 / (2.0 * M_PI));

    return speed;
}


} /* end of namespace */