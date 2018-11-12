#include <Arduino.h>

#include "ArmManager.h"



ArmManager::ArmManager(Picker *arm, workspace_t workspace, double time_resolution)
{
    m_arm       = arm;
    m_ws        = workspace;
    m_res_time  = time_resolution;
}

path_t ArmManager::merge_trajectories(path_t traj_a, path_t traj_b)
{
    path_t new_path;

    // path th1
    new_path.path_th1.t.insert(new_path.path_th1.t.end(), traj_a.path_th1.t.begin(), traj_a.path_th1.t.end());
    new_path.path_th1.pos.insert(new_path.path_th1.pos.end(), traj_a.path_th1.pos.begin(), traj_a.path_th1.pos.end());
    new_path.path_th1.vel.insert(new_path.path_th1.vel.end(), traj_a.path_th1.vel.begin(), traj_a.path_th1.vel.end());
    new_path.path_th1.acc.insert(new_path.path_th1.acc.end(), traj_a.path_th1.acc.begin(), traj_a.path_th1.acc.end());

    new_path.path_th1.t.insert(new_path.path_th1.t.end(), traj_b.path_th1.t.begin(), traj_b.path_th1.t.end());
    new_path.path_th1.pos.insert(new_path.path_th1.pos.end(), traj_b.path_th1.pos.begin(), traj_b.path_th1.pos.end());
    new_path.path_th1.vel.insert(new_path.path_th1.vel.end(), traj_b.path_th1.vel.begin(), traj_b.path_th1.vel.end());
    new_path.path_th1.acc.insert(new_path.path_th1.acc.end(), traj_b.path_th1.acc.begin(), traj_b.path_th1.acc.end());

    // path th2
    new_path.path_th2.t.insert(new_path.path_th2.t.end(), traj_a.path_th2.t.begin(), traj_a.path_th2.t.end());
    new_path.path_th2.pos.insert(new_path.path_th2.pos.end(), traj_a.path_th2.pos.begin(), traj_a.path_th2.pos.end());
    new_path.path_th2.vel.insert(new_path.path_th2.vel.end(), traj_a.path_th2.vel.begin(), traj_a.path_th2.vel.end());
    new_path.path_th2.acc.insert(new_path.path_th2.acc.end(), traj_a.path_th2.acc.begin(), traj_a.path_th2.acc.end());

    new_path.path_th2.t.insert(new_path.path_th2.t.end(), traj_b.path_th2.t.begin(), traj_b.path_th2.t.end());
    new_path.path_th2.pos.insert(new_path.path_th2.pos.end(), traj_b.path_th2.pos.begin(), traj_b.path_th2.pos.end());
    new_path.path_th2.vel.insert(new_path.path_th2.vel.end(), traj_b.path_th2.vel.begin(), traj_b.path_th2.vel.end());
    new_path.path_th2.acc.insert(new_path.path_th2.acc.end(), traj_b.path_th2.acc.begin(), traj_b.path_th2.acc.end());

    // path th3
    new_path.path_th3.t.insert(new_path.path_th3.t.end(), traj_a.path_th3.t.begin(), traj_a.path_th3.t.end());
    new_path.path_th3.pos.insert(new_path.path_th3.pos.end(), traj_a.path_th3.pos.begin(), traj_a.path_th3.pos.end());
    new_path.path_th3.vel.insert(new_path.path_th3.vel.end(), traj_a.path_th3.vel.begin(), traj_a.path_th3.vel.end());
    new_path.path_th3.acc.insert(new_path.path_th3.acc.end(), traj_a.path_th3.acc.begin(), traj_a.path_th3.acc.end());

    new_path.path_th3.t.insert(new_path.path_th3.t.end(), traj_b.path_th3.t.begin(), traj_b.path_th3.t.end());
    new_path.path_th3.pos.insert(new_path.path_th3.pos.end(), traj_b.path_th3.pos.begin(), traj_b.path_th3.pos.end());
    new_path.path_th3.vel.insert(new_path.path_th3.vel.end(), traj_b.path_th3.vel.begin(), traj_b.path_th3.vel.end());
    new_path.path_th3.acc.insert(new_path.path_th3.acc.end(), traj_b.path_th3.acc.begin(), traj_b.path_th1.acc.end());

    return new_path;    
}

workspace_t ArmManager::workspace_containing_position(coords_t position)
{
    if(position_within_workspace(position, m_ws))
    {
        return m_ws;
    }
    else
    {
        m_arm->forward_kinematics({0,0,0});
        return m_ws;
    }    
}

bool ArmManager::workspace_within_constraints(workspace_t workspace)
{
    if ((workspace.x_min < m_arm->x_axis.pos_min) \
            || (workspace.x_max > m_arm->x_axis.pos_max) \
            || (workspace.y_max > m_arm->y_axis.pos_max) \
            || (workspace.y_max > m_arm->y_axis.pos_max) \
            || (workspace.phi_max > m_arm->phi_axis.pos_max) \
            || (workspace.phi_max > m_arm->phi_axis.pos_max))
    {
        return false;
    }
    else
    {
        return true;
    }     
}