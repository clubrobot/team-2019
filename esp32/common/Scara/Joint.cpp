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
	m_pos_min = pos_min;
	m_pos_max = pos_max;

	m_velocity_min = velociy_min;
	m_velocity_max = velociy_max;

	m_acc_min = acc_min;
	m_acc_max = acc_max;
}

double Joint::polyval(std::tuple<double,double,double>polynome, double x)
{
	double a2,a1,a0;

	std::tie(a2,a1,a0) = polynome;

	return a2*pow(x,2) + a1*pow(x,1) + a0 * x;
}

bool Joint::trajectory_is_feasible(double initial_pos, double initial_vel, double final_pos, double final_vel)
{

	// checks boundaries to determine feasibility

	if(final_pos > (m_pos_max + EPSILON) || final_pos < (m_pos_min - EPSILON))
	{
		return false;
	}

	if(final_vel > (m_velocity_max + EPSILON) || final_vel < (m_velocity_min - EPSILON))
	{
		return false;
	}

	double delta_p_dec = 0.5 * final_vel * abs(final_vel) / m_acc_max;

	if((final_pos + delta_p_dec) > (m_pos_max + EPSILON) || (final_pos + delta_p_dec) < (m_pos_min - EPSILON))
	{
		return false;
	}

    return true;
}

tuple_type_t Joint::get_path(double initial_pos, double initial_vel , double final_pos, double final_vel, double tf_sync, double delta_t)
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

    double tf_lim = (delta_p / vel_c) + (0.5 * sign_traj * (vel_c - final_vel) / m_acc_max) + (0.5 * sign_traj * (initial_vel - vel_c) / m_acc_max);

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

tuple_type_t Joint::trapezoidal_profile(double initial_pos, double initial_vel , double final_pos, double final_vel, double tf_sync,double tf_lim, double delta_t)
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

    double b = m_acc_max * tf_sync + sign_traj * initial_vel;

    double vel_c = 0.5 * ( b - sqrt( pow(b,2) - 4 * sign_traj * m_acc_max * delta_p - 2 * pow((initial_vel - final_vel),2) ) );

    return generic_profile(initial_pos, initial_vel, final_pos, final_vel, tf_sync, tf_lim, delta_t, sign_traj, 1, vel_c);
}

tuple_type_t Joint::doubleramp_profile(double initial_pos, double initial_vel , double final_pos, double final_vel, double tf_sync,double tf_lim, double delta_t)
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

   	double vel_c = (sign_traj * delta_p - 0.5 * (pow((initial_vel - final_vel),2) / m_acc_max)) / (tf_sync - ((initial_vel  - final_vel) / (sign_traj * m_acc_max)));

    return generic_profile(initial_pos, initial_vel, final_pos, final_vel, tf_sync, tf_lim, delta_t, sign_traj, 1, vel_c);
}

tuple_type_t Joint::generic_profile(double initial_pos, double initial_vel, double final_pos, double final_vel, double tf_sync, double tf_lim,double delta_t, int sign_traj, int sign_sync, double vel_c)
{
 	/*
        Generate a generic profile (valid for trapezoidal and double ramp)
    */

    double t1 = (sign_traj * vel_c - initial_vel) / (sign_traj * m_acc_max);
    double t2 = tf_sync - abs(vel_c - final_vel) / m_acc_max;

    // First piece
    double a0 = initial_pos;
    double a1 = initial_vel;
    double a2 = 0.5 * sign_traj * sign_sync * m_acc_max;

    double time_1, traj_pos_1, traj_vel_1, traj_acc_1;
   	std::tie(time_1, traj_pos_1, traj_vel_1, traj_acc_1) = polynomial_piece_profile(std::make_tuple(a2, a1, a0), 0, t1, delta_t);

    // Second piece
    a0 = polyval(std::make_tuple(a2, a1, a0), t1);
    a1 = sign_traj * vel_c;
    a2 = 0;

    double time_2, traj_pos_2, traj_vel_2, traj_acc_2;
    std::tie(time_2, traj_pos_2, traj_vel_2, traj_acc_2) = polynomial_piece_profile(std::make_tuple(a2, a1, a0), t1, t2, delta_t);

    //Third piece
    a0 = polyval(std::make_tuple(a2, a1, a0), t2 - t1);
    a1 = sign_traj * vel_c;
    a2 = - 0.5 * sign_traj * m_acc_max;

    double time_3, traj_pos_3, traj_vel_3, traj_acc_3;
    std::tie(time_3, traj_pos_3, traj_vel_3, traj_acc_3) = polynomial_piece_profile(std::make_tuple(a2, a1, a0), t2, tf_sync, delta_t);

    double time_4 = tf_sync;
    double traj_pos_4 = final_pos;
    double traj_vel_4 = final_vel;
    double traj_acc_4 = 0.0;

    // Combine piecewise trajectory
    double_tuple_t time = std::make_tuple(time_1, time_2, time_3, time_4);
    
    double_tuple_t traj_pos = std::make_tuple(traj_pos_1, traj_pos_2, traj_pos_3, traj_pos_4);
        
    double_tuple_t traj_vel = std::make_tuple(traj_vel_1, traj_vel_2, traj_vel_3, traj_vel_4);
        
    double_tuple_t traj_acc = std::make_tuple(traj_acc_1, traj_acc_2, traj_acc_3, traj_acc_4);

    return std::make_tuple(time, traj_pos, traj_vel, traj_acc);
}

double_tuple_t Joint::polynomial_piece_profile(std::tuple<double,double,double>polynome, double start, double stop, double delta)
{
	/*
        Generate a polynomial piece profile
        Return time, position, velocity and acceleration discrete profile
    */ 
    if(stop < start)
    {
    	return std::make_tuple(-1, -1, -1, -1);
    }

    double a2,a1,a0;
    
    double pos = polyval(polynome, (stop-start)/2);

	std::tie(a2,a1,a0) = polynome;

    double vel = polyval(std::make_tuple(0, 2*a2, a1),(stop-start)/2);
    double acc = polyval(std::make_tuple(0, 0, 2*a2) ,(stop-start)/2);

    return std::make_tuple((stop-start)/2, pos, vel, acc);
}

std::tuple<double,double,double> Joint::time_to_destination(double initial_pos, double initial_vel, double final_pos, double final_vel)
{

    if(!trajectory_is_feasible(initial_pos, initial_vel, final_pos, final_vel))
    {
        return std::make_tuple(-1,-1,-1);
    }

    double delta_p = final_pos - initial_pos;

    if(delta_p == 0)
    {
        return std::make_tuple(0,0,0);
    }

    int sign_traj = trajectory_sign(initial_pos, initial_vel, final_pos, final_vel);

    double t_1 = (sign_traj * m_velocity_max - initial_vel) / (sign_traj * m_acc_max);

    double t_2 = (1 / m_velocity_max) * ((pow(final_vel,2) + pow(initial_vel,2 - 2 * sign_traj * initial_vel) / (2 * m_acc_max) + (sign_traj * delta_p)));

    double t_f = t_2 - (final_vel - sign_traj * m_velocity_max) / (sign_traj * m_acc_max);

    return std::make_tuple(t_1,t_2,t_f);
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

    double delta_p_crit = 0.5 * sign * (pow(final_vel,2) - pow(initial_vel,2))/ m_acc_max;

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