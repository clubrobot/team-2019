#ifndef __PICKER_H
#define __PICKER_H

#include "Joint.h"
#include <Arduino.h>
#include "Matrix.h"
#include "thread_tools.h"
#include "datatype.h"

#define AX_12_MAX_SPEED_RAD 6.178465545

namespace IK
{

class Picker
{
	public:

		void init(double l1, double l2, double l3, joints_t joints, coords_t origin, int elbow_or) throw();
		
		void flip_elbow(int elbow) throw();
		
		coords_t forward_kinematics(joints_t joints) throw();

		joints_t inverse_kinematics(coords_t tool);

		coords_t get_tool(void) const throw();

		joints_t get_joints(void) const throw();

		detailed_pos_t get_detailed_pos(void) const throw();

		coords_t get_tool_vel(joints_t joints_vel) throw();

		joints_t get_joints_vel(coords_t tool_vel);

		matrix_t compute_jacobian(void) throw();

		path_t get_path(coords_t start_pos, coords_t start_vel, coords_t target_pos, coords_t target_vel, double delta_t);

		double synchronisation_time(joints_t start_pos, joints_t start_vel, joints_t target_pos, joints_t target_vel);

		double m_flip_elbow;
		constraints_t x_axis;
		constraints_t y_axis;
		constraints_t phi_axis;

	private:

		coords_t m_origin;
		coords_t m_tool;
		joints_t m_joints;

		double m_l1;
		double m_l2;
		double m_l3;
		double m_lsq;

		Joint Theta1_joint = Joint(0, -M_PI, M_PI, -AX_12_MAX_SPEED_RAD, AX_12_MAX_SPEED_RAD, -25, 25);
		Joint Theta2_joint = Joint(1, -(5*M_PI)/6, (5*M_PI)/6, -AX_12_MAX_SPEED_RAD, AX_12_MAX_SPEED_RAD, -25, 25);
		Joint Theta3_joint = Joint(2, -(5*M_PI)/6, (5*M_PI)/6, -AX_12_MAX_SPEED_RAD, AX_12_MAX_SPEED_RAD, -25, 25);

		Matrix3 m_matrix;

		Mutex m_mutex;

};

/***** Debug *****/
ostream& operator<< (ostream& out, const coords_t& c);
ostream& operator<< (ostream& out, const joints_t& j);
ostream& operator<< (ostream& out, const detailed_pos_t& d);
ostream& operator<< (ostream& out, const path_t& p);
}

#endif /* __PICKER_H */