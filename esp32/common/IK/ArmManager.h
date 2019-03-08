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
#include "MoveBatch.h"

namespace IK
{

class ArmManager : public Picker
{
    public:

        ArmManager() throw(){};

        void        init_workspace(workspace_t ws_front, workspace_t ws_back) throw();
        void        set_origin(coords_t origin) throw();
        void        set_arm_link(float l1, float l2, float l3, int elbow_or);
        void        init_arm();
        void        set_initial_joint_pos(joints_t joints) {_joints = joints;}

        workspace_t clip_workspace_to_constraints(workspace_t workspace) throw();
        coords_t    workspace_center(workspace_t workspace) throw();
        MoveBatch   go_to(coords_t start_pos, coords_t target_pos);

        float      estimated_time_of_arrival(coords_t start_pos, coords_t start_vel, coords_t target_pos, coords_t target_vel);
        
    private:

        workspace_t workspace_containing_position(coords_t position) throw();
        bool        workspace_within_constraints(workspace_t workspace) throw();
        bool        position_within_workspace(coords_t position, workspace_t workspace) throw();
        MoveBatch   goto_workspace(coords_t start_pos, coords_t target_pos, workspace_t new_workspace);
        joints_t    goto_position(coords_t target_pos);

        workspace_t _ws_front;
        workspace_t _ws_back;
        workspace_t _workspace;

        coords_t _tool;
        coords_t _origin;
        joints_t _joints;

        float _l1;
        float _l2;
        float _l3;
        float _elbow;

        Mutex _mutex;
};

}

#endif /* __ARMMANAGER_H */