#ifndef __MOTOR_WRAPPER
#define __MOTOR_WRAPPER

#include <Arduino.h>
#include <vector>
#include "../AX12/AX12.h"
#include "thread_tools.h"
#include "mathutils.h"
#include "PeriodicProcess.h"
#include "PID.h"

using namespace std;

typedef struct
{
    uint8_t id;
    uint8_t timeout;
    uint8_t err_code;
}Motor_state_t;


namespace IK
{
#define RMP_TO_DEG_S 0.16666666666667

#define MIN(a,b) ((a)<(b)?(a):(b));
#define MAX(a,b) ((a)>(b)?(a):(b));

class MotorWrapper : public PeriodicProcess
{
    public:
        MotorWrapper();
        
        void setID(int id);
        void setOFFSET(float offset);

        int   getID()     const {return _id;}
        float getOFFSET() const {return _offset;}

        void init();

        void setGoalPos(float pos);
        void setVelocityProfile(vector<float> vel);

        bool arrived() const {return _arrived;}

        virtual void process(float timestep);

        void load(int address);
        void save(int address) const;

        void end(){_motor.end();}

    private:

        float _velInput;
        float _posInput;
        float _pos;
        float _offset;
        int   _id;
        int _step_counter;
        bool _arrived;

        Motor_state_t _state;

        vector<float> _vel_profile;

        AX12 _motor;

        Mutex _mutex;
	
};

}

#endif /* __MOTOR_WRAPPER */