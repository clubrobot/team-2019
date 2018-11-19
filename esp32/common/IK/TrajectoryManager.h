#ifndef __TRAJECTORYMANAGER_H
#define __TRAJECTORYMANAGER_H

#include <Arduino.h>
#include "Picker.h"
#include "ArmManager.h"

#define RUNNING_CORE 0

typedef enum{
    ARRIVED,
    ON_THE_ROAD,
    ERROR,
}is_arrived_t;

typedef struct
{
    SemaphoreHandle_t semaphore;
    is_arrived_t      is_arrived;

    coords_t start_coord;
    coords_t vel;
    coords_t end_coord;

}trajectory_task_pv_t;

class TrajectoryManager
{

    public :
        TrajectoryManager();
        /* go directly to pos */
        double goto_directly(double x, double y, double phi);
        /* go to pos with path */
        double goto_path(double x, double y, double phi);

        is_arrived_t is_arrived(void){return m_task_parameters.is_arrived;}

    private :

        trajectory_task_pv_t m_task_parameters;
       
        
};

#endif /* __TRAJECTORYMANAGER_H */