#include <Arduino.h>

#include "ArmManager.h"



ArmManager::ArmManager(Picker *arm, workspace_t ws_front, workspace_t ws_back, double time_resolution)
{
    m_arm       = arm;
    m_ws_front  = ws_front;
    m_ws_back   = ws_back;
    m_dt        = time_resolution;
    m_tool      = {0,0,0};
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
    if(position_within_workspace(position, m_ws_back))
    {
        return m_ws_front;
    }
    if(position_within_workspace(position, m_ws_back))
    {
        return m_ws_back;
    }
    else
    {
        m_arm->forward_kinematics({0,0,0});
        return m_ws_front;
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

workspace_t ArmManager::clip_workspace_to_constraints(workspace_t workspace)
{
    double x_min = max(workspace.x_min, m_arm->x_axis.pos_min);
    double x_max = min(workspace.x_max, m_arm->x_axis.pos_max);
        
    double y_min = max(workspace.y_min, m_arm->y_axis.pos_min);
    double y_max = min(workspace.y_max, m_arm->y_axis.pos_max);

    double phi_min = max(workspace.phi_min, m_arm->phi_axis.pos_min);
    double phi_max = min(workspace.phi_max, m_arm->phi_axis.pos_max);

    return (workspace_t){x_min, x_max, y_min, y_max, phi_min, phi_max, workspace.elbow_orientation};
}

bool ArmManager::position_within_workspace(coords_t position, workspace_t workspace)
{
    if ((position.x < workspace.x_min) \
            || (position.x > workspace.x_max) \
            || (position.y < workspace.y_min) \
            || (position.y > workspace.y_max) \
            || (position.phi < workspace.phi_min) \
            || (position.phi > workspace.phi_max))
    {
        return false;
    }
    else
    {
        return true;
    }
}

coords_t ArmManager::workspace_center(workspace_t workspace)
{
    return {(workspace.x_min + workspace.x_max) / 2, (workspace.y_min + workspace.y_max) / 2, -1};
}

path_t ArmManager::go_to(coords_t start_pos, coords_t start_vel, coords_t target_pos, coords_t target_vel)
{
    workspace_t new_ws = workspace_containing_position(target_pos);

    path_t  new_traj;

    bool traj_is_unfeasible = false;

    new_traj = goto_workspace(start_pos, start_vel, target_pos, target_vel, new_ws);
    if(new_traj.feasible != true)
    {
        m_tool = start_pos;
        m_arm->inverse_kinematics(m_tool);
        traj_is_unfeasible = true;
    }

    if(traj_is_unfeasible)
    {
        new_traj = go_home(start_pos, start_vel);
        m_tool = new_traj.pos;
        m_arm->inverse_kinematics(m_tool);
    }
    else
    {
        m_tool = target_pos;
        m_arm->inverse_kinematics(m_tool);
    }
    
    return new_traj;
}

path_t ArmManager::go_home(coords_t start_pos, coords_t start_vel)
{
    //Define home position as target position
    joints_t start_joints_pos = m_arm->inverse_kinematics(start_pos);
    joints_t target_joints_pos = {-M_PI/2, 2*M_PI/3, start_joints_pos.th3};

    coords_t target_pos = m_arm->forward_kinematics(target_joints_pos);
    coords_t target_vel = {0,0,0};

    workspace_t new_ws = workspace_containing_position(target_pos);

    path_t new_traj;
    new_traj = goto_workspace(start_pos, start_vel, target_pos, target_vel, new_ws);
    
    if(new_traj.feasible != true)
    {
        m_tool = start_pos;
        m_arm->inverse_kinematics(m_tool);
        new_traj =  goto_workspace(start_pos, start_vel, target_pos, target_vel, new_ws);
    }

    new_traj.pos = target_pos;
    
    return new_traj;
     
}
path_t ArmManager::goto_workspace(coords_t start_pos, coords_t start_vel, coords_t target_pos, coords_t target_vel, workspace_t new_workspace)
{
    // Check that new position is within workspace
    if (!position_within_workspace(target_pos, new_workspace))
    {
        m_workspace = new_workspace;
    }
    // Compute sequence to move from old workspace to the new position
    // in the new workspace defined
    if(new_workspace.elbow_orientation == m_arm->m_flip_elbow)
    {
        return goto_position(start_pos, start_vel, target_pos, target_vel);
    }

    //Else, we need to flip the elbow!
    joints_t start_joints = m_arm->inverse_kinematics(start_pos);
    joints_t inter_joints = start_joints;
    inter_joints.th2 = 0.0;
        
    coords_t inter_pos = m_arm->forward_kinematics(inter_joints);
    coords_t inter_vel = {0.0, 0.0, 0.0};

    //Go to intermediary point (singularity)
    path_t new_traja;
    new_traja = goto_position(start_pos, start_vel, inter_pos, inter_vel);

    m_arm->m_flip_elbow *= -1;

    //Go to target
    path_t new_trajb;
    new_trajb = goto_position(inter_pos, inter_vel, target_pos, target_vel);

    path_t new_traj;

    new_traj = merge_trajectories(new_traja, new_trajb);

    //Return trajectory to execute for adjustment
    return new_traj;
}

path_t ArmManager::goto_position(coords_t start_pos, coords_t start_vel, coords_t target_pos, coords_t target_vel)
{
    return m_arm->get_path(start_pos, start_vel, target_pos, target_vel, m_dt);
}

double ArmManager::estimated_time_of_arrival(coords_t start_pos, coords_t start_vel, coords_t target_pos, coords_t target_vel)
{
    joints_t start_joints_pos = m_arm->inverse_kinematics(start_pos);
    m_arm->compute_jacobian();
    joints_t start_joints_vel = m_arm->get_joints_vel(start_vel);

    joints_t target_joints_pos = m_arm->inverse_kinematics(target_pos);
    m_arm->compute_jacobian();
    joints_t target_joints_vel = m_arm->get_joints_vel(target_vel);

    return m_arm->synchronisation_time(start_joints_pos, start_joints_vel, target_joints_pos, target_joints_vel);
}