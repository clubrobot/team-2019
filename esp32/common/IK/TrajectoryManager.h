#ifndef __TRAJECTORYMANAGER_H
#define __TRAJECTORYMANAGER_H

#include <Arduino.h>
#include "Picker.h"
#include "ArmManager.h"
#include "thread_tools.h"
#include "arm_config.h"
#include "TaskManager.h"

typedef enum{
    ARRIVED     = 0X00,
    ON_THE_ROAD = 0X01,
    ERROR       = 0X02,
}status_t;


namespace IK
{

class TrajectoryManager : public ArmManager, public TaskManager, public MotorWrapper
{

    public :

        TrajectoryManager() throw(){}

        void attach(int id_1, int id_2, int id_3) throw();
        void begin(coords_t initial_pos);

        double goto_directly(coords_t pos);

        void set_status(status_t status) throw();

        status_t get_status() const throw();

        bool move_directly();

    private :

        double convert_deg(double theta);
        double convert_speed(double theta_speed);

        status_t m_status;

        coords_t m_start_coord;
        coords_t m_start_vel;
        coords_t m_end_vel;
        coords_t m_end_coord;

        double m_current_traj_time;

        Mutex m_mutex;
};
}

#endif /* __TRAJECTORYMANAGER_H */