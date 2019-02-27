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
        
        void attach(int id);
        void setOFFSET(float offset);

        void setGoalPos(float pos);
        void setVelocityProfile(vector<double> vel);

        bool arrived() const {return _arrived;}

        virtual void process(float timestep);

    private:

        float _velInput;
        float _posInput;
        float _pos;
        float _offset;
        int _step_counter;
        bool _arrived;

        bool _error_occur;

        Motor_state_t _state;

        vector<double> _vel_profile;

        AX12 _motor;

        Mutex _mutex;
	
};

}

#endif /* __MOTOR_WRAPPER */