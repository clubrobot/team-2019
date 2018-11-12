#ifndef __PICKER_H
#define __PICKER_H

#include "Joint.h"
#include <Arduino.h>
#include "Joint.h"
#include <tuple>
#include <iostream>
#include <string>
#include <stdexcept>
#include "Matrix.h"

typedef struct
{
	double x;
	double y;
	double phi;
}coords_t;

typedef struct
{
	double th1;
	double th2;
	double th3;
	
}joints_t;

typedef struct
{
	coords_t origin;
	coords_t link1;
	coords_t link2;
	coords_t tool; 
}detailed_pos_t;

typedef struct
{
	vector_t path_th1;
	vector_t path_th2;
	vector_t path_th3;
}path_t;


class Picker
{
	private:

		coords_t m_origin;
		coords_t m_tool;
		joints_t m_joints;

		double m_l1;
		double m_l2;
		double m_l3;
		double m_lsq;

		Joint Theta1_joint = Joint(0, -M_PI, M_PI, -1, 1, -1, 1);
		Joint Theta2_joint = Joint(1, -M_PI, M_PI, -1, 1, -1, 1);
		Joint Theta3_joint = Joint(2, -M_PI, M_PI, -1, 1, -1, 1);

		Matrix3 m_matrix;

	public:
		Picker(double l1, double l2, double l3, joints_t joints, coords_t origin);
		coords_t forward_kinematics(joints_t joints);

		joints_t inverse_kinematics(coords_t tool);

		coords_t get_tool(void);

		joints_t get_joints(void);

		detailed_pos_t get_detailed_pos(void);

		matrix_t compute_jacobian(void);

		coords_t get_tool_vel(joints_t joints_vel);

		joints_t get_joints_vel(coords_t tool_vel);

		path_t get_path(coords_t start_pos, coords_t start_vel, coords_t target_pos, coords_t target_vel, double delta_t);

		double synchronisation_time(joints_t start_pos, joints_t start_vel, joints_t target_pos, joints_t target_vel);

		~Picker();

		constraints_t x_axis;
		constraints_t y_axis;
		constraints_t phi_axis;

};


#endif /* __PICKER_H */