#include <Arduino.h>
#include "ArmManager.h"

static bool double_equals(double a, double b, double epsilon = 0.001)
{
    return std::abs(a - b) < epsilon;
}

ArmManager::ArmManager(double dt)
{
    m_dt = dt;
}
void ArmManager::init_workspace(workspace_t ws_front, workspace_t ws_back)
{
    m_ws_front  = ws_front;
    m_ws_back   = ws_back;
}

void ArmManager::set_origin(double x, double y, double phi)
{
    m_origin.x   = x;
    m_origin.y   = y;
    m_origin.phi = phi;
}

void ArmManager::attach(int id_1, int id_2, int id_3, double l1, double l2, double l3)
{
    m_id1 = id_1;
    m_id2 = id_2;
    m_id3 = id_3;

    m_len1 = l1;
    m_len2 = l2;
    m_len3 = l3;
}

void ArmManager::init_arm(double x, double y, double phi, int elbow_or)
{
    AX12::SerialBegin(9600, 2);
    
    m_AX1.attach(m_id1);
    m_AX2.attach(m_id2);
    m_AX3.attach(m_id3);

    m_AX1.setLEDAlarm(32); // max torque only
    m_AX2.setLEDAlarm(32); // max torque only
    m_AX3.setLEDAlarm(32); // max torque only

    m_AX1.setShutdownAlarm(32); // max torque only
    m_AX2.setShutdownAlarm(32); // max torque only
    m_AX3.setShutdownAlarm(32); // max torque only

    m_AX1.setMaxTorque(1023);
    m_AX2.setMaxTorque(1023);
    m_AX3.setMaxTorque(1023);

    m_AX1.setEndlessMode(OFF);
    m_AX2.setEndlessMode(OFF);
    m_AX3.setEndlessMode(OFF);

    m_AX1.hold(OFF);
    m_AX2.hold(OFF);
    m_AX3.hold(OFF);

    Picker::init(m_len1, m_len2, m_len3, m_joints, m_origin, elbow_or);

    m_tool.x    = x;
    m_tool.y    = y;
    m_tool.phi  = phi;

    m_joints = Picker::inverse_kinematics(m_tool);

    m_AX1.move(AX12_COORDS(m_joints.th1));
    m_AX2.move(AX12_COORDS(m_joints.th2));
    m_AX3.move(AX12_COORDS(m_joints.th3));
}

bool ArmManager::kinematics_error()
{
    arm_error_t m_kinematics_errors = Picker::get_error();

    if(m_kinematics_errors.th1_error != 0)
    {
        return true;
    }
    else if(m_kinematics_errors.th2_error != 0)
    {
        return true;
    }
    else if(m_kinematics_errors.th3_error != 0)
    {
        return true;
    }
    return false;
}

bool ArmManager::motors_error()
{

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
    if(position_within_workspace(position, m_ws_front))
    {
        return m_ws_back;
    }
    else
    {
        joints_t joints;
        joints.th1 = 0;
        joints.th2 = 0;
        joints.th3 = 0;
        Picker::forward_kinematics(joints);
        return m_ws_front;
    }    
}

bool ArmManager::workspace_within_constraints(workspace_t workspace)
{
    if ((workspace.x_min < Picker::x_axis.pos_min) \
            || (workspace.x_max > Picker::x_axis.pos_max) \
            || (workspace.y_min < Picker::y_axis.pos_min) \
            || (workspace.y_max > Picker::y_axis.pos_max))
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
    workspace_t new_ws;
    new_ws.x_min = max(workspace.x_min, Picker::x_axis.pos_min);
    new_ws.x_max = min(workspace.x_max, Picker::x_axis.pos_max);
        
    new_ws.y_min = max(workspace.y_min, Picker::y_axis.pos_min);
    new_ws.y_max = min(workspace.y_max, Picker::y_axis.pos_max);

    new_ws.elbow_orientation = workspace.elbow_orientation;

    return new_ws;
}

bool ArmManager::position_within_workspace(coords_t position, workspace_t workspace)
{
    if ((position.x < workspace.x_min) \
            || (position.x > workspace.x_max) \
            || (position.y < workspace.y_min) \
            || (position.y > workspace.y_max))
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
    coords_t coord;
    coord.x = (workspace.x_min + workspace.x_max) / 2;
    coord.y = (workspace.y_min + workspace.y_max) / 2;
    coord.phi = 0;
    return coord;
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
        Picker::inverse_kinematics(m_tool);
        traj_is_unfeasible = true;
    }

    if(traj_is_unfeasible)
    {
        new_traj = go_home(start_pos, start_vel);
        m_tool = new_traj.pos;
        Picker::inverse_kinematics(m_tool);
    }
    else
    {
       m_tool = target_pos;
       Picker::inverse_kinematics(m_tool);
    }
    
    return new_traj;
}

path_t ArmManager::go_home(coords_t start_pos, coords_t start_vel)
{
    //Define home position as target position
    joints_t start_joints_pos = Picker::inverse_kinematics(start_pos);
    joints_t target_joints_pos = {5, 5, 0};

    coords_t target_pos = Picker::forward_kinematics(target_joints_pos);
    coords_t target_vel;
    target_vel.x    = 0;
    target_vel.y    = 0;
    target_vel.phi  = 0;

    workspace_t new_ws = workspace_containing_position(target_pos);

    path_t new_traj;
    new_traj = goto_workspace(start_pos, start_vel, target_pos, target_vel, new_ws);
    
    if(new_traj.feasible != true)
    {
        m_tool = start_pos;
        Picker::inverse_kinematics(m_tool);
        new_traj =  goto_workspace(start_pos, start_vel, target_pos, target_vel, new_ws);
    }

    new_traj.pos = target_pos;
    
    return new_traj;
     
}
path_t ArmManager::goto_workspace(coords_t start_pos, coords_t start_vel, coords_t target_pos, coords_t target_vel, workspace_t new_workspace)
{
    path_t new_traj;
    // Check that new position is within workspace
    if (!position_within_workspace(target_pos, new_workspace))
    {
        //error
    }

    m_workspace = new_workspace;

    new_traj.feasible = true;
    // Compute sequence to move from old workspace to the new position
    // in the new workspace defined
    if(double_equals(new_workspace.elbow_orientation,Picker::m_flip_elbow))
    {
        new_traj = goto_position(start_pos, start_vel, target_pos, target_vel);
        new_traj.feasible = true;
        return new_traj;
    }
    //Else, we need to flip the elbow!
    joints_t start_joints = Picker::inverse_kinematics(start_pos);
    joints_t inter_joints = start_joints;
    inter_joints.th2 = 0.0;
        
    coords_t inter_pos = Picker::forward_kinematics(inter_joints);
    coords_t inter_vel;
    inter_vel.x     = 0;
    inter_vel.y     = 0;
    inter_vel.phi   = 0;

    //Go to intermediary point (singularity)
    path_t new_traja;
    new_traja = goto_position(start_pos, start_vel, inter_pos, inter_vel);

    Picker::m_flip_elbow *= (double)-1;

    //Go to target
    path_t new_trajb;
    new_trajb = goto_position(inter_pos, inter_vel, target_pos, target_vel);

    new_traj = merge_trajectories(new_traja, new_trajb);

    //Return trajectory to execute for adjustment
    return new_traj;
}

path_t ArmManager::goto_position(coords_t start_pos, coords_t start_vel, coords_t target_pos, coords_t target_vel)
{
    return Picker::get_path(start_pos, start_vel, target_pos, target_vel, m_dt);
}

double ArmManager::estimated_time_of_arrival(coords_t start_pos, coords_t start_vel, coords_t target_pos, coords_t target_vel)
{
    joints_t start_joints_pos = Picker::inverse_kinematics(start_pos);
    Picker::compute_jacobian();
    joints_t start_joints_vel = Picker::get_joints_vel(start_vel);

    joints_t target_joints_pos = Picker::inverse_kinematics(target_pos);
    Picker::compute_jacobian();
    joints_t target_joints_vel = Picker::get_joints_vel(target_vel);

    return Picker::synchronisation_time(start_joints_pos, start_joints_vel, target_joints_pos, target_joints_vel);
}