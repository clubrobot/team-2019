#ifndef __JOINT_H
#define __JOINT_H

#include <Arduino.h>
#include <tuple>
#include <iostream>
#include <string>
#include <stdexcept>
#define EPSILON 0.001

typedef std::tuple<float,float,float,float> float_tuple_t;

typedef std::tuple<float_tuple_t , float_tuple_t , float_tuple_t , float_tuple_t> tuple_type_t;


class Joint
{

	private :
		int m_id;

		// constraints 
		float m_pos_min;
		float m_pos_max;

		float m_velocity_min;
		float m_velocity_max;

		float m_acc_min;
		float m_acc_max;

		float polyval(std::tuple<float,float,float>polynome, float x);
		
	public:

		Joint(int id, float pos_min, float pos_max, float velociy_min, float velociy_max, float acc_min, float acc_max);

		bool trajectory_is_feasible(float initial_pos, float initial_vel, float final_pos, float final_vel);

		tuple_type_t get_path(float initial_pos, float initial_vel , float final_pos, float final_vel, float tf_sync, float delta_t);

		tuple_type_t trapezoidal_profile(float initial_pos, float initial_vel , float final_pos, float final_vel, float tf_sync,float tf_lim, float delta_t);

		tuple_type_t doubleramp_profile(float initial_pos, float initial_vel , float final_pos, float final_vel, float tf_sync,float tf_lim, float delta_t);

		tuple_type_t generic_profile(float initial_pos, float initial_vel, float final_pos, float final_vel, float tf_sync, float tf_lim,float delta_t, int sign_traj, int sign_sync, float vel_c);

		float_tuple_t polynomial_piece_profile(std::tuple<float,float,float>polynome, float start, float stop, float delta);

		std::tuple<float,float,float> time_to_destination(float initial_pos, float initial_vel, float final_pos, float final_vel);

		int trajectory_sign(float initial_pos, float initial_vel, float final_pos, float final_vel);


};

#endif /* __JOINT_H */
