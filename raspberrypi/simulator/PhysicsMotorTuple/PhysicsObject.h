#ifndef _GEOMETRY_TYPE_H_
#define _GEOMETRY_TYPE_H_


#include <boost/python.hpp>
#include <tuple>
#include <iostream>
#include "../common/PhysicsUtils.h"

class PhysicsObject
{
public:
    //PhysicsObject(Point p,Polygon shape) : m_center(p) , m_shape(shape) ,m_velocity({0.0,0.0}), m_acceleration({0.0,0.0}) {}
    PhysicsObject();
    void setVelocity(float lin,float ang);
    boost::python::tuple getVelocity();
    void setPosition(int x,int y);
    boost::python::tuple getPosition();
    boost::python::tuple getPolygon();
    void setPolygon(boost::python::tuple poly);


private:
    Polygon      m_shape;
    Point         m_center;
    Velocity      m_velocity;
    Velocity      m_setvel;
    Acceleration  m_acceleration;

    //friend PhysicsMotor;
};

#endif //_GEOMETRY_TYPE_H_