
#include <boost/python.hpp>
#include "PhysicsObject.h"


BOOST_PYTHON_MODULE(Physics)
{
    using namespace boost::python;
    class_<PhysicsObject>("PhysicsObject")
    .def(init<>())
    .def("setVelocity", &PhysicsObject::setVelocity)
    .def("getVelocity", &PhysicsObject::getVelocity)
    .def("getPosition", &PhysicsObject::getPosition)
    .def("setPosition", &PhysicsObject::setPosition)
    .def("getPolygon" , &PhysicsObject::getPolygon)
    .def("setPolygon" , &PhysicsObject::setPolygon);
}