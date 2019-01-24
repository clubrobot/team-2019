#ifndef __TASK_MANAGER
#define __TASK_MANAGER

#include <Arduino.h>
#include "thread_tools.h"

class TaskManager
{
    public :
        TaskManager(): _coreID(0), _stackDepth(8192), _taskPriority(1), _taskHandler(NULL){}

        bool create_task(TaskFunction_t TaskCode, void * const Parameters);
        void delete_task();
        bool task_is_running();

    private : 

        BaseType_t   _coreID;
        uint32_t     _stackDepth;
        UBaseType_t  _taskPriority;
        TaskHandle_t _taskHandler;

        BaseType_t   _taskStatus;

        bool _task_is_runnig;

        Mutex m_mutex;

};

#endif /* __TASK_MANAGER */