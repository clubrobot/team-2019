#ifndef __JOINT_H
#define __JOINT_H

#include <Arduino.h>
#include <vector>
#include <iostream>
#include "thread_tools.h"
#include "datatype.h"
#include "arm_config.h"

using namespace std;

namespace IK
{

class Joint
{		
	public:
		/* Joint constructor */
		Joint(int id, double pos_min, double pos_max, double velociy_min, double velociy_max, double acc_min, double acc_max) throw();

		/* get joint path */
		vector_t get_path (double initial_pos, double initial_vel , double final_pos, double final_vel, double tf_sync, double delta_t) throw();
		/* get time to reach position */
		trajectory_time_t time_to_destination(double initial_pos, double initial_vel, double final_pos, double final_vel);

	private :
		/* private methods */
		double 			polyval       (polynom_t polynome, double x) throw();
		polynom_t 		polyder       (polynom_t poly) throw();
		vector<double> 	vector_polyval(polynom_t polynome, vector<double> x) throw();

		template<typename T>vector<T> arange(T start, T stop, T step) throw();

		/* trajectory generation utils */
		bool 		trajectory_is_feasible  (double initial_pos, double initial_vel, double final_pos, double final_vel);
		vector_t 	trapezoidal_profile	    (double initial_pos, double initial_vel , double final_pos, double final_vel, double tf_sync,double tf_lim, double delta_t) throw();
		vector_t 	doubleramp_profile      (double initial_pos, double initial_vel , double final_pos, double final_vel, double tf_sync,double tf_lim, double delta_t) throw();
		vector_t 	generic_profile         (double initial_pos, double initial_vel, double final_pos, double final_vel, double tf_sync, double tf_lim,double delta_t, int sign_traj, int sign_sync, double vel_c) throw();
		int 		trajectory_sign		    (double initial_pos, double initial_vel, double final_pos, double final_vel) throw();
		vector_t 	polynomial_piece_profile(polynom_t polynome, double start, double stop, double delta);
		
		/* private variables */
		int _id;

		/* constraints */
		constraints_t _constraints;

		Mutex _mutex;
};
/***** Debug *****/
template<typename T>
ostream& operator<< (ostream& out, const vector<T>& v);
ostream& operator<< (ostream& out, const vector_t& v);
ostream& operator<< (ostream& out, const trajectory_time_t& t);

}

#endif /* __JOINT_H */
