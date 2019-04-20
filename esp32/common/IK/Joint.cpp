#include <Arduino.h>
#include "Joint.h"

#ifdef IK_LOG
    #define LOG_JOINT(arg) cout << __TIME__<< " (JOINT)("<< __func__ << " , " << __LINE__ << ")\t\t\t: "<< arg << endl; 
#else
    #define LOG_JOINT(arg) 
#endif

namespace IK
{

Joint::Joint(int id, float pos_min, float pos_max, float velociy_min, float velociy_max, float acc_min, float acc_max) throw()
{
	_id = id;

	// constraints 
	_constraints.pos.min = pos_min;
    _constraints.pos.max = pos_max;

    _constraints.vel.min = velociy_min;
    _constraints.vel.max = velociy_max;

    _constraints.acc.min = acc_min;
    _constraints.acc.max = acc_max;
}

float Joint::polyval(Polynom polynome, float x) throw()
{
    float ret;

	ret =  polynome.a2 * pow(x,2) + polynome.a1 * x + polynome.a0;

    return ret;
}

Polynom Joint::polyder(Polynom poly) throw()
{
    Polynom dp;

    dp.a0 = poly.a1;
    dp.a1 = 2*poly.a2;
    dp.a2 = 0;

    return dp;
}

vector<float> Joint::vector_polyval(Polynom polynome, vector<float> x) throw()
{
    vector<float> values;
    int size = x.size();
    for(int i = 0; i<size; i++)
        values.push_back(polynome.a2*pow(x.at(i),2) + polynome.a1 * x.at(i) + polynome.a0);

    return values;
}

template<typename T>vector<T> Joint::arange(T start, T stop, T step) throw()
{
    vector<T> values;
    for (T value = start; value < stop; value += step)
        values.push_back(value);

    return values;
}

bool Joint::trajectory_is_feasible(float initial_pos, float initial_vel, float final_pos, float final_vel)
{
	// checks boundaries to determine feasibility
	if(final_pos > (_constraints.pos.max + EPSILON) || final_pos < (_constraints.pos.min - EPSILON))
	{
        LOG_JOINT("Target position unreachable by joint");
        throw string("Target position unreachable by joint");
		return false;
	}

	if(final_vel > (_constraints.vel.max + EPSILON) || final_vel < (_constraints.vel.min - EPSILON))
	{
        LOG_JOINT("Target velocity unreachable by joint");
        throw string("Target velocity unreachable by joint");
		return false;
	}

	float delta_p_dec = 0.5 * final_vel * abs(final_vel) / _constraints.acc.max;

	if((final_pos + delta_p_dec) > (_constraints.pos.max + EPSILON) || (final_pos + delta_p_dec) < (_constraints.pos.min - EPSILON))
	{
        LOG_JOINT("Target position unreachable at specified velocity by joint");
        throw string("Target position unreachable at specified velocity by joint");

		return false;
	}

    return true;
}

vector_t Joint::get_path(float initial_pos, float initial_vel , float final_pos, float final_vel, float tf_sync, float delta_t) throw()
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

    vector_t ret ;

    // Compute limit time
    float delta_p = final_pos - initial_pos;
    int sign_traj = trajectory_sign(initial_pos, initial_vel, final_pos, final_vel);

    float vel_c;

    if( (std::abs(initial_vel) < EPSILON) || (std::abs(final_vel) < EPSILON) )
    {
        vel_c = EPSILON; //ensure we use a trapezoidal trajectory
    }
    else if(std::abs(initial_vel) < std::abs(final_vel))
    {
        vel_c = final_vel;
    }
    else
    {
        vel_c = initial_vel;
    }

    float tf_lim = (delta_p / vel_c) \
                    + (0.5 * sign_traj * (vel_c - final_vel) / _constraints.acc.max)\
                    + (0.5 * sign_traj * (initial_vel - vel_c) / _constraints.acc.max);

    //Determine shape of trajectory
    if( (tf_sync < tf_lim) || (initial_vel == 0 && final_vel == 0))
    {
        ret = trapezoidal_profile(initial_pos, initial_vel, final_pos, final_vel, tf_sync, tf_lim, delta_t);
    }
    else
    {
        ret = floatramp_profile(initial_pos, initial_vel, final_pos, final_vel, tf_sync, tf_lim, delta_t);
    }

    return ret;
}

vector_t Joint::trapezoidal_profile(float initial_pos, float initial_vel , float final_pos, float final_vel, float tf_sync,float tf_lim, float delta_t) throw()
{
	/*
        Generate a trapezoidal profile to reach target
    */

    vector_t ret;

    //Compute cruise speed
    float delta_p = final_pos - initial_pos;

    int sign_traj = trajectory_sign(initial_pos, initial_vel, final_pos, final_vel);
    
    //Avoid division by 0
    if(sign_traj == 0)
    {
        sign_traj = 1;
    }

    float b = (_constraints.acc.max * tf_sync) + (sign_traj * initial_vel);

    float vel_c = 0.5 * ( b - sqrt( pow(b,2) \
                    - 4 * sign_traj * _constraints.acc.max * delta_p \
                    - 2 * pow((initial_vel - final_vel),2) ) );

    ret = generic_profile(initial_pos, initial_vel, final_pos, final_vel, tf_sync, tf_lim, delta_t, sign_traj, 1, vel_c);

    return ret;
}

vector_t Joint::floatramp_profile(float initial_pos, float initial_vel , float final_pos, float final_vel, float tf_sync,float tf_lim, float delta_t) throw()
{
	/*
        Generate a float ramp profile to reach target
    */
    vector_t ret;

    // Compute cruise speed     
    float delta_p = final_pos - initial_pos;

    int sign_traj = trajectory_sign(initial_pos, initial_vel, final_pos, final_vel);

    // Avoid division by 0
    if( sign_traj == 0)
    {
        sign_traj = 1;
    }   

   	float vel_c = (sign_traj * delta_p - 0.5 * (pow((initial_vel - final_vel),2) / _constraints.acc.max)) / (tf_sync - ((initial_vel  - final_vel) / (sign_traj * _constraints.acc.max)));

    ret = generic_profile(initial_pos, initial_vel, final_pos, final_vel, tf_sync, tf_lim, delta_t, sign_traj, 1, vel_c);


    return ret;
}

vector_t Joint::generic_profile(float initial_pos, float initial_vel, float final_pos, float final_vel, float tf_sync, float tf_lim,float delta_t, int sign_traj, int sign_sync, float vel_c) throw()
{
    /*
        Generate a generic profile (valid for trapezoidal and float ramp)
    */

    float t1 = (sign_traj * (vel_c - initial_vel)) / (sign_traj * _constraints.acc.max);
    float t2 = tf_sync - std::abs(vel_c - final_vel) / _constraints.acc.max;

    Polynom poly;

    // First piece
    poly.a0 = initial_pos;
    poly.a1 = initial_vel;
    poly.a2 = 0.5 * sign_traj * sign_sync * _constraints.acc.max;

    vector_t traj1 = polynomial_piece_profile(poly, 0, t1, delta_t);

    // Second piece
    poly.a0 = polyval(poly, t1);
    poly.a1 = sign_traj * vel_c;
    poly.a2 = 0;

    vector_t traj2 = polynomial_piece_profile(poly, t1, t2, delta_t);

    //Third piece
    poly.a0 = polyval(poly, t2 - t1);
    poly.a1 = sign_traj * vel_c;
    poly.a2 = - 0.5 * sign_traj * _constraints.acc.max;

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

vector_t Joint::polynomial_piece_profile(Polynom polynome, float start, float stop, float delta)
{
    /*
        Generate a polynomial piece profile
        Return time, position, velocity and acceleration discrete profile
    */

    vector_t piece;

    if (stop < start)
    { 
        LOG_JOINT("Non causal trajectory profile requested");
        throw string("Non causal trajectory profile requested");
    }
        
    Polynom polynome_dot = polyder(polynome);
    Polynom polynome_dot_dot = polyder(polynome_dot);

    vector<float> time = arange<float>(start, stop, delta);
    vector<float> dtime = arange<float>(0, stop-start, delta);

    piece.t = time;
    piece.pos = vector_polyval(polynome, dtime);
    piece.vel = vector_polyval(polynome_dot, dtime);
    piece.acc = vector_polyval(polynome_dot_dot, dtime);

    return piece;
}

TrajectoryTime Joint::time_to_destination(float initial_pos, float initial_vel, float final_pos, float final_vel)
{
    TrajectoryTime time_traj;

    if(!trajectory_is_feasible(initial_pos, initial_vel, final_pos, final_vel))
    {
        LOG_JOINT("Trajectory is not feasibile");
        throw string("Trajectory is not feasibile");
    }

    float delta_p = final_pos - initial_pos;

    if(delta_p == 0)
    {
        time_traj.t1 = 0;
        time_traj.t2 = 0;
        time_traj.tf = 0;

        return time_traj;
    }

    int sign_traj = trajectory_sign(initial_pos, initial_vel, final_pos, final_vel);

    time_traj.t1 = (sign_traj * _constraints.vel.max - initial_vel) / (sign_traj * _constraints.acc.max);

    time_traj.t2 = (1 / _constraints.vel.max) * ((pow(final_vel,2) + pow(initial_vel,2) - 2 * sign_traj * initial_vel) / (2 * _constraints.acc.max) + (sign_traj * delta_p));

    time_traj.tf = time_traj.t2 - (final_vel - sign_traj * _constraints.vel.max) / (sign_traj * _constraints.acc.max);

    return time_traj;
}

int Joint::trajectory_sign(float initial_pos, float initial_vel, float final_pos, float final_vel) throw()
{
	/*
        Get sign of trajectory to be executed
    */ 

    float delta_p = final_pos - initial_pos;
    float delta_v = final_vel - initial_vel;

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

    float delta_p_crit = 0.5 * sign * (pow(final_vel,2) - pow(initial_vel,2))/ _constraints.acc.max;

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

/****** DEBUG *****/
template<typename T>
ostream& operator<< (ostream& out, const vector<T>& v) {
    out << "{";
    size_t last = v.size() - 1;
    for(size_t i = 0; i < v.size(); ++i) {
        out << v[i];
        if (i != last) 
            out << ", ";
    }
    out << "}";
    return out;
}
ostream& operator<< (ostream& out, const vector_t& v)
{
    out << "Vector_t : "<< endl;
    out << " t:";
    out << v.t<< endl;
    out << " pos:";
    out << v.pos<< endl;
    out << " vel:";
    out << v.vel<< endl;
    
    return out;
}

ostream& operator<< (ostream& out, const TrajectoryTime& t)
{
    out << "Trejectory time : " << endl;
    out << "t1: "<< t.t1 << " t2 : "<< t.t2 << " tf: "<< t.tf;
    return out;
}

} /* end of namespace */