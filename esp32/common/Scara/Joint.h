#ifndef __JOINT_H
#define __JOINT_H

#include <Arduino.h>
#include <tuple>
#include <iostream>
#include <string>
#include <stdexcept>
#define EPSILON 0.001

typedef std::tuple<double,double,double,double> double_tuple_t;

typedef std::tuple<double_tuple_t , double_tuple_t , double_tuple_t , double_tuple_t> tuple_type_t;


class Joint
{

	private :
		int m_id;

		// constraints 
		double m_pos_min;
		double m_pos_max;

		double m_velocity_min;
		double m_velocity_max;

		double m_acc_min;
		double m_acc_max;

		double polyval(std::tuple<double,double,double>polynome, double x);
		
	public:

		Joint(int id, double pos_min, double pos_max, double velociy_min, double velociy_max, double acc_min, double acc_max);

		bool trajectory_is_feasible(double initial_pos, double initial_vel, double final_pos, double final_vel);

		tuple_type_t get_path(double initial_pos, double initial_vel , double final_pos, double final_vel, double tf_sync, double delta_t);

		tuple_type_t trapezoidal_profile(double initial_pos, double initial_vel , double final_pos, double final_vel, double tf_sync,double tf_lim, double delta_t);

		tuple_type_t doubleramp_profile(double initial_pos, double initial_vel , double final_pos, double final_vel, double tf_sync,double tf_lim, double delta_t);

		tuple_type_t generic_profile(double initial_pos, double initial_vel, double final_pos, double final_vel, double tf_sync, double tf_lim,double delta_t, int sign_traj, int sign_sync, double vel_c);

		double_tuple_t polynomial_piece_profile(std::tuple<double,double,double>polynome, double start, double stop, double delta);

		std::tuple<double,double,double> time_to_destination(double initial_pos, double initial_vel, double final_pos, double final_vel);

		int trajectory_sign(double initial_pos, double initial_vel, double final_pos, double final_vel);


};

#endif /* __JOINT_H */
