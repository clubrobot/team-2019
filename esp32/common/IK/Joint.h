#ifndef __JOINT_H
#define __JOINT_H

#include <Arduino.h>
#include <vector>
#include <iostream>
#include "thread_tools.h"
#include "datatype.h"
#include "constants.h"

using namespace std;

namespace IK
{

class Joint
{		
	public:
		/* Joint constructor */
		Joint(int id, float pos_min, float pos_max, float velociy_min, float velociy_max, float acc_min, float acc_max) throw();

		/* get joint path */
		vector_t get_path (float initial_pos, float initial_vel , float final_pos, float final_vel, float tf_sync, float delta_t) throw();
		/* get time to reach position */
		TrajectoryTime time_to_destination(float initial_pos, float initial_vel, float final_pos, float final_vel);

	private :
		/* private methods */
		float 			polyval       (Polynom polynome, float x) throw();
		Polynom 		polyder       (Polynom poly) throw();
		vector<float> 	vector_polyval(Polynom polynome, vector<float> x) throw();

		template<typename T>vector<T> arange(T start, T stop, T step) throw();

		/* trajectory generation utils */
		bool 		trajectory_is_feasible  (float initial_pos, float initial_vel, float final_pos, float final_vel);
		vector_t 	trapezoidal_profile	    (float initial_pos, float initial_vel , float final_pos, float final_vel, float tf_sync,float tf_lim, float delta_t) throw();
		vector_t 	floatramp_profile      (float initial_pos, float initial_vel , float final_pos, float final_vel, float tf_sync,float tf_lim, float delta_t) throw();
		vector_t 	generic_profile         (float initial_pos, float initial_vel, float final_pos, float final_vel, float tf_sync, float tf_lim,float delta_t, int sign_traj, int sign_sync, float vel_c) throw();
		int 		trajectory_sign		    (float initial_pos, float initial_vel, float final_pos, float final_vel) throw();
		vector_t 	polynomial_piece_profile(Polynom polynome, float start, float stop, float delta);
		
		/* private variables */
		int _id;

		/* constraints */
		Constraints _constraints;

		Mutex _mutex;
};
/***** Debug *****/
template<typename T>
ostream& operator<< (ostream& out, const vector<T>& v);
ostream& operator<< (ostream& out, const vector_t& v);
ostream& operator<< (ostream& out, const TrajectoryTime& t);

}

#endif /* __JOINT_H */
