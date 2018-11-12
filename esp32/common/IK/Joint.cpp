#include <Arduino.h>
#include <tuple>
#include <iostream>
#include <string>
#include <stdexcept>

#include "Joint.h"


Joint::Joint(int id, double pos_min, double pos_max, double velociy_min, double velociy_max, double acc_min, double acc_max)
{
	m_id = id;

	// constraints 
	m_constraints.pos_min = pos_min;
	m_constraints.pos_max = pos_max;

	m_constraints.vel_min = velociy_min;
	m_constraints.vel_max = velociy_max;

	m_constraints.acc_min = acc_min;
	m_constraints.acc_max = acc_max;
}
Joint::~Joint()
{
    
}
double Joint::polyval(polynom_t polynome, double x)
{
	return polynome.a2 * pow(x,2) + polynome.a1 * x + polynome.a0;
}

polynom_t Joint::polyder(polynom_t poly)
{
    polynom_t dp;

    dp.a0 = poly.a1;
    dp.a1 = 2*poly.a2;
    dp.a2 = 0;

    return dp;
}

vector<double> Joint::vector_polyval(polynom_t polynome, vector<double> x)
{
    vector<double> values;
    int size = x.size();
    for(int i = 0; i<size; i++)
        values.push_back(polynome.a2*pow(x.at(i),2) + polynome.a1 * x.at(i) + polynome.a0);
    return values;
}


template<typename T>vector<T> Joint::arange(T start, T stop, T step) 
{
    vector<T> values;
    for (T value = start; value < stop; value += step)
        values.push_back(value);
    return values;
}

bool Joint::trajectory_is_feasible(double initial_pos, double initial_vel, double final_pos, double final_vel)
{
	// checks boundaries to determine feasibility

	if(final_pos > (m_constraints.pos_max + EPSILON) || final_pos < (m_constraints.pos_min - EPSILON))
	{
        //throw String("Target position unreachable by joint");
		return false;
	}

	if(final_vel > (m_constraints.vel_max + EPSILON) || final_vel < (m_constraints.vel_min - EPSILON))
	{
        //throw String("Target velocity unreachable by joint");
		return false;
	}

	double delta_p_dec = 0.5 * final_vel * abs(final_vel) / m_constraints.acc_max;

	if((final_pos + delta_p_dec) > (m_constraints.pos_max + EPSILON) || (final_pos + delta_p_dec) < (m_constraints.pos_min - EPSILON))
	{
        //throw String("Target position unreachable at specified velocity by joint");
		return false;
	}

    return true;
}

vector_t Joint::get_path(double initial_pos, double initial_vel , double final_pos, double final_vel, double tf_sync, double delta_t)
{
	/*
        Generates a time optimal trajectory
        Input:
        initial_pos - initial position
        initial_vel - initial velocity
        final_pos - final position
        final_vel - final velocity
        tf_sync - duration of the trajectory (synchronisation time)
    */
    // Compute limit time
    double delta_p = final_pos - initial_pos;
    int sign_traj = trajectory_sign(initial_pos, initial_vel, final_pos, final_vel);

    double vel_c;

    if( (abs(initial_vel) < EPSILON) || (abs(final_vel) < EPSILON) )
    {
        vel_c = EPSILON; //ensure we use a trapezoidal trajectory
    }
    else if(abs(initial_vel) < abs(final_vel))
    {
        vel_c = final_vel;
    }
    else
    {
        vel_c = initial_vel;
    }

    double tf_lim = (delta_p / vel_c) + (0.5 * sign_traj * (vel_c - final_vel) / m_constraints.acc_max) + (0.5 * sign_traj * (initial_vel - vel_c) / m_constraints.acc_max);

    //Determine shape of trajectory
    if( (tf_sync < tf_lim) || (initial_vel == 0 &&& final_vel == 0))
    {
    	return trapezoidal_profile(initial_pos, initial_vel, final_pos, final_vel, tf_sync, tf_lim, delta_t);
    }
    else
    {
        return doubleramp_profile(initial_pos, initial_vel, final_pos, final_vel, tf_sync, tf_lim, delta_t);
    }
}

vector_t Joint::trapezoidal_profile(double initial_pos, double initial_vel , double final_pos, double final_vel, double tf_sync,double tf_lim, double delta_t)
{
	/*
        Generate a trapezoidal profile to reach target
    */

    //Compute cruise speed
    double delta_p = final_pos - initial_pos;

    int sign_traj = trajectory_sign(initial_pos, initial_vel, final_pos, final_vel);
    
    //Avoid division by 0
    if(sign_traj == 0)
    {
        sign_traj = 1;
    }

    double b = m_constraints.acc_max * tf_sync + sign_traj * initial_vel;

    double vel_c = 0.5 * ( b - sqrt( pow(b,2) - 4 * sign_traj * m_constraints.acc_max * delta_p - 2 * pow((initial_vel - final_vel),2) ) );

    return generic_profile(initial_pos, initial_vel, final_pos, final_vel, tf_sync, tf_lim, delta_t, sign_traj, 1, vel_c);
}

vector_t Joint::doubleramp_profile(double initial_pos, double initial_vel , double final_pos, double final_vel, double tf_sync,double tf_lim, double delta_t)
{
	/*
        Generate a double ramp profile to reach target
    */

    // Compute cruise speed     
    double delta_p = final_pos - initial_pos;

    int sign_traj = trajectory_sign(initial_pos, initial_vel, final_pos, final_vel);

    // Avoid division by 0
    if( sign_traj == 0)
    {
    	sign_traj = 1;
    }   

   	double vel_c = (sign_traj * delta_p - 0.5 * (pow((initial_vel - final_vel),2) / m_constraints.acc_max)) / (tf_sync - ((initial_vel  - final_vel) / (sign_traj * m_constraints.acc_max)));

    return generic_profile(initial_pos, initial_vel, final_pos, final_vel, tf_sync, tf_lim, delta_t, sign_traj, 1, vel_c);
}

vector_t Joint::generic_profile(double initial_pos, double initial_vel, double final_pos, double final_vel, double tf_sync, double tf_lim,double delta_t, int sign_traj, int sign_sync, double vel_c)
{
 	/*
        Generate a generic profile (valid for trapezoidal and double ramp)
    */

    double t1 = (sign_traj * vel_c - initial_vel) / (sign_traj * m_constraints.acc_max);
    double t2 = tf_sync - abs(vel_c - final_vel) / m_constraints.acc_max;

    
    polynom_t poly;

    // First piece
    poly.a0 = initial_pos;
    poly.a1 = initial_vel;
    poly.a2 = 0.5 * sign_traj * sign_sync * m_constraints.acc_max;

    vector_t traj1 = polynomial_piece_profile(poly, 0, t1, delta_t);

    // Second piece
    poly.a0 = polyval(poly, t1);
    poly.a1 = sign_traj * vel_c;
    poly.a2 = 0;

    vector_t traj2 = polynomial_piece_profile(poly, t1, t2, delta_t);

    //Third piece
    poly.a0 = polyval(poly, t2 - t1);
    poly.a1 = sign_traj * vel_c;
    poly.a2 = - 0.5 * sign_traj * m_constraints.acc_max;

    vector_t traj3 = polynomial_piece_profile(poly, t2, tf_sync, delta_t);

    //last piece
    vector_t traj4;

    traj4.t.push_back(tf_sync);
    traj4.pos.push_back(final_pos);
    traj4.vel.push_back(final_vel);
    traj4.acc.push_back(0.0);

    // Combine trajectory
    vector_t complete_path;

    // time vecor
    complete_path.t.insert( complete_path.t.end(), traj1.t.begin(), traj1.t.end());
    complete_path.t.insert( complete_path.t.end(), traj2.t.begin(), traj2.t.end());
    complete_path.t.insert( complete_path.t.end(), traj3.t.begin(), traj3.t.end());
    complete_path.t.insert( complete_path.t.end(), traj4.t.begin(), traj4.t.end());

    // pos vector
    complete_path.pos.insert( complete_path.pos.end(), traj1.pos.begin(), traj1.pos.end());
    complete_path.pos.insert( complete_path.pos.end(), traj2.pos.begin(), traj2.pos.end());
    complete_path.pos.insert( complete_path.pos.end(), traj3.pos.begin(), traj3.pos.end());
    complete_path.pos.insert( complete_path.pos.end(), traj4.pos.begin(), traj4.pos.end());

    // velocity vector
    complete_path.vel.insert( complete_path.vel.end(), traj1.vel.begin(), traj1.vel.end());
    complete_path.vel.insert( complete_path.vel.end(), traj2.vel.begin(), traj2.vel.end());
    complete_path.vel.insert( complete_path.vel.end(), traj3.vel.begin(), traj3.vel.end());
    complete_path.vel.insert( complete_path.vel.end(), traj4.vel.begin(), traj4.vel.end());

    //acceleration vector
    complete_path.acc.insert( complete_path.acc.end(), traj1.acc.begin(), traj1.acc.end());
    complete_path.acc.insert( complete_path.acc.end(), traj2.acc.begin(), traj2.acc.end());
    complete_path.acc.insert( complete_path.acc.end(), traj3.acc.begin(), traj3.acc.end());
    complete_path.acc.insert( complete_path.acc.end(), traj4.acc.begin(), traj4.acc.end());

    return complete_path;
}

vector_t Joint::polynomial_piece_profile(polynom_t polynome, double start, double stop, double delta)
{
    /*
        Generate a polynomial piece profile
        Return time, position, velocity and acceleration discrete profile
    */

    vector_t piece;

    if (stop < start)
    {
       //throw String("Non causal trajectory profile requested"); 
    }
        
    polynom_t polynome_dot = polyder(polynome);
    polynom_t polynome_dot_dot = polyder(polynome_dot);

    vector<double> time = arange<double>(start, stop, delta);
    vector<double> dtime = arange<double>(0, stop-start, delta);

    piece.t = time;
    piece.pos = vector_polyval(polynome, dtime);
    piece.vel = vector_polyval(polynome_dot, dtime);
    piece.acc = vector_polyval(polynome_dot_dot, dtime);

    return piece;
}

trajectory_time_t Joint::time_to_destination(double initial_pos, double initial_vel, double final_pos, double final_vel)
{
    trajectory_time_t time_traj;

    if(!trajectory_is_feasible(initial_pos, initial_vel, final_pos, final_vel))
    {
        //throw String("Trajectory is not feasible"); 
    }

    double delta_p = final_pos - initial_pos;

    if(delta_p == 0)
    {
        time_traj.t1 = 0;
        time_traj.t2 = 0;
        time_traj.tf = 0;
        return time_traj;
    }

    int sign_traj = trajectory_sign(initial_pos, initial_vel, final_pos, final_vel);

   
    time_traj.t1 = (sign_traj * m_constraints.vel_max - initial_vel) / (sign_traj * m_constraints.acc_max);

    time_traj.t2 = (1 / m_constraints.vel_max) * ((pow(final_vel,2) + pow(initial_vel,2 - 2 * sign_traj * initial_vel) / (2 * m_constraints.acc_max) + (sign_traj * delta_p)));

    time_traj.tf = time_traj.t2 - (final_vel - sign_traj * m_constraints.vel_max) / (sign_traj * m_constraints.acc_max);

    return time_traj;
}

int Joint::trajectory_sign(double initial_pos, double initial_vel, double final_pos, double final_vel)
{
	/*
        Get sign of trajectory to be executed
    */ 
    double delta_p = final_pos - initial_pos;
    double delta_v = final_vel - initial_vel;

    int sign = 0;
    if(delta_v == 0)
    {
    	sign = 0;
    }
    else if(delta_v < 0)
    {
    	sign = -1; 
    }
    else if (delta_v > 0)
    {
    	sign = 1;
    }

    double delta_p_crit = 0.5 * sign * (pow(final_vel,2) - pow(initial_vel,2))/ m_constraints.acc_max;

    if(delta_p - delta_p_crit == 0)
    {
    	return 0;
    }
    else if(delta_p - delta_p_crit < 0)
    {
    	return -1; 
    }
    else if (delta_p - delta_p_crit > 0)
    {
    	return 1;
    }
}