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

#define CONVERT_DEG(x) (( x * (180.0 / M_PI)))

namespace IK
{

class ArmManager : public Picker, public MotorWrapper
{
    public:

        ArmManager(double dt = 0.2) throw();

        void        init_workspace(workspace_t ws_front, workspace_t ws_back) throw();
        void        set_origin(coords_t origin) throw();
        void        attach(int id_1, int id_2, int id_3, double l1, double l2, double l3) throw();
        void        init_arm(coords_t initial_pos, int elbow_or);

        workspace_t clip_workspace_to_constraints(workspace_t workspace) throw();
        coords_t    workspace_center(workspace_t workspace) throw();
        path_t      go_to(coords_t start_pos, coords_t start_vel, coords_t target_pos, coords_t target_vel);
        path_t      go_home(coords_t start_pos, coords_t start_vel);
        path_t      goto_position(coords_t start_pos, coords_t start_vel, coords_t target_pos, coords_t target_vel);
        double      estimated_time_of_arrival(coords_t start_pos, coords_t start_vel, coords_t target_pos, coords_t target_vel);
        
    private:

        path_t      merge_trajectories(path_t traj_a, path_t traj_b) throw();
        workspace_t workspace_containing_position(coords_t position) throw();
        bool        workspace_within_constraints(workspace_t workspace) throw();
        bool        position_within_workspace(coords_t position, workspace_t workspace) throw();
        path_t      goto_workspace(coords_t start_pos, coords_t start_vel, coords_t target_pos, coords_t target_vel, workspace_t new_workspace);

        workspace_t m_ws_front;
        workspace_t m_ws_back;
        workspace_t m_workspace;

        double m_dt;
        coords_t m_tool;
        coords_t m_origin;
        joints_t m_joints;

        /* AX12 motors */

        int m_id1;
        int m_id2;
        int m_id3;

        double m_len1;
        double m_len2;
        double m_len3;

        Mutex m_mutex;
};

}

#endif /* __ARMMANAGER_H */