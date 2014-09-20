#ifndef COMPONENT_H
#define COMPONENT_H

#include <string>
#include <sstream>
#include "lua.hpp"

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/filesystem.hpp>

class Component
{
public:
    Component();
    Component(std::string addr);
    virtual int listMethods(lua_State *L) =0;
    virtual int onInvoke(lua_State *L) =0;
    virtual std::string getName() =0;
    std::string address;
private:
    boost::uuids::uuid addressu;
};

#endif // COMPONENT_H
