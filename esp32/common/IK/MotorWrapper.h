#ifndef __MOTOR_WRAPPER
#define __MOTOR_WRAPPER

#include <Arduino.h>
#include "../AX12/AX12.h"
#include "thread_tools.h"

namespace IK
{

class MotorWrapper
{
    public:
        MotorWrapper();
        void attach(int id1, int id2, int id3);
        void detach();

        void init();
        void init_offsets(double offset1, double offset2, double offset3);

        void move(double th1, double th2, double th3);
        void moveSpeed(double th1, double th1_speed, double th2,double th2_speed, double th3, double th3_speed);

        bool position_reached();
        void motor_read();

        bool converge_to_pos();

    private:

        

        bool equals(double a, double b, double epsilon = 1);

        double m_th1;
        double m_th2;
        double m_th3;

        double m_offset1;
        double m_offset2;
        double m_offset3;

        double m_cur_pos1;
        double m_cur_pos2;
        double m_cur_pos3;

        double delta_pos1;
        double delta_pos2;
        double delta_pos3;

        AX12 m_AX1;
        AX12 m_AX2;
        AX12 m_AX3;

        AX12 m_AX_Broadcast;

        Mutex m_mutex;
};

}

#endif /* __MOTOR_WRAPPER */