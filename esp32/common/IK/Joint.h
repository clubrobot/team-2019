#ifndef __JOINT_H
#define __JOINT_H

#include <Arduino.h>
#include <tuple>
#include <vector>
#include <iostream>
#include <string>
#include <stdexcept>

using std::vector;
using std::ostream;

#define EPSILON 0.001

typedef struct
{
	double a0;
	double a1;
	double a2;
}polynom_t;

typedef struct 
{
	vector<double> t;
	vector<double> pos;
	vector<double> vel;
	vector<double> acc;
}vector_t;

typedef struct
{
	double pos_min;
	double pos_max;

	double vel_min;
	double vel_max;

	double acc_min;
	double acc_max;
}constraints_t;

typedef struct
{
	double t1;
	double t2;
	double tf;
}trajectory_time_t;

typedef enum
{
	NO_ERROR					= 0X00,
	TRAJECTORY_IS_NOT_FEASIBLE 	= 0X01,
	NON_CAUSAL_TRAJECTORY		= 0X02,
	TARGET_POSITION_UNREACHABLE	= 0X03,
	TARGET_VELOCITY_UNREACHABLE	= 0X04,
}joint_error_t;

class Joint
{

	private :
		int m_id;

		// constraints 
		constraints_t m_constraints;

		double polyval(polynom_t polynome, double x);
		polynom_t polyder(polynom_t poly);

		vector<double> vector_polyval(polynom_t polynome, vector<double> x);
		template<typename T>vector<T> arange(T start, T stop, T step);

		int m_error_byte;
		
	public:

		Joint(int id, double pos_min, double pos_max, double velociy_min, double velociy_max, double acc_min, double acc_max);

		int get_error();

		void reset_error();

		bool trajectory_is_feasible(double initial_pos, double initial_vel, double final_pos, double final_vel);

		vector_t get_path(double initial_pos, double initial_vel , double final_pos, double final_vel, double tf_sync, double delta_t);

		vector_t trapezoidal_profile(double initial_pos, double initial_vel , double final_pos, double final_vel, double tf_sync,double tf_lim, double delta_t);

		vector_t doubleramp_profile(double initial_pos, double initial_vel , double final_pos, double final_vel, double tf_sync,double tf_lim, double delta_t);

		vector_t generic_profile(double initial_pos, double initial_vel, double final_pos, double final_vel, double tf_sync, double tf_lim,double delta_t, int sign_traj, int sign_sync, double vel_c);

		vector_t polynomial_piece_profile(polynom_t polynome, double start, double stop, double delta);

		trajectory_time_t time_to_destination(double initial_pos, double initial_vel, double final_pos, double final_vel);

		int trajectory_sign(double initial_pos, double initial_vel, double final_pos, double final_vel);

		~Joint();
};

#endif /* __JOINT_H */
