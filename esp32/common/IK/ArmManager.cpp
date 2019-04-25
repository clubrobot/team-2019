#include <Arduino.h>
#include <EEPROM.h>
#include "ArmManager.h"

#ifdef IK_LOG
    #define LOG_ARM(arg) cout << __TIME__<<" (ARM MANAGER)("<< __func__ << " , " << __LINE__ << ")\t\t: "<< arg <<endl;
#else
    #define LOG_ARM(arg) 
#endif

/* null velocity parameter*/
Coords NULL_VEL;

static bool float_equals(float a, float b, float epsilon = 0.001)
{
    return std::abs(a - b) < epsilon;
}

namespace IK
{

void ArmManager::set_workspace(Workspace ws_front, Workspace ws_back) throw()
{
    _mutex.acquire();

    _ws_front  = ws_front;
    _ws_back   = ws_back;

    _mutex.release();
}

void ArmManager::set_origin(Coords origin) throw()
{
    _mutex.acquire();

    _origin  = origin;

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
void ArmManager::set_initial_joint_pos(Joints joints)
{
    _mutex.acquire();

    _joints = joints;
    
    _mutex.release();
}
void ArmManager::init()
{
    _mutex.acquire();

    Picker::init(_l1, _l2, _l3, _joints, _origin, _elbow);

    _mutex.release();
}

Workspace ArmManager::workspace_containing_position(Coords position) throw()
{
    if(position_within_workspace(position, _ws_front))
    {
        return _ws_front;
    }
    else if(position_within_workspace(position, _ws_back))
    {
        return _ws_back;  
    }
    else
    {
        Joints joints;
        Picker::forward_kinematics(joints);
        return _ws_front;  
    }    
    return _ws_front;  
}

bool ArmManager::workspace_within_constraints(Workspace workspace) throw()
{
    _mutex.acquire();
    bool ret;
    if ((workspace.x_min < Picker::x_axis.pos.min) \
            || (workspace.x_max > Picker::x_axis.pos.max) \
            || (workspace.y_min < Picker::y_axis.pos.min) \
            || (workspace.y_max > Picker::y_axis.pos.max))
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

Workspace ArmManager::clip_Workspaceo_constraints(Workspace workspace) throw()
{
    _mutex.acquire();
    Workspace new_ws;
    new_ws.x_min = max(workspace.x_min, Picker::x_axis.pos.min);
    new_ws.x_max = min(workspace.x_max, Picker::x_axis.pos.max);

    new_ws.y_min = max(workspace.y_min, Picker::y_axis.pos.min);
    new_ws.y_max = min(workspace.y_max, Picker::y_axis.pos.max);

    new_ws.elbow_or = workspace.elbow_or;
    _mutex.release();
    return new_ws;
}

bool ArmManager::position_within_workspace(Coords position, Workspace workspace) throw()
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

Coords ArmManager::workspace_center(Workspace workspace) throw()
{
    _mutex.acquire();
    Coords coord;
    coord.x = (workspace.x_min + workspace.x_max) / 2;
    coord.y = (workspace.y_min + workspace.y_max) / 2;
    coord.phi = 0;
    _mutex.release();
    return coord;
}

MoveBatch ArmManager::go_to(Coords start_pos, Coords target_pos)
{
    _mutex.acquire();
    Workspace new_ws = workspace_containing_position(target_pos);

    MoveBatch  new_batch;
    Joints new_joints;
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

MoveBatch ArmManager::goto_workspace(Coords start_pos, Coords target_pos, Workspace new_workspace)
{
    Joints new_joints;
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
    if(float_equals(new_workspace.elbow_or, Picker::_flip_elbow))
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

Joints ArmManager::goto_position(Coords target_pos)
{
    Joints ret;

    ret = Picker::inverse_kinematics(target_pos);

    return ret;
}

float ArmManager::estimated_time_of_arrival(Coords start_pos, Coords start_vel, Coords target_pos, Coords target_vel)
{
    float ret;

    Joints start_joints_pos = Picker::inverse_kinematics(start_pos);
    Joints start_joints_vel = Picker::get_joints_vel(start_vel);

    Joints target_joints_pos = Picker::inverse_kinematics(target_pos);
    Joints target_joints_vel = Picker::get_joints_vel(target_vel);

    ret = Picker::synchronisation_time(start_joints_pos, start_joints_vel, target_joints_pos, target_joints_vel);
    return ret;
}

void ArmManager::load(int address)
{
	_mutex.acquire();
	EEPROM.get(address, _ws_front);     address += sizeof(_ws_front);
	EEPROM.get(address, _ws_back);      address += sizeof(_ws_back);
    EEPROM.get(address, _origin);       address += sizeof(_origin);
    EEPROM.get(address, _joints);       address += sizeof(_joints);
    EEPROM.get(address, _l1);           address += sizeof(_l1);
    EEPROM.get(address, _l2);           address += sizeof(_l2);
    EEPROM.get(address, _l3);           address += sizeof(_l3);
    EEPROM.get(address, _elbow);        address += sizeof(_elbow);
	_mutex.release();
}

void ArmManager::save(int address) const
{
	_mutex.acquire();
	EEPROM.put(address, _ws_front);     address += sizeof(_ws_front);
	EEPROM.put(address, _ws_back);      address += sizeof(_ws_back);
    EEPROM.put(address, _origin);       address += sizeof(_origin);
    EEPROM.put(address, _joints);       address += sizeof(_joints);
    EEPROM.put(address, _l1);           address += sizeof(_l1);
    EEPROM.put(address, _l2);           address += sizeof(_l2);
    EEPROM.put(address, _l3);           address += sizeof(_l3);
    EEPROM.put(address, _elbow);        address += sizeof(_elbow);
	EEPROM.commit();
	_mutex.release();
}

} /* end of namespace */