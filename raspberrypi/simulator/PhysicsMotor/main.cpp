
#include <boost/python.hpp>
#include "PhysicsMotor.h"
#include "../common/PeriodicProcess.h"

BOOST_PYTHON_MODULE(Physics)
{
    using namespace boost::python;
    class_<PhysicsMotor>("PhysicsMotor")
    .def(init<>())
    // PeriodicProcess Method
    .def("start", &PeriodicProcess::start)
    .def("stop",&PeriodicProcess::stop)
    .def("setTimestep",&PeriodicProcess::setTimestep)
    .def("getTimestep",&PeriodicProcess::getTimestep)

    //Object Manager
    .def("addObject",&PhysicsMotor::addObject)
    .def("removeObject",&PhysicsMotor::removeObject)
    .def("getObject"  ,&PhysicsMotor::getObject)
    .def("enableObject"     ,&PhysicsMotor::enableObject)
    .def("disableObject"    ,&PhysicsMotor::disableObject)

    //Objects getter
    .def("getPosition",&PhysicsMotor::getPosition)
    .def("getTheta",&PhysicsMotor::getTheta)
    .def("getVelocity",&PhysicsMotor::getVelocity)
    .def("getMaxAcceleration",&PhysicsMotor::getMaxAcceleration)
    .def("getMinAcceleration",&PhysicsMotor::getMinAcceleration)
    .def("getWeight",&PhysicsMotor::getWeight)
    .def("getShape",&PhysicsMotor::getShape)
    .def("isCollided",&PhysicsMotor::isCollided)
    
    //Object setter
    .def("setPosition",&PhysicsMotor::setPosition)
    .def("setTheta",&PhysicsMotor::setTheta)
    .def("setShape",&PhysicsMotor::setShape)
    .def("setWeight",&PhysicsMotor::setWeight)
    .def("setVelocity",&PhysicsMotor::setVelocity)
    .def("setMaxAcceleration",&PhysicsMotor::setMaxAcceleration)
    .def("setMinAcceleration",&PhysicsMotor::setMinAcceleration)

    //Map Manager
    .def("getMap",&PhysicsMotor::getMap)
    .def("setMap",&PhysicsMotor::setMap);

}
