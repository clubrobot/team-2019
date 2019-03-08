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

		coords_t _origin;
		coords_t _tool;
		joints_t _joints;

		float _l1;
		float _l2;
		float _lsq;

		Joint Theta1_joint = Joint(0,-M_PI,M_PI,-1,1,-1,1);
		Joint Theta2_joint = Joint(1,-M_PI,M_PI,-1,1,-1,1);

		Matrix2 _matrix;

	public:
		Scara(float l1, float l2, joints_t joints, coords_t origin);
		coords_t forward_kinematics(joints_t joints);

		joints_t inverse_kinematics(coords_t tool);

		coords_t get_tool(void);

		joints_t get_joints(void);

		detailed_pos_t get_detailed_pos(void);

		matrix_t compute_jacobian(void);

		coords_t get_tool_vel(joints_t joints_vel);

		joints_t get_joints_vel(coords_t tool_vel);

		path_t get_path(coords_t start_pos, coords_t start_vel, coords_t target_pos, coords_t target_vel, float delta_t);

		float synchronisation_time(joints_t start_pos, joints_t start_vel, joints_t target_pos, joints_t target_vel);
};

}

#endif /* __SCARA_H */