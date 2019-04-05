#ifndef __SCARA_H
#define __SCARA_H
#include "Joint.h"
#include <Arduino.h>
#include <tuple>
#include <iostream>
#include <string>
#include <stdexcept>
#include "Matrix.h"
#include "datatype.h"

namespace IK
{

class Scara
{
	private:

		Coords _origin;
		Coords _tool;
		Joints _joints;

		float _l1;
		float _l2;
		float _lsq;

		Joint Theta1_joint = Joint(0,-M_PI,M_PI,-1,1,-1,1);
		Joint Theta2_joint = Joint(1,-M_PI,M_PI,-1,1,-1,1);

		Matrix2 _matrix;

	public:
		Scara(float l1, float l2, Joints joints, Coords origin);
		Coords forward_kinematics(Joints joints);

		Joints inverse_kinematics(Coords tool);

		Coords get_tool(void);

		Joints get_joints(void);

		DetailedPos get_detailed_pos(void);

		matrix_t compute_jacobian(void);

		Coords get_tool_vel(Joints joints_vel);

		Joints get_joints_vel(Coords tool_vel);

		path_t get_path(Coords start_pos, Coords start_vel, Coords target_pos, Coords target_vel, float delta_t);

		float synchronisation_time(Joints start_pos, Joints start_vel, Joints target_pos, Joints target_vel);
};

}

#endif /* __SCARA_H */