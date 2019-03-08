#include <Arduino.h>
#include "ArmManager.h"

#ifdef IK_LOG
    #define LOG_ARM(arg) cout << __TIME__<<" (ARM MANAGER)("<< __func__ << " , " << __LINE__ << ")\t\t: "<< arg <<endl;
#else
    #define LOG_ARM(arg) 
#endif

static bool float_equals(float a, float b, float epsilon = 0.001)
{
    return std::abs(a - b) < epsilon;
}

namespace IK
{

void ArmManager::init_workspace(workspace_t ws_front, workspace_t ws_back) throw()
{
    _mutex.acquire();

    _ws_front  = ws_front;
    _ws_back   = ws_back;

    _mutex.release();
}

void ArmManager::set_origin(coords_t origin) throw()
{
    _mutex.acquire();

    _origin.x   = origin.x;
    _origin.y   = origin.y;
    _origin.phi = origin.phi; 
    
    _mutex.release(); 
}
void ArmManager::set_arm_link(float l1, float l2, float l3, int elbow_or)
{
    _mutex.acquire();

    _l1    = l1;
    _l2    = l2;
    _l3    = l3;
    _elbow = elbow_or;

    _mutex.release();
}

void ArmManager::init_arm()
{
    _mutex.acquire();

    Picker::init(_l1, _l2, _l3, _joints, _origin, _elbow);

    _mutex.release();
}

workspace_t ArmManager::workspace_containing_position(coords_t position) throw()
{
    workspace_t ret;

    if(position_within_workspace(position, _ws_front))
    {
        ret = _ws_front;
    }
    else if(position_within_workspace(position, _ws_back))
    {
        ret = _ws_back;  
    }
    else
    {
         
        joints_t joints;
        joints.th1 = 0;
        joints.th2 = 0;
        joints.th3 = 0;
        Picker::forward_kinematics(joints);
        ret = _ws_front;  
    }    
    return ret;
}

bool ArmManager::workspace_within_constraints(workspace_t workspace) throw()
{
    _mutex.acquire();

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
    _mutex.release();
    return ret;
}

workspace_t ArmManager::clip_workspace_to_constraints(workspace_t workspace) throw()
{
    _mutex.acquire();
    workspace_t new_ws;
    new_ws.x_min = max(workspace.x_min, Picker::x_axis.pos_min);
    new_ws.x_max = min(workspace.x_max, Picker::x_axis.pos_max);
        
    new_ws.y_min = max(workspace.y_min, Picker::y_axis.pos_min);
    new_ws.y_max = min(workspace.y_max, Picker::y_axis.pos_max);

    new_ws.elbow_orientation = workspace.elbow_orientation;
    _mutex.release();
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
    _mutex.acquire();
    coords_t coord;
    coord.x = (workspace.x_min + workspace.x_max) / 2;
    coord.y = (workspace.y_min + workspace.y_max) / 2;
    coord.phi = 0;
    _mutex.release();
    return coord;
}

MoveBatch ArmManager::go_to(coords_t start_pos, coords_t target_pos)
{
    _mutex.acquire();
    workspace_t new_ws = workspace_containing_position(target_pos);

    MoveBatch  new_batch;
    joints_t new_joints;
    try
    { 
        new_batch = goto_workspace(start_pos, target_pos, new_ws);
    }
    catch(const string& err)
    {
        _tool = start_pos;
        new_joints = Picker::inverse_kinematics(_tool);
        new_batch.addMove(0, new_joints.th1);
        new_batch.addMove(1, new_joints.th2);
        new_batch.addMove(2, new_joints.th3);
    }

    _mutex.release();
    return new_batch;
}

MoveBatch ArmManager::goto_workspace(coords_t start_pos, coords_t target_pos, workspace_t new_workspace)
{
    joints_t new_joints;
    path_t   new_path;
    MoveBatch new_batch;

    // Check that new position is within workspace
    if (!position_within_workspace(target_pos, new_workspace))
    {
        LOG_ARM("Target position not within new workspace boundaries, target may be out of defined workspaces");
        throw string("Target position not within new workspace boundaries, target may be out of defined workspaces");
    }
     
    _workspace = new_workspace;

    new_batch.addDuration(estimated_time_of_arrival(start_pos, NULL_VEL, target_pos, NULL_VEL));
    
    // Compute sequence to move from old workspace to the new position
    // in the new workspace defined
    if(float_equals(new_workspace.elbow_orientation, Picker::_flip_elbow))
    {
        new_joints = goto_position(target_pos);
        new_path   = get_path(start_pos, NULL_VEL, target_pos, NULL_VEL, DELTA_T);
        LOG_ARM("don't FLIP elbow");
        /* add velocity profile to MoveBatch */
        new_batch.addVelocityProfile(0, new_path.path_th1.vel, new_path.path_th1.t);
        new_batch.addVelocityProfile(1, new_path.path_th2.vel, new_path.path_th2.t);
        new_batch.addVelocityProfile(2, new_path.path_th3.vel, new_path.path_th3.t);
        /* add move position to MoveBatch */
        new_batch.addMove(0, new_joints.th1);
        new_batch.addMove(1, new_joints.th2);
        new_batch.addMove(2, new_joints.th3);
        return new_batch;
    }
    LOG_ARM("FLIP elbow move");

    Picker::_flip_elbow *= (float)-1;
    
    //Go to target
    new_joints = goto_position(target_pos);
    new_path   = get_path(start_pos, NULL_VEL, target_pos, NULL_VEL, DELTA_T);

    /* add velocity profile to MoveBatch */
    new_batch.addVelocityProfile(0, new_path.path_th1.vel, new_path.path_th1.t);
    new_batch.addVelocityProfile(1, new_path.path_th2.vel, new_path.path_th2.t);
    new_batch.addVelocityProfile(2, new_path.path_th3.vel, new_path.path_th3.t);

    /* add move position to MoveBatch */
    new_batch.addMove(0, new_joints.th1);
    new_batch.addMove(1, new_joints.th2);
    new_batch.addMove(2, new_joints.th3);

    new_batch.addDuration(estimated_time_of_arrival(start_pos, NULL_VEL, target_pos, NULL_VEL));
    
    //Return trajectory to execute for adjustment
    return new_batch;
}

joints_t ArmManager::goto_position(coords_t target_pos)
{
    joints_t ret;

    ret = Picker::inverse_kinematics(target_pos);

    return ret;
}

float ArmManager::estimated_time_of_arrival(coords_t start_pos, coords_t start_vel, coords_t target_pos, coords_t target_vel)
{
    float ret;

    joints_t start_joints_pos = Picker::inverse_kinematics(start_pos);
    joints_t start_joints_vel = Picker::get_joints_vel(start_vel);

    joints_t target_joints_pos = Picker::inverse_kinematics(target_pos);
    joints_t target_joints_vel = Picker::get_joints_vel(target_vel);

    ret = Picker::synchronisation_time(start_joints_pos, start_joints_vel, target_joints_pos, target_joints_vel);
    return ret;
}

} /* end of namespace */