#include "PhysicsObject.h"

PhysicsObject::PhysicsObject(){
m_center = {0,0};
 m_velocity = {0.0,0.0};
 m_setvel= {0.0,0.0};
 m_acceleration ={0.0,0.0};
        m_shape.length=2;
        m_shape.list[0] = {0,0};
        m_shape.list[1] = {1,1};
    }


boost::python::tuple PhysicsObject::getVelocity(){
    return (boost::python::make_tuple(m_velocity.linear,m_velocity.angular)); 
}
void PhysicsObject::setVelocity(float lin,float ang){
    m_velocity.linear = lin;
    m_velocity.angular = ang;
}

boost::python::tuple PhysicsObject::getPosition(){
    return (boost::python::make_tuple(m_center.x,m_center.y)); 
}
void PhysicsObject::setPosition(int x,int y){
    m_center.x = x;
    m_center.y = y;
}

boost::python::tuple PhysicsObject::getPolygon(){
    return (m_shape.to_python());
}

void PhysicsObject::setPolygon(boost::python::tuple poly){

    /*
    std::tuple<boost::python::tuple> pol = boost::python::extract<boost::python::tuple>(poly);
    int i;
    for( i = std::tuple_size<decltype(pol)>::value; i>0;i--){
        m_shape.list[i].x = boost::python::extract<int>(std::get<i>(pol)[0]);
        m_shape.list[i].x = boost::python::extract<int>(std::get<i>(pol)[1]);
    }
    */
    m_shape.from_python(poly);
    
}