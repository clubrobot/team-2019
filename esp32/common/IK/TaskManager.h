#ifndef __TASK_MANAGER
#define __TASK_MANAGER

#include <Arduino.h>
#include "arm_config.h"

class TaskManager
{
    public :
        TaskManager(): _coreID(RUNNING_CORE), _stackDepth(20000), _taskPriority(0){}

        bool create_task(TaskFunction_t TaskCode, void * const Parameters);
        bool delete_task();
        bool task_is_running(){return _task_is_runnig;}

    private : 

        BaseType_t   _coreID;
        uint32_t     _stackDepth;
        UBaseType_t  _taskPriority;
        TaskHandle_t _taskHandler;

        BaseType_t   _taskStatus;

        bool _task_is_runnig;

};

#endif /* __TASK_MANAGER */