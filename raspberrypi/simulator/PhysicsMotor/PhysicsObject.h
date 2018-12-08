#ifndef _PHYSICSOBJECT_H_
#define _PHYSICSOBJECT_H_

#include <boost/python.hpp>
#include <tuple>
#include <iostream>
#include "../common/PhysicsUtils.h"


class PhysicsObject
{
public:
    PhysicsObject();

    void setVelocity(float lin,float ang);
    void setVelSet(float lin,float ang);
    void setPosition(float x,float y);
    void stop();
    void setTheta(float t){m_theta = t;}
    void setPolygon(boost::python::tuple poly);
    void setMaxAcceleration(float lin,float ang);
    void setMinAcceleration(float lin,float ang);
    void setWeight(float weight);
    void enable(){m_enable=true;}
    void disable(){m_enable=false;}


    boost::python::tuple getMaxAcceleration();
    boost::python::tuple getMinAcceleration();
    boost::python::tuple getVelocity();
    float                getTheta(){return m_theta;}
    float                getWeight(){return m_theta;}

    Polygon getShape(){return m_shape_temp;}
    boost::python::tuple getPosition();
    boost::python::tuple getPolygon();

    bool isCollided(){return m_collided;}

    void genShape();
    long int getLastCall(){return m_lastcall;}
    void setTime(long int time){m_lastcall = time;}


    long int      m_lastcall;
    float         m_weight;
    Polygon       m_shape;
    Polygon       m_shape_temp;
    Point         m_center;
    float         m_theta;
    Velocity      m_velocity;
    Velocity      m_setvel;
    Acceleration  m_maxacc;
    Acceleration  m_minacc;
    bool          m_enable;
    bool          m_collided;

};

#endif //_PHYSICSOBJECT_H_