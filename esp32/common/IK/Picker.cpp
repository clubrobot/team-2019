#include <Arduino.h>
#include <math.h>
#include "Picker.h"
#include "arm_config.h"

#ifdef IK_LOG
    #define LOG_PICKER(arg) cout << __TIME__<<" (PICKER)("<< __func__ << " , " << __LINE__ << ")\t\t\t: "<< arg <<endl;
#else
    #define LOG_PICKER(arg) 
#endif

namespace IK
{

bool equals(double a, double b, double epsilon = EPSILON)
{ 
    return ((std::abs(a - b) < epsilon));
}


void Picker::init(double l1, double l2, double l3, joints_t joints, coords_t origin, int elbow_or) throw()
{
    m_joints 	= joints;
	m_origin	= origin;
     
	m_tool	 	= get_tool();

	m_l1		= l1;
	m_l2		= l2;
    m_l3        = l3;
	m_lsq   	= pow(l1,2)+pow(l2,2);

    x_axis   = {-(m_l1 + m_l2 + m_l3), (m_l1 + m_l2 + m_l3), -1, 1, -1, 1};
	y_axis   = {-(m_l1 + m_l2 + m_l3), (m_l1 + m_l2 + m_l3), -1, 1, -1, 1};
	phi_axis = {-(5*M_PI)/6, (5*M_PI)/6 , -1, 1, -1, 1};

    m_flip_elbow = elbow_or;
}

void Picker::flip_elbow(int elbow) throw()
{
    m_flip_elbow = elbow;
}

coords_t Picker::forward_kinematics(joints_t joints) throw()
{
    coords_t ret;
	m_joints = joints;

	ret = get_tool();

    return ret;
}

joints_t Picker::inverse_kinematics(coords_t tool)
{
    joints_t ret;

    double dotx = (tool.x - m_origin.x) - (m_l3 * cos(tool.phi));
    double doty = (tool.y - m_origin.y) - (m_l3 * sin(tool.phi));

	double norm = pow(dotx,2) + pow(doty,2);
    
    if( norm > pow((m_l1 + m_l2 + m_l3),2) || norm < pow((m_l1 - m_l2 - m_l3),2))
    {
        m_tool = get_tool();
        LOG_PICKER("Target unreacheable");
        throw string("Target unreacheable");
    }
    else
    {
        m_tool = tool;
         
        m_joints = get_joints();
         
        ret = m_joints;
    }      
    return ret;
}

coords_t Picker::get_tool(void) const throw()
{
    coords_t new_cords;

    new_cords.x     = m_l1 * cos(m_joints.th1) + m_l2 * cos(m_joints.th1 + m_joints.th2) + m_l3 * cos(m_joints.th1 + m_joints.th2 + m_joints.th3);
    new_cords.y     = m_l1 * sin(m_joints.th1) + m_l2 * sin(m_joints.th1 + m_joints.th2) + m_l3 * sin(m_joints.th1 + m_joints.th2 + m_joints.th3);
    new_cords.phi   = m_joints.th1 + m_joints.th2 + m_joints.th3;

    new_cords.x     += m_origin.x;
    new_cords.y     += m_origin.y;
    new_cords.phi   += m_origin.phi;
     
    return new_cords;
}

joints_t Picker::get_joints(void) const throw()
{
    joints_t new_joints;

    double dotx,doty,costh,sinth,k1,k2, sqr;

    dotx = (m_tool.x - m_origin.x) - (m_l3 * cos(m_tool.phi));
    doty = (m_tool.y - m_origin.y) - (m_l3 * sin(m_tool.phi));

    costh = (pow(dotx,2) + pow(doty,2) - pow(m_l1,2) - pow(m_l2, 2)) / (2 * m_l1 * m_l2);

    if(costh > 1)
    {
        LOG_PICKER("ERROR : NOT A NUMBER");
        throw string("Not A number");
    }

    sinth = m_flip_elbow * sqrt(1 - pow(costh,2));

    k1 = m_l1 + (m_l2 * costh);
    k2 = m_l2 * sinth ;

    new_joints.th1 = atan2(doty, dotx) - atan2(k2 , k1);
    new_joints.th2 = atan2(sinth, costh);
    new_joints.th3 = m_tool.phi - (new_joints.th1 + new_joints.th2);

    new_joints.th1 = std::fmod((new_joints.th1 + (M_PI)) , 2*M_PI) - M_PI; // Stay between -pi and pi
    new_joints.th2 = std::fmod((new_joints.th2 + (M_PI)) , 2*M_PI) - M_PI; // Stay between -pi and pi
    new_joints.th3 = std::fmod((new_joints.th3 + (M_PI)) , 2*M_PI) - M_PI; // Stay between -pi and pi

    return new_joints;
}

detailed_pos_t Picker::get_detailed_pos(void) const throw()
{
    /*
        Returns origin, position of end of link 1, position of end of link 2
    */ 
    detailed_pos_t new_pos;

    new_pos.link1.x = m_l1 * cos(m_joints.th1) + m_origin.x;
    new_pos.link1.y = m_l1 * sin(m_joints.th1) + m_origin.y;
    
    new_pos.link2.x = new_pos.link1.x + m_l2 * cos(m_joints.th1 + m_joints.th2);
    new_pos.link2.y = new_pos.link1.x + m_l2 * cos(m_joints.th1 + m_joints.th2);

    new_pos.origin  = m_origin;
    new_pos.tool    = m_tool;
     
    return new_pos;
}

matrix_t Picker::compute_jacobian(void) throw()
{
    /*
        Returns jacobian matrix at current state
    */
    matrix_t ret;

    double dx_dth1 = - m_l1 * sin(m_joints.th1) - m_l2 * sin(m_joints.th1 + m_joints.th2) - m_l3 * sin(m_joints.th1 + m_joints.th2 + m_joints.th3);

    double dx_dth2 = - m_l2 * sin(m_joints.th1 + m_joints.th2) - m_l3 * sin(m_joints.th1 + m_joints.th2 + m_joints.th3);

    double dx_dth3 = - m_l3 * sin(m_joints.th1 + m_joints.th2 + m_joints.th3);;

    double dy_dth1 = m_l1 * cos(m_joints.th1) + m_l2 * cos(m_joints.th1 + m_joints.th2) + m_l3 * cos(m_joints.th1 + m_joints.th2 + m_joints.th3);

    double dy_dth2 = m_l2 * cos(m_joints.th1 + m_joints.th2) + m_l3 * cos(m_joints.th1 + m_joints.th2 + m_joints.th3);

    double dy_dth3 = m_l3 * cos(m_joints.th1 + m_joints.th2 + m_joints.th3);

    ret = m_matrix.createMatrix33(dx_dth1, dx_dth2, dx_dth3, dy_dth1, dy_dth2, dy_dth3, 1.0, 1.0, 1.0);
     
    return ret;
}

coords_t Picker::get_tool_vel(joints_t joints_vel) throw()
{
    /*
        Computes current tool velocity using jacobian
    */
    coords_t new_vel;

    matrix_t jt_vel = m_matrix.createMatrix31(joints_vel.th1, joints_vel.th2, joints_vel.th3);

    matrix_t jacobian = compute_jacobian();
     
    matrix_t tl_vel = m_matrix.multMatrix33x13(jacobian , jt_vel);

    new_vel.x   = tl_vel[0][0];
    new_vel.y   = tl_vel[1][0];
    new_vel.phi = tl_vel[2][0];

    m_matrix.free(jt_vel);
    m_matrix.free(jacobian);
    m_matrix.free(tl_vel);
     
    return new_vel;
}

joints_t Picker::get_joints_vel(coords_t tool_vel)
{
    /*
        Computes current tool velocity using jacobian
    */
    joints_t vel;
    vel.th1 = 0;
    vel.th2 = 0;
    vel.th3 = 0;

    matrix_t tool_v = m_matrix.createMatrix31(tool_vel.x, tool_vel.y, tool_vel.phi);
     
    matrix_t jacobian = compute_jacobian();
     
    if (m_matrix.norm(tool_v) < EPSILON)
    {
        return vel;
    }
    if (abs(m_matrix.det(jacobian)) < EPSILON)
    {
        LOG_PICKER("Singularity");
        throw string("Singularity");
    }

    matrix_t joints_vel = m_matrix.solve(jacobian, tool_v);

    vel.th1 = joints_vel[0][0];
    vel.th2 = joints_vel[1][0];
    vel.th3 = joints_vel[2][0];

    m_matrix.free(tool_v);
    m_matrix.free(jacobian);
    m_matrix.free(joints_vel);
     
    return vel;
}

path_t Picker::get_path(coords_t start_pos, coords_t start_vel, coords_t target_pos, coords_t target_vel, double delta_t)
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

    new_path.path_th3 = Theta3_joint.get_path(start_joints_pos.th3,
                                                start_joints_vel.th3,
                                                target_joints_pos.th3,
                                                target_joints_vel.th3,
                                                tf_sync,
                                                delta_t);
     
    return new_path;
}

double Picker::synchronisation_time(joints_t start_pos, joints_t start_vel, joints_t target_pos, joints_t target_vel)
{
    /*        
    Return largest time to destination to use slowest joint as synchronisation
        reference
    */
    // Compute time to destination for all joints
    trajectory_time_t t_theta1 = Theta1_joint.time_to_destination(start_pos.th1, start_vel.th1, target_pos.th1, target_vel.th1);

    trajectory_time_t t_theta2 = Theta2_joint.time_to_destination(start_pos.th2, start_vel.th2, target_pos.th2, target_vel.th2);

    trajectory_time_t t_theta3 = Theta3_joint.time_to_destination(start_pos.th3, start_vel.th3, target_pos.th3, target_vel.th3);

    double maxi = max(t_theta1.tf, t_theta2.tf);
     
    return max(maxi, t_theta3.tf);
}

/***** Debug *****/
ostream& operator<< (ostream& out, const coords_t& c)
{
    out << "coords_t : ";
    out << "[x : " << c.x << " y : " << c.y << " phi : " << c.phi << "]";
    return out;
}
ostream& operator<< (ostream& out, const joints_t& j)
{
    out << "joints_t : ";
    out << "[th1 : " << j.th1 << " th2 : " << j.th2 << " th3 : " << j.th3 << "]";
    return out;
}
ostream& operator<< (ostream& out, const detailed_pos_t& d)
{
    out << "detailed_pos_t : " <<endl;
    out << "{" << endl;
    out << 	d.origin << endl;
    out << d.link1 << endl;
    out << d.link2 << endl;
    out << d.tool << endl;
    out << "}" << endl;
    return out;
}
ostream& operator<< (ostream& out, const path_t& p)
{
    out << "path_t : " <<endl;
    out << "{" << endl;
    out << "path th1 : "<< p.path_th1 << endl;
    out << "path th2 : "<< p.path_th2 << endl;
    out << "path th3 : "<< p.path_th3 << endl;
    out << "pos : " << p.pos << endl;
    out << "}" << endl;

    return out;
}

} /* end of namespace */