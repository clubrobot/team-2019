#include <Arduino.h>
#include <math.h>
#include "Picker.h"
#include "constants.h"

#ifdef IK_LOG
    #define LOG_PICKER(arg) cout << __TIME__<<" (PICKER)("<< __func__ << " , " << __LINE__ << ")\t\t\t: "<< arg <<endl;
#else
    #define LOG_PICKER(arg) 
#endif

namespace IK
{

bool equals(float a, float b, float epsilon = EPSILON)
{ 
    return ((std::abs(a - b) < epsilon));
}


void Picker::init(float l1, float l2, float l3, Joints joints, Coords origin, int elbow_or) throw()
{
    _joints 	= joints;
	_origin	= origin;

	_tool	 	= get_tool();

	_l1		= l1;
	_l2		= l2;
    _l3        = l3;
	_lsq   	= pow(l1,2)+pow(l2,2);

    _flip_elbow = elbow_or;
}

void Picker::flip_elbow(int elbow) throw()
{
    _flip_elbow = elbow;
}

Coords Picker::forward_kinematics(Joints joints) throw()
{
    Coords ret;
	_joints = joints;

	ret = get_tool();

    return ret;
}

Joints Picker::inverse_kinematics(Coords tool)
{
    Joints ret;

    float dotx,doty,costh;

    dotx = (_tool.x - _origin.x) - (_l3 * cos(_tool.phi));
    doty = (_tool.y - _origin.y) - (_l3 * sin(_tool.phi));

    costh = (pow(dotx,2) + pow(doty,2) - pow(_l1,2) - pow(_l2, 2)) / (2 * _l1 * _l2);

    if(costh < -1 || costh > 1)
    {
        _tool = get_tool();
        LOG_PICKER("Target unreacheable");
        throw string("Target unreacheable");
    }
    else
    {
        _tool = tool;

        _joints = get_joints();

        ret = _joints;
    }      
    return ret;
}

Coords Picker::get_tool(void) const throw()
{
    Coords new_cords;

    new_cords.x     = _l1 * cos(_joints.th1) + _l2 * cos(_joints.th1 + _joints.th2) + _l3 * cos(_joints.th1 + _joints.th2 + _joints.th3);
    new_cords.y     = _l1 * sin(_joints.th1) + _l2 * sin(_joints.th1 + _joints.th2) + _l3 * sin(_joints.th1 + _joints.th2 + _joints.th3);
    new_cords.phi   = _joints.th1 + _joints.th2 + _joints.th3;

    new_cords.x     += _origin.x;
    new_cords.y     += _origin.y;
    new_cords.phi   += _origin.phi;

    return new_cords;
}

Joints Picker::get_joints(void) const throw()
{
    Joints new_joints;

    float dotx,doty,costh,k1,k2,c2,s2;

    dotx = (_tool.x - _origin.x) - (_l3 * cos(_tool.phi));
    doty = (_tool.y - _origin.y) - (_l3 * sin(_tool.phi));

    costh = (pow(dotx,2) + pow(doty,2) - pow(_l1,2) - pow(_l2, 2)) / (2 * _l1 * _l2);

    new_joints.th2 = _flip_elbow * acos(costh);

    k2 = _l2 * sin(new_joints.th2);
    k1 = _l1 + (_l2 * cos(new_joints.th2));

    new_joints.th1 = atan2(doty, dotx) - atan2(k2, k1);
    new_joints.th3 = _tool.phi - new_joints.th1 - new_joints.th2;


    new_joints.th1 = std::fmod((new_joints.th1 + (M_PI)) , 2*M_PI) - M_PI; // Stay between -pi and pi
    new_joints.th2 = std::fmod((new_joints.th2 + (M_PI)) , 2*M_PI) - M_PI; // Stay between -pi and pi
    new_joints.th3 = std::fmod((new_joints.th3 + (M_PI)) , 2*M_PI) - M_PI; // Stay between -pi and pi

    return new_joints;
}

DetailedPos Picker::get_detailed_pos(void) const throw()
{
    /*
        Returns origin, position of end of link 1, position of end of link 2
    */ 
    DetailedPos new_pos;

    new_pos.link1.x = _l1 * cos(_joints.th1) + _origin.x;
    new_pos.link1.y = _l1 * sin(_joints.th1) + _origin.y;
    
    new_pos.link2.x = new_pos.link1.x + _l2 * cos(_joints.th1 + _joints.th2);
    new_pos.link2.y = new_pos.link1.x + _l2 * cos(_joints.th1 + _joints.th2);

    new_pos.origin  = _origin;
    new_pos.tool    = _tool;

    return new_pos;
}

matrix_t Picker::compute_jacobian(void) throw()
{
    /*
        Returns jacobian matrix at current state
    */
    matrix_t ret;

    float dx_dth1 = - _l1 * sin(_joints.th1) - _l2 * sin(_joints.th1 + _joints.th2) - _l3 * sin(_joints.th1 + _joints.th2 + _joints.th3);

    float dx_dth2 = - _l2 * sin(_joints.th1 + _joints.th2) - _l3 * sin(_joints.th1 + _joints.th2 + _joints.th3);

    float dx_dth3 = - _l3 * sin(_joints.th1 + _joints.th2 + _joints.th3);;

    float dy_dth1 = _l1 * cos(_joints.th1) + _l2 * cos(_joints.th1 + _joints.th2) + _l3 * cos(_joints.th1 + _joints.th2 + _joints.th3);

    float dy_dth2 = _l2 * cos(_joints.th1 + _joints.th2) + _l3 * cos(_joints.th1 + _joints.th2 + _joints.th3);

    float dy_dth3 = _l3 * cos(_joints.th1 + _joints.th2 + _joints.th3);

    ret = _matrix.createMatrix33(dx_dth1, dx_dth2, dx_dth3, dy_dth1, dy_dth2, dy_dth3, 1.0, 1.0, 1.0);

    return ret;
}

Coords Picker::get_tool_vel(Joints joints_vel) throw()
{
    /*
        Computes current tool velocity using jacobian
    */
    Coords new_vel;

    matrix_t jt_vel = _matrix.createMatrix31(joints_vel.th1, joints_vel.th2, joints_vel.th3);

    matrix_t jacobian = compute_jacobian();

    matrix_t tl_vel = _matrix.multMatrix33x13(jacobian , jt_vel);

    new_vel.x   = tl_vel[0][0];
    new_vel.y   = tl_vel[1][0];
    new_vel.phi = tl_vel[2][0];

    _matrix.free(jt_vel);
    _matrix.free(jacobian);
    _matrix.free(tl_vel);

    return new_vel;
}

Joints Picker::get_joints_vel(Coords tool_vel)
{
    /*
        Computes current tool velocity using jacobian
    */
    Joints vel;
    vel.th1 = 0;
    vel.th2 = 0;
    vel.th3 = 0;

    matrix_t tool_v = _matrix.createMatrix31(tool_vel.x, tool_vel.y, tool_vel.phi);

    matrix_t jacobian = compute_jacobian();

    if (_matrix.norm(tool_v) < EPSILON)
    {
        return vel;
    }
    if (std::abs(_matrix.det(jacobian)) < EPSILON)
    {
        LOG_PICKER("Singularity");
        throw string("Singularity");
    }

    matrix_t joints_vel = _matrix.solve(jacobian, tool_v);

    vel.th1 = joints_vel[0][0];
    vel.th2 = joints_vel[1][0];
    vel.th3 = joints_vel[2][0];

    _matrix.free(tool_v);
    _matrix.free(jacobian);
    _matrix.free(joints_vel);

    return vel;
}

path_t Picker::get_path(Coords start_pos, Coords start_vel, Coords target_pos, Coords target_vel, float delta_t)
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

    new_path.path_th3 = Theta3_joint.get_path(start_joints_pos.th3,
                                                start_joints_vel.th3,
                                                target_joints_pos.th3,
                                                target_joints_vel.th3,
                                                tf_sync,
                                                delta_t);

    return new_path;
}

float Picker::synchronisation_time(Joints start_pos, Joints start_vel, Joints target_pos, Joints target_vel)
{
    /*        
    Return largest time to destination to use slowest joint as synchronisation
        reference
    */
    // Compute time to destination for all joints
    TrajectoryTime t_theta1 = Theta1_joint.time_to_destination(start_pos.th1, start_vel.th1, target_pos.th1, target_vel.th1);

    TrajectoryTime t_theta2 = Theta2_joint.time_to_destination(start_pos.th2, start_vel.th2, target_pos.th2, target_vel.th2);

    TrajectoryTime t_theta3 = Theta3_joint.time_to_destination(start_pos.th3, start_vel.th3, target_pos.th3, target_vel.th3);

    float maxi = max(t_theta1.tf, t_theta2.tf);

    return max(maxi, t_theta3.tf);
}

/***** Debug *****/
ostream& operator<< (ostream& out, const Coords& c)
{
    out << "Coords : ";
    out << "[x : " << c.x << " y : " << c.y << " phi : " << c.phi << "]";
    return out;
}
ostream& operator<< (ostream& out, const Joints& j)
{
    out << "Joints : ";
    out << "[th1 : " << j.th1 << " th2 : " << j.th2 << " th3 : " << j.th3 << "]";
    return out;
}
ostream& operator<< (ostream& out, const DetailedPos& d)
{
    out << "DetailedPos : " <<endl;
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