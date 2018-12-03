#ifndef __TRAJECTORYMANAGER_H
#define __TRAJECTORYMANAGER_H

#include <Arduino.h>
#include "Picker.h"
#include "ArmManager.h"
#include "thread_tools.h"

constexpr int RUNNING_CORE = 0;
constexpr int TIMEOUT      = 10;

typedef enum{
    ARRIVED     = 0X00,
    ON_THE_ROAD = 0X01,
    ERROR       = 0X02,
}status_t;

typedef struct
{
    coords_t start_coord;
    coords_t vel;
    coords_t end_coord;

}trajectory_task_pv_t;

namespace IK
{

class TrajectoryManager : public ArmManager
{

    public :
    
        TrajectoryManager(double dt = 0.2) throw() : ArmManager(dt){}
        /* go directly to pos */
        double goto_directly(double x, double y, double phi);
        /* go to pos with path */
        double goto_path(double x, double y, double phi);

        double goto_home();

        void set_status(status_t status) throw();

        status_t get_status() throw();

    private :

        double convert_deg(double theta);
        double convert_speed(double theta_speed);

        status_t m_status;

        trajectory_task_pv_t m_task_parameters;

        Mutex m_mutex;
};

}

#endif /* __TRAJECTORYMANAGER_H */