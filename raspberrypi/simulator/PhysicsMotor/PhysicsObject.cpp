#include "PhysicsObject.h"
#include <cmath>

PhysicsObject::PhysicsObject(){
    m_center = {0.,0.};
    m_theta = 0.0;
    m_velocity = {0.0,0.0};
    m_setvel= {0.0,0.0};
    m_maxacc ={100,100.0};
    m_minacc ={100.0,100.0};
    m_shape.length=1;  
    m_shape.list[0] = {0,0};
    m_shape_temp.length=1;  
    m_shape_temp.list[0] = {0,0};
    m_weight = 100;
    m_enable = true;
    m_collided =false;
    }


boost::python::tuple PhysicsObject::getVelocity(){
    return (m_velocity.to_python() ); 
}
void PhysicsObject::setVelocity(float lin,float ang){
    m_velocity.linear = lin;
    m_velocity.angular = ang;
}

void PhysicsObject::setWeight(float weight){
    m_weight  = weight;
}

void PhysicsObject::setVelSet(float lin,float ang){
    m_setvel.linear = lin;
    m_setvel.angular = ang;
}

void PhysicsObject::stop(){
    m_setvel.linear = m_velocity.linear  = 0;
    m_setvel.angular = m_velocity.angular = 0;
}

boost::python::tuple PhysicsObject::getPosition(){
    return (m_center.to_python()); 
}
void PhysicsObject::setPosition(float x,float y){
    m_center.x = x;
    m_center.y = y;
}

boost::python::tuple PhysicsObject::getPolygon(){
    return (m_shape_temp.to_python());
}

void PhysicsObject::setPolygon(boost::python::tuple poly){
    m_shape.from_python(poly);
    m_shape_temp = m_shape;

}

void PhysicsObject::setMaxAcceleration(float lin,float ang){
    m_maxacc.linear = lin;
    m_maxacc.angular = ang; 
}

void PhysicsObject::setMinAcceleration(float lin,float ang){
    m_minacc.linear = lin;
    m_minacc.angular = ang; 
}

boost::python::tuple PhysicsObject::getMaxAcceleration(){
    return m_maxacc.to_python();
}
boost::python::tuple PhysicsObject::getMinAcceleration(){
    return m_minacc.to_python();
}

void PhysicsObject::genShape(){
    int i;

    for(i=0;i<m_shape.length;i++){
        

        m_shape_temp.list[i].x =  m_shape.list[i].x*cosf(m_theta)  + -m_shape.list[i].y*sinf(m_theta); 
        m_shape_temp.list[i].y = m_shape.list[i].x*sinf(m_theta)  + m_shape.list[i].y*cosf(m_theta);
        m_shape_temp.list[i].x += m_center.x;
        m_shape_temp.list[i].y += m_center.y;
  
    }
}