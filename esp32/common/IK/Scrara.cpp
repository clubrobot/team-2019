#include <Arduino.h>
#include <math.h>
#include <iostream>
#include <string>
#include <stdexcept>
#include "Joint.h"
#include "Scara.h"
#include "Matrix.h"
#include "arm_config.h"

namespace IK
{

Scara::Scara(double l1, double l2, joints_t joints, coords_t origin)
{
	m_joints 	= joints;
	m_origin	= origin;
	m_tool	 	= get_tool();

	m_l1		= l1;
	m_l2		= l2;
	m_lsq   	= pow(l1,2)+pow(l2,2);
}

coords_t Scara::forward_kinematics(joints_t joints)
{
	m_joints = joints;
	return get_tool();
}

joints_t Scara::inverse_kinematics(coords_t tool)
{

	double norm = pow((tool.x - m_origin.x),2) + pow((tool.y - m_origin.y),2);

    if( norm > pow((m_l1 + m_l2),2) || norm < pow((m_l1 - m_l2),2))
    {
    	//Target unreachable
        m_tool = get_tool();
        //return ;
    }
            

    m_tool = tool;
    m_joints = get_joints();

    return m_joints;
}

coords_t Scara::get_tool(void)
{
    coords_t new_cords;

    new_cords.x = (m_l1 * cos(m_joints.th1) + m_l2 * cos(m_joints.th1 + m_joints.th2));
    new_cords.y = m_l1 * sin(m_joints.th1) + m_l2 * sin(m_joints.th2 + m_joints.th1);

    new_cords.x += m_origin.x;
    new_cords.y += m_origin.y;

    return new_cords;
}

joints_t Scara::get_joints(void)
{
    joints_t new_joints;

	double x = m_tool.x - m_origin.x;
    double y = m_tool.y - m_origin.y;

    if(x == 0 and y == 0)
    {
        new_joints.th2 = M_PI;
    	return new_joints;
    }

    double l = pow(x,2) + pow(y,2);
    
   	double lsq = m_lsq;

    double cos_gamma = (l + pow(m_l1,2) - pow(m_l2,2)) / (2 * m_l1 * sqrt(l));

    double gamma;
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
    new_joints.th2 = atan2(sqrt(1 - pow(((l - lsq) / (2 * m_l1 * m_l2)),2)), (l - lsq) / (2 * m_l1 * m_l2));


    new_joints.th1 = std::fmod((new_joints.th1 + M_PI) , (2 * M_PI)) - M_PI; // Stay between -pi and pi
    new_joints.th2 = std::fmod((new_joints.th2 + M_PI) , (2 * M_PI)) - M_PI; // Stay between -pi and pi

    return new_joints;
}

detailed_pos_t Scara::get_detailed_pos(void)
{
    /*
        Returns origin, position of end of link 1, position of end of link 2
    */
    detailed_pos_t new_pos;

    new_pos.link1.x = m_l1 * cos(m_joints.th1) + m_origin.x;
    new_pos.link1.y = m_l1 * sin(m_joints.th1) + m_origin.y;

    new_pos.origin  = m_origin;
    new_pos.tool    = m_tool;

    return new_pos;
}

matrix_t Scara::compute_jacobian(void)
{
    /*
        Returns jacobian matrix at current state
    */

    double dx_dth1 = - m_l1 * sin(m_joints.th1) - m_l2 * sin(m_joints.th1 + m_joints.th2);

    double dx_dth2 = - m_l2 * sin(m_joints.th1 + m_joints.th2);

    double dy_dth1 = m_l1 * cos(m_joints.th1) + m_l2 * cos(m_joints.th1 + m_joints.th2);

    double dy_dth2 = m_l2 * cos(m_joints.th1 + m_joints.th2);

    return m_matrix.createMatrix22(dx_dth1, dx_dth2, dy_dth1, dy_dth2);
}

coords_t Scara::get_tool_vel(joints_t joints_vel)
{
    /*
        Computes current tool velocity using jacobian
    */
    coords_t new_vel;

    matrix_t jt_vel = m_matrix.createMatrix21(joints_vel.th1, joints_vel.th1);

    matrix_t jacobian = compute_jacobian();

    matrix_t tl_vel = m_matrix.multMatrix22x12(jacobian , jt_vel);

    new_vel.x = tl_vel[0][0];
    new_vel.y = tl_vel[1][0];

    return new_vel;
}

joints_t Scara::get_joints_vel(coords_t tool_vel)
{
    /*
        Computes current tool velocity using jacobian
    */
    joints_t vel;
    vel.th1 = 0;
    vel.th2 = 0;

    matrix_t tool_v = m_matrix.createMatrix21(tool_vel.x, tool_vel.y);

    matrix_t jacobian = compute_jacobian();

    if (m_matrix.norm(tool_v) < EPSILON)
        return vel;

    if (abs(m_matrix.det(jacobian)) < EPSILON)
    {
        //raise ValueError('Singularity')
    }

    matrix_t joints_vel = m_matrix.solve(jacobian, tool_v);

    vel.th1 = joints_vel[0][0];
    vel.th2 = joints_vel[1][0];

    return vel;
}

path_t Scara::get_path(coords_t start_pos, coords_t start_vel, coords_t target_pos, coords_t target_vel, double delta_t)
{

    joints_t start_joints_pos = inverse_kinematics(start_pos);
        
    joints_t start_joints_vel = get_joints_vel(start_vel);

    joints_t target_joints_pos = inverse_kinematics(target_pos);
    joints_t target_joints_vel = get_joints_vel(target_vel);

        
    double tf_sync = synchronisation_time(start_joints_pos,
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

double Scara::synchronisation_time(joints_t start_pos, joints_t start_vel, joints_t target_pos, joints_t target_vel)
{
    /*        
    Return largest time to destination to use slowest joint as synchronisation
        reference
    */

    // Compute time to destination for all joints
    trajectory_time_t t_theta1 = Theta1_joint.time_to_destination(start_pos.th1, start_vel.th1, target_pos.th1, target_vel.th1);

    trajectory_time_t t_theta2 = Theta2_joint.time_to_destination(start_pos.th2, start_vel.th2, target_pos.th2, target_vel.th2);
    
    return max(t_theta1.tf, t_theta2.tf);
}

} /* end of namespace */