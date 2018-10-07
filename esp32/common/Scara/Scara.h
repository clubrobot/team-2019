#ifndef __SCARA_H
#define __SCARA_H
#include "Joint.h"
#include <Arduino.h>
#include <tuple>
#include <iostream>
#include <string>
#include <stdexcept>

typedef std::tuple<double,double> coords_t;

class Scara
{
	private:

		coords_t m_origin;
		coords_t m_tool;
		coords_t m_joints;

		double m_l1;
		double m_l2;
		double m_lsq;

		Joint Theta1_joint = Joint(-M_PI,M_PI,1,-1,1,-1,1);
		Joint Theta2_joint = Joint(-M_PI,M_PI,1,-1,1,-1,1);

	public:
		Scara(double l1, double l2, coords_t joints, coords_t origin);
		coords_t forward_kinematics(coords_t joints);

		coords_t inverse_kinematics(coords_t tool);

		coords_t get_tool(void);

		coords_t get_joints(void);


};


#endif /* __SCARA_H */