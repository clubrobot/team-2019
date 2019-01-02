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
    m_manager = &manager;
}

void TrajectoryManager::set_Motors(MotorWrapper& motors)
{
    m_motors = &motors;
}

void TrajectoryManager::move_directly(coords_t pos)
{
    MoveBatch mb;

    /* set_parameters */
    coords_t end_coord     = pos;
    coords_t start_coord   = m_manager->get_tool();

    LOG_TRAJ("start : " << start_coord);
    LOG_TRAJ("end : " << end_coord );

    mb = m_manager->go_to(start_coord, end_coord);

    addMoveBatch(mb);
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

void TrajectoryManager::update()
{
    if (_batchQueue.peek().is_active())
    {
        MoveBatch mb = _batchQueue.pop();

        if(mb.is_active())
        {
            cout << "batch th1 :" << convert_deg(mb.batch[0].position) <<endl;
            cout << "batch th2 :" << convert_deg(mb.batch[1].position) <<endl;
            cout << "batch th3 :" << convert_deg(mb.batch[2].position) <<endl;
        }
        if(mb.is_interBatch())
        {
            cout << "inter batch th1 :" << convert_deg(mb.inter_batch[0].position) <<endl;
            cout << "inter batch th2 :" << convert_deg(mb.inter_batch[1].position) <<endl;
            cout << "inter batch th3 :" << convert_deg(mb.inter_batch[2].position) <<endl;
        }
        cout << endl;
    }
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