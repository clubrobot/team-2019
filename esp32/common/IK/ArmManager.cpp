#include <Arduino.h>
#include "ArmManager.h"

static bool double_equals(double a, double b, double epsilon = 0.001)
{
    return std::abs(a - b) < epsilon;
}

namespace IK
{

ArmManager::ArmManager(double dt) throw()
{
     

    m_dt = dt;

     
}
void ArmManager::init_workspace(workspace_t ws_front, workspace_t ws_back) throw()
{
     

    m_ws_front  = ws_front;
    m_ws_back   = ws_back;

     
}

void ArmManager::set_origin(coords_t origin) throw()
{
     

    m_origin.x   = origin.x;
    m_origin.y   = origin.y;
    m_origin.phi = origin.phi;

     
}

void ArmManager::attach(int id_1, int id_2, int id_3, double l1, double l2, double l3) throw()
{
     

    m_id1 = id_1;
    m_id2 = id_2;
    m_id3 = id_3;

    m_len1 = l1;
    m_len2 = l2;
    m_len3 = l3;

     
}

void ArmManager::init_arm(coords_t initial_pos, int elbow_or)
{
     
    
    MotorWrapper::attach(m_id1, m_id2, m_id3);
    MotorWrapper::init();
    MotorWrapper::init_offsets(LINK1_OFFSET, LINK2_OFFSET, LINK3_OFFSET);

    Picker::init(m_len1, m_len2, m_len3, m_joints, m_origin, elbow_or);

    m_tool.x    = initial_pos.x;
    m_tool.y    = initial_pos.y;
    m_tool.phi  = initial_pos.phi;

    m_joints = Picker::inverse_kinematics(m_tool);
    
    MotorWrapper::move(CONVERT_DEG(m_joints.th1), CONVERT_DEG(m_joints.th2), CONVERT_DEG(m_joints.th3));

     
}

path_t ArmManager::merge_trajectories(path_t traj_a, path_t traj_b) throw()
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
    new_path.path_th3.acc.insert(new_path.path_th3.acc.end(), traj_b.path_th3.acc.begin(), traj_b.path_th3.acc.end());

     

    return new_path;    
}

workspace_t ArmManager::workspace_containing_position(coords_t position) throw()
{
    workspace_t ret;

    if(position_within_workspace(position, m_ws_front))
    {
        ret = m_ws_front;
    }
    else if(position_within_workspace(position, m_ws_back))
    {
        ret = m_ws_back;  
    }
    else
    {
         
        joints_t joints;
        joints.th1 = 0;
        joints.th2 = 0;
        joints.th3 = 0;
        Picker::forward_kinematics(joints);
        ret = m_ws_front;  
    }    
    return ret;
}

bool ArmManager::workspace_within_constraints(workspace_t workspace) throw()
{
     

    bool ret;

    if ((workspace.x_min < Picker::x_axis.pos_min) \
            || (workspace.x_max > Picker::x_axis.pos_max) \
            || (workspace.y_min < Picker::y_axis.pos_min) \
            || (workspace.y_max > Picker::y_axis.pos_max))
    {
        ret = false;
    }
    else
    {
        ret = true;
    }     

     

    return ret;
}

workspace_t ArmManager::clip_workspace_to_constraints(workspace_t workspace) throw()
{
     

    workspace_t new_ws;
    new_ws.x_min = max(workspace.x_min, Picker::x_axis.pos_min);
    new_ws.x_max = min(workspace.x_max, Picker::x_axis.pos_max);
        
    new_ws.y_min = max(workspace.y_min, Picker::y_axis.pos_min);
    new_ws.y_max = min(workspace.y_max, Picker::y_axis.pos_max);

    new_ws.elbow_orientation = workspace.elbow_orientation;

     

    return new_ws;
}

bool ArmManager::position_within_workspace(coords_t position, workspace_t workspace) throw()
{
    bool ret;

    if ((position.x < workspace.x_min) || (position.x > workspace.x_max) \
    || (position.y < workspace.y_min) || (position.y > workspace.y_max))
    {
        ret = false;
    }
    else
    {
        ret = true;
    }

    return ret;
}

coords_t ArmManager::workspace_center(workspace_t workspace) throw()
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

    try
    { 
        new_traj = goto_workspace(start_pos, start_vel, target_pos, target_vel, new_ws);
    }
    catch(const string& err)
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
    joints_t target_joints_pos = {10, 5, 0};

    coords_t target_pos = Picker::forward_kinematics(target_joints_pos);
    coords_t target_vel;
    target_vel.x    = 0;
    target_vel.y    = 0;
    target_vel.phi  = 0;

     

    workspace_t new_ws = workspace_containing_position(target_pos);

    path_t new_traj;
    new_traj = goto_workspace(start_pos, start_vel, target_pos, target_vel, new_ws);

     

    new_traj.pos = target_pos;

     
    
    return new_traj;
     
}
path_t ArmManager::goto_workspace(coords_t start_pos, coords_t start_vel, coords_t target_pos, coords_t target_vel, workspace_t new_workspace)
{
    path_t new_traj;
    // Check that new position is within workspace
    if (!position_within_workspace(target_pos, new_workspace))
    {
        throw string("Target position not within new workspace boundaries, target may be out of defined workspaces");
    }
     
    m_workspace = new_workspace;
    
    // Compute sequence to move from old workspace to the new position
    // in the new workspace defined
    if(double_equals(new_workspace.elbow_orientation,Picker::m_flip_elbow))
    {
        new_traj = goto_position(start_pos, start_vel, target_pos, target_vel);
        return new_traj;
    }

    //Else, we need to flip the elbow!
    joints_t start_joints = Picker::inverse_kinematics(start_pos);
    joints_t inter_joints = start_joints;
    inter_joints.th1 = M_PI/2;
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
     
    path_t ret;

    ret = Picker::get_path(start_pos, start_vel, target_pos, target_vel, m_dt);

    return ret;
}

double ArmManager::estimated_time_of_arrival(coords_t start_pos, coords_t start_vel, coords_t target_pos, coords_t target_vel)
{
     

    double ret;

    joints_t start_joints_pos = Picker::inverse_kinematics(start_pos);
    joints_t start_joints_vel = Picker::get_joints_vel(start_vel);

    joints_t target_joints_pos = Picker::inverse_kinematics(target_pos);
    joints_t target_joints_vel = Picker::get_joints_vel(target_vel);

    ret = Picker::synchronisation_time(start_joints_pos, start_joints_vel, target_joints_pos, target_joints_vel);

     

    return ret;
}

} /* end of namespace */