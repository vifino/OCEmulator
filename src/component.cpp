#include "component.h"

Component::Component() : addressu(boost::uuids::random_generator()())
{
    std::stringstream ss;
    ss << addressu;
    address = ss.str();
}


Component::Component(std::string addr)
{
    address = addr;
}
