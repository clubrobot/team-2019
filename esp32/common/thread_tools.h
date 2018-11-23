#ifndef _PTHREAD_TOOLS_H__
#define _PTHREAD_TOOLS_H__


#include <pthread.h>



class Semaphore
{
    SemaphoreHandle_t _core;

    public:
        Semaphore(int init_val, int max_val)
        {
           _core =  xSemaphoreCreateCounting(init_val, max_val);
           if(_core==NULL) throw -1;
        }
        bool acquire(int wait_time=-1) const
        {
            return (pdTRUE == xSemaphoreTake( _core, ( TickType_t ) wait_time ));
        }
        bool release() const
        {
            return (pdTRUE == xSemaphoreGive( _core));
 
        }
};


class Mutex
{
    SemaphoreHandle_t _core;

    public:
        Mutex()
        {
            _core = xSemaphoreCreateMutex();
            if(_core==NULL) throw -1;
        }

        bool acquire(int wait_time=-1) const
        {
            return (pdTRUE == xSemaphoreTake( _core, ( TickType_t ) wait_time ));
        }
        bool release() const
        {
            return (pdTRUE == xSemaphoreGive( _core));
        }

};

class Thread
{
    pthread_t _core;
    void * (*_target)(void *);
    public:
        Thread(void * (*_funct)(void * ), void * args) : _core(0), _target(_funct)
        {
            if(pthread_create(&_core, NULL, _target, args)!=0) throw -1;   
        }

        void kill()
        {
            pthread_cancel(_core);
        }

        void join()
        {
            pthread_join(_core, NULL);
        }
        //TODO non blocking join or get_status
};




#endif //_PTHREAD_TOOLS_H__