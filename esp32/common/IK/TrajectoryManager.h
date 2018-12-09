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

typedef struct
{

}trajectory_task_pv_t;

namespace IK
{

class TrajectoryManager : public ArmManager, public TaskManager, public MotorWrapper
{

    public :
    
        TrajectoryManager(double dt = 0.2) throw() : ArmManager(dt){}

        void attach(int id_1, int id_2, int id_3) throw();
        void begin(coords_t initial_pos);
        /* go directly to pos */
        double goto_directly(double x, double y, double phi);
        /* go to pos with path */
        double goto_path(double x, double y, double phi);

        double goto_home();

        void set_status(status_t status) throw();

        status_t get_status() const throw();

        bool move_directly();
        bool move_path();
        bool move_home();

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