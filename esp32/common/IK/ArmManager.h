#ifndef __ARMMANAGER_H
#define __ARMMANAGER_H

#include "Picker.h"
#include "Joint.h"

typedef struct
{
    double x_min;
    double x_max;

    double y_min;
    double y_max;

    double phi_min;
    double phi_max;
}workspace_t;

typedef struct
{
    path_t q1;
    path_t q2;
    path_t q3;
    path_t q4;

    coords_t pos;
}traj_t;

class ArmManager
{
    public:

        ArmManager(Picker *arm, workspace_t workspace, double time_resolution);
        workspace_t workspace_containing_position(coords_t position);
        bool        workspace_within_constraints(workspace_t workspace);
        workspace_t clip_workspace_to_constraints(workspace_t workspace);
        bool        position_within_workspace(coords_t position, workspace_t workspace);
        coords_t    workspace_center(workspace_t workspace);
        traj_t      go_to(coords_t start_pos, coords_t start_vel, coords_t target_pos, coords_t target_vel);
        traj_t      go_home(coords_t start_pos, coords_t start_vel);
        path_t      goto_workspace(coords_t start_pos, coords_t start_vel, coords_t target_pos, coords_t target_vel, workspace_t new_workspace);
        path_t      goto_position(coords_t start_pos, coords_t start_vel, coords_t target_pos, coords_t target_vel);
        trajectory_time_t estimated_time_of_arrival(coords_t start_pos, coords_t start_vel, coords_t target_pos, coords_t target_vel);

    private:

        path_t merge_trajectories(path_t traj_a, path_t traj_b);

        Picker* m_arm;

        workspace_t m_ws;
        double m_res_time;


};

#endif /* __ARMMANAGER_H */