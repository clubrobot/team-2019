#ifndef __ARMMANAGER_H
#define __ARMMANAGER_H

#include <Arduino.h>
#include "Picker.h"
#include "Joint.h"
#include "MotorWrapper.h"
#include "../AX12/AX12.h"
#include "thread_tools.h"
#include "arm_config.h"
#include "datatype.h"

#define AX12_SPEED(x) (x * ( 60.0 / (2.0 * M_PI)))

namespace IK
{

class ArmManager : public Picker
{
    public:

        ArmManager() throw(){};

        void        init_workspace(workspace_t ws_front, workspace_t ws_back) throw();
        void        set_origin(coords_t origin) throw();
        void        init_arm(double l1, double l2, double l3, int elbow_or);

        workspace_t clip_workspace_to_constraints(workspace_t workspace) throw();
        coords_t    workspace_center(workspace_t workspace) throw();
        joints_t    go_to(coords_t start_pos, coords_t target_pos);
        joints_t    goto_position(coords_t target_pos);
        double      estimated_time_of_arrival(coords_t start_pos, coords_t start_vel, coords_t target_pos, coords_t target_vel);
        
    private:

        workspace_t workspace_containing_position(coords_t position) throw();
        bool        workspace_within_constraints(workspace_t workspace) throw();
        bool        position_within_workspace(coords_t position, workspace_t workspace) throw();
        joints_t    goto_workspace(coords_t start_pos, coords_t target_pos, workspace_t new_workspace);

        workspace_t m_ws_front;
        workspace_t m_ws_back;
        workspace_t m_workspace;

        coords_t m_tool;
        coords_t m_origin;
        joints_t m_joints;

        Mutex m_mutex;
};

}

#endif /* __ARMMANAGER_H */