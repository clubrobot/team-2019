#include <Arduino.h>
#include <math.h>
#include <cmath>
#include <tuple>
#include <iostream>
#include <string>
#include <stdexcept>
#include "Joint.h"
#include "Scara.h"

Scara::Scara(double l1, double l2, coords_t joints, coords_t origin)
{
	m_joints 	= joints;
	m_origin	= origin;
	m_tool	 	= get_tool();

	m_l1		= l1;
	m_l2		= l2;
	m_lsq   	= pow(m_l2,2)+pow(m_l1,2);
}

coords_t Scara::forward_kinematics(coords_t joints)
{
	m_joints = joints;
	return get_tool();
}

coords_t Scara::inverse_kinematics(coords_t tool)
{
	double x, y ,last_x, last_y;

	std::tie(x,y) = tool;
	std::tie(last_x, last_y) = m_tool;


	double norm = pow((x - last_x),2) + pow((y - last_y),2);

    if( norm > pow((m_l1 + m_l2),2) || norm < pow((m_l1 - m_l2),2))
    {
    	//Target unreachable
        m_tool = get_tool();
        return std::make_tuple(-1,-1);
    }
            

    m_tool = tool;
    m_joints = get_joints();

    return m_joints;
}

coords_t Scara::get_tool(void)
{
	double theta1,theta2;
	double x_origin,y_origin;

	std::tie(theta1,theta2) = m_joints;
	std::tie(x_origin,y_origin) = m_origin;

    double x = (m_l1 * cos(theta1) + m_l2 * cos(theta1 + theta2));
    double y = m_l1 * sin(theta1) + m_l2 * sin(theta1 + theta2);

    x += x_origin;
    y += y_origin;

    return std::make_tuple(x,y);
}

coords_t Scara::get_joints(void)
{
	double x_tool,y_tool,x_origin,y_origin,x,y,theta1,theta2;

	std::tie(x_tool,y_tool) 	= m_tool;
	std::tie(x_origin,y_origin) = m_origin;
	std::tie(theta1,theta2) 	= m_joints;

	x = x_tool - x_origin;
    y = y_tool - y_origin;

    if(x == 0 and y == 0)
    {
    	return std::make_tuple(theta1, M_PI);
    }
        

    double l = pow(x,2) + pow(y,2);
    
   	double lsq = m_lsq;

    double cos_gamma = (l + pow(m_l1,2) - pow(m_l2,2)) / (2 * m_l1 * sqrt(l));

    double gamma;
    //Numerical errors can make abs(cos_gamma) > 1
    if(cos_gamma > 1 - EPSILON || cos_gamma < -1 + EPSILON)
    {
        gamma = 0.0;
    }
    else
    {
        gamma = acos(cos_gamma);
    }

    theta1 = atan2(y, x) - gamma;
    theta2 = atan2(sqrt(1 - ((l - lsq) / pow((2 * m_l1 * m_l2),2))), (l - lsq) / (2 * m_l1 * m_l2));

    theta1 = std::fmod((theta1 + M_PI) ,(2 * M_PI)) - M_PI; // Stay between -pi and pi
    theta2 = std::fmod((theta2 + M_PI) , (2 * M_PI)) - M_PI; // Stay between -pi and pi

    return std::make_tuple(theta1,theta2);
}