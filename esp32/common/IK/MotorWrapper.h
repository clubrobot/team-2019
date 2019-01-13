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

namespace IK
{
#define RMP_TO_DEG_S 0.16666666666667

#define MIN(a,b) ((a)<(b)?(a):(b));
#define MAX(a,b) ((a)>(b)?(a):(b));

class MotorWrapper //: public PeriodicProcess
{
    public:
        MotorWrapper();
        
        void setPID(PID& pid){m_PID = &pid;}
        void setMOTOR(AX12& motor){m_motor = &motor;}
        void setOFFSET(float offset){m_offset = offset;}

        void setGoalPos(float pos){m_pos = pos; m_step_counter = 0;}
        void setVelocityProfile(vector<double> vel){m_vel_profile = vel;}

        bool arrived() const {return m_arrived;}

        void process(float timestep);

    private:

        float m_velInput;
        float m_posInput;
        float m_pos;
        float m_offset;
        int m_step_counter;
        bool m_arrived;

        vector<double> m_vel_profile;

        PID* m_PID;
        AX12* m_motor;
	
};

}

#endif /* __MOTOR_WRAPPER */