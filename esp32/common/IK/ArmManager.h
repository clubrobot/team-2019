#ifndef __ARMMANAGER_H
#define __ARMMANAGER_H

#include <Arduino.h>
#include "Picker.h"
#include "Joint.h"

#define LINK1_LEN 10.0
#define LINK2_LEN 10.0
#define LINK3_LEN 5.0

#define ORIGIN_X 0.0
#define ORIGIN_Y 0.0
#define ORIGIN_PHI 0.0

typedef struct
{
    double x_min;
    double x_max;

    double y_min;
    double y_max;

    double elbow_orientation;
}workspace_t;

class ArmManager
{
    public:

        void        init(workspace_t ws_front, workspace_t ws_back, double time_resolution);
        workspace_t clip_workspace_to_constraints(workspace_t workspace);
        coords_t    workspace_center(workspace_t workspace);
        path_t      go_to(coords_t start_pos, coords_t start_vel, coords_t target_pos, coords_t target_vel);
        path_t      go_home(coords_t start_pos, coords_t start_vel);
        path_t      goto_position(coords_t start_pos, coords_t start_vel, coords_t target_pos, coords_t target_vel);
        double      estimated_time_of_arrival(coords_t start_pos, coords_t start_vel, coords_t target_pos, coords_t target_vel);

        Picker m_arm;
        
    private:

        path_t      merge_trajectories(path_t traj_a, path_t traj_b);
        workspace_t workspace_containing_position(coords_t position);
        bool        workspace_within_constraints(workspace_t workspace);
        bool        position_within_workspace(coords_t position, workspace_t workspace);
        path_t      goto_workspace(coords_t start_pos, coords_t start_vel, coords_t target_pos, coords_t target_vel, workspace_t new_workspace);

        workspace_t m_ws_front;
        workspace_t m_ws_back;
        workspace_t m_workspace;
        double m_dt;
        coords_t m_tool;

};

#endif /* __ARMMANAGER_H */