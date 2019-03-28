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

		void init(float l1, float l2, float l3, Joints joints, Coords origin, int elbow_or) throw();
		
		void flip_elbow(int elbow) throw();
		
		Coords forward_kinematics(Joints joints) throw();

		Joints inverse_kinematics(Coords tool);

		Coords get_tool(void) const throw();

		Joints get_joints(void) const throw();

		DetailedPos get_detailed_pos(void) const throw();

		Coords get_tool_vel(Joints joints_vel) throw();

		Joints get_joints_vel(Coords tool_vel);

		matrix_t compute_jacobian(void) throw();

		path_t get_path(Coords start_pos, Coords start_vel, Coords target_pos, Coords target_vel, float delta_t);

		float synchronisation_time(Joints start_pos, Joints start_vel, Joints target_pos, Joints target_vel);

		float _flip_elbow;
		Constraints x_axis;
		Constraints y_axis;
		Constraints phi_axis;

	private:

		Coords _origin;
		Coords _tool;
		Joints _joints;

		float _l1;
		float _l2;
		float _l3;
		float _lsq;

		Joint Theta1_joint = Joint(0, -M_PI, M_PI, -AX_12_MAX_SPEED_RAD, AX_12_MAX_SPEED_RAD, -25, 25);
		Joint Theta2_joint = Joint(1, -(5*M_PI)/6, (5*M_PI)/6, -AX_12_MAX_SPEED_RAD, AX_12_MAX_SPEED_RAD, -25, 25);
		Joint Theta3_joint = Joint(2, -M_PI, M_PI, -AX_12_MAX_SPEED_RAD, AX_12_MAX_SPEED_RAD, -25, 25);

		Matrix3 _matrix;

		Mutex _mutex;

};

/***** Debug *****/
ostream& operator<< (ostream& out, const Coords& c);
ostream& operator<< (ostream& out, const Joints& j);
ostream& operator<< (ostream& out, const DetailedPos& d);
ostream& operator<< (ostream& out, const path_t& p);
}

#endif /* __PICKER_H */