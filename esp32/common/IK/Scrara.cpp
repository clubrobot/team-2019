#include <Arduino.h>
#include <math.h>
#include <iostream>
#include <string>
#include <stdexcept>
#include "Joint.h"
#include "Scara.h"
#include "Matrix.h"
#include "constants.h"

namespace IK
{

Scara::Scara(float l1, float l2, Joints joints, Coords origin)
{
	_joints 	= joints;
	_origin	= origin;
	_tool	 	= get_tool();

	_l1		= l1;
	_l2		= l2;
	_lsq   	= pow(l1,2)+pow(l2,2);
}

Coords Scara::forward_kinematics(Joints joints)
{
	_joints = joints;
	return get_tool();
}

Joints Scara::inverse_kinematics(Coords tool)
{

	float norm = pow((tool.x - _origin.x),2) + pow((tool.y - _origin.y),2);

    if( norm > pow((_l1 + _l2),2) || norm < pow((_l1 - _l2),2))
    {
    	//Target unreachable
        _tool = get_tool();
        //return ;
    }
            

    _tool = tool;
    _joints = get_joints();

    return _joints;
}

Coords Scara::get_tool(void)
{
    Coords new_cords;

    new_cords.x = (_l1 * cos(_joints.th1) + _l2 * cos(_joints.th1 + _joints.th2));
    new_cords.y = _l1 * sin(_joints.th1) + _l2 * sin(_joints.th2 + _joints.th1);

    new_cords.x += _origin.x;
    new_cords.y += _origin.y;

    return new_cords;
}

Joints Scara::get_joints(void)
{
    Joints new_joints;

	float x = _tool.x - _origin.x;
    float y = _tool.y - _origin.y;

    if(x == 0 and y == 0)
    {
        new_joints.th2 = M_PI;
    	return new_joints;
    }

    float l = pow(x,2) + pow(y,2);
    
   	float lsq = _lsq;

    float cos_gamma = (l + pow(_l1,2) - pow(_l2,2)) / (2 * _l1 * sqrt(l));

    float gamma;
    //Numerical errors can make abs(cos_gamma) > 1
    if(cos_gamma > 1 - EPSILON || cos_gamma < (-1 + EPSILON))
    {
        gamma = 0.0;
    }
    else
    {
        gamma = acos(cos_gamma);
    }

    new_joints.th1 = atan2(y, x) - gamma;
    new_joints.th2 = atan2(sqrt(1 - pow(((l - lsq) / (2 * _l1 * _l2)),2)), (l - lsq) / (2 * _l1 * _l2));


    new_joints.th1 = std::fmod((new_joints.th1 + M_PI) , (2 * M_PI)) - M_PI; // Stay between -pi and pi
    new_joints.th2 = std::fmod((new_joints.th2 + M_PI) , (2 * M_PI)) - M_PI; // Stay between -pi and pi

    return new_joints;
}

DetailedPos Scara::get_detailed_pos(void)
{
    /*
        Returns origin, position of end of link 1, position of end of link 2
    */
    DetailedPos new_pos;

    new_pos.link1.x = _l1 * cos(_joints.th1) + _origin.x;
    new_pos.link1.y = _l1 * sin(_joints.th1) + _origin.y;

    new_pos.origin  = _origin;
    new_pos.tool    = _tool;

    return new_pos;
}

matrix_t Scara::compute_jacobian(void)
{
    /*
        Returns jacobian matrix at current state
    */

    float dx_dth1 = - _l1 * sin(_joints.th1) - _l2 * sin(_joints.th1 + _joints.th2);

    float dx_dth2 = - _l2 * sin(_joints.th1 + _joints.th2);

    float dy_dth1 = _l1 * cos(_joints.th1) + _l2 * cos(_joints.th1 + _joints.th2);

    float dy_dth2 = _l2 * cos(_joints.th1 + _joints.th2);

    return _matrix.createMatrix22(dx_dth1, dx_dth2, dy_dth1, dy_dth2);
}

Coords Scara::get_tool_vel(Joints joints_vel)
{
    /*
        Computes current tool velocity using jacobian
    */
    Coords new_vel;

    matrix_t jt_vel = _matrix.createMatrix21(joints_vel.th1, joints_vel.th1);

    matrix_t jacobian = compute_jacobian();

    matrix_t tl_vel = _matrix.multMatrix22x12(jacobian , jt_vel);

    new_vel.x = tl_vel[0][0];
    new_vel.y = tl_vel[1][0];

    return new_vel;
}

Joints Scara::get_joints_vel(Coords tool_vel)
{
    /*
        Computes current tool velocity using jacobian
    */
    Joints vel;
    vel.th1 = 0;
    vel.th2 = 0;

    matrix_t tool_v = _matrix.createMatrix21(tool_vel.x, tool_vel.y);

    matrix_t jacobian = compute_jacobian();

    if (_matrix.norm(tool_v) < EPSILON)
        return vel;

    if (std::abs(_matrix.det(jacobian)) < EPSILON)
    {
        //raise ValueError('Singularity')
    }

    matrix_t joints_vel = _matrix.solve(jacobian, tool_v);

    vel.th1 = joints_vel[0][0];
    vel.th2 = joints_vel[1][0];

    return vel;
}

path_t Scara::get_path(Coords start_pos, Coords start_vel, Coords target_pos, Coords target_vel, float delta_t)
{

    Joints start_joints_pos = inverse_kinematics(start_pos);
        
    Joints start_joints_vel = get_joints_vel(start_vel);

    Joints target_joints_pos = inverse_kinematics(target_pos);
    Joints target_joints_vel = get_joints_vel(target_vel);

        
    float tf_sync = synchronisation_time(start_joints_pos,
                                            start_joints_vel,
                                            target_joints_pos,
                                            target_joints_vel);

    //get_path
    path_t new_path;

    new_path.path_th1 = Theta1_joint.get_path(start_joints_pos.th1,
                                                start_joints_vel.th1,
                                                target_joints_pos.th1,
                                                target_joints_vel.th1,
                                                tf_sync,
                                                delta_t);

    new_path.path_th2 = Theta2_joint.get_path(start_joints_pos.th2,
                                                start_joints_vel.th2,
                                                target_joints_pos.th2,
                                                target_joints_vel.th2,
                                                tf_sync,
                                                delta_t);

    return new_path;
}

float Scara::synchronisation_time(Joints start_pos, Joints start_vel, Joints target_pos, Joints target_vel)
{
    /*        
    Return largest time to destination to use slowest joint as synchronisation
        reference
    */

    // Compute time to destination for all joints
    TrajectoryTime t_theta1 = Theta1_joint.time_to_destination(start_pos.th1, start_vel.th1, target_pos.th1, target_vel.th1);

    TrajectoryTime t_theta2 = Theta2_joint.time_to_destination(start_pos.th2, start_vel.th2, target_pos.th2, target_vel.th2);
    
    return max(t_theta1.tf, t_theta2.tf);
}

} /* end of namespace */