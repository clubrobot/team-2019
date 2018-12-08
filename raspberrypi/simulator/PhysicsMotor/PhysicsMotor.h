#ifndef _PHYSICS_MOTOR_H_
#define _PHYSICS_MOTOR_H_


#include "../common/PhysicsUtils.h"
#include "../common/PeriodicProcess.h"
#include "PhysicsObject.h"
#include <iostream>
#include <map>
#include <vector>
#include <boost/python.hpp>


#define MAX_OBJECT  64
 	

class PhysicsMotor : public PeriodicProcess
{
public:


    //Constructor
    PhysicsMotor() : PeriodicProcess(10),  m_map({0}) {}


    // Map access
    void setMap(boost::python::tuple m){m_map.from_python(m);}
    boost::python::tuple getMap(){return m_map.to_python();}


    // Object access
    void addObject(std::string name);
    void removeObject(std::string name);
    std::string getObject(float x,float y,float threshold);
    void enableObject(std::string name);
    void disableObject(std::string name);

    void setShape(std::string name,boost::python::tuple poly);
    void setPosition(std::string name,float x,float y);
    void setTheta(std::string name,float t);
    void setWeight(std::string name,float weight);
    void setVelocity(std::string name,float lin,float vel,bool forced = true);
    void setMaxAcceleration(std::string name,float lin,float vel);
    void setMinAcceleration(std::string name,float lin,float vel);


    boost::python::tuple getShape(std::string name);
    boost::python::tuple getPosition(std::string name);
    float                getTheta(std::string name);
    float                getWeight(std::string name);
    boost::python::tuple getVelocity(std::string name);
    boost::python::tuple getMaxAcceleration(std::string name);
    boost::python::tuple getMinAcceleration(std::string name);
    bool                 isCollided(std::string name);                





protected:
     // Computing method
    virtual void process();
    bool collide(std::string name,PhysicsObject  * main, std::vector<std::string> ban_list);
    void move(PhysicsObject& p);



protected:

private:



    Polygon m_map;
    std::map<std::string,PhysicsObject> m_objects;




};

#endif // _PHYSICS_MOTOR_H_
