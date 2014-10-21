#ifndef COMPONENTBASE_H
#define COMPONENTBASE_H

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <lua.hpp>
#include <vector>
#include <sstream>
#include <iostream>

class ComponentBase {
public:
    ComponentBase(int nSlot);

    ~ComponentBase();

    virtual int listMethods(lua_State *state) = 0;

    virtual int getDocumentation(lua_State *state) = 0;

    virtual int invokeMethod(lua_State *state) = 0;

    std::string getUUID();

    int getSlot();

    virtual std::string getType() = 0;

protected:
    std::string uuid;

    static void pushMethod(lua_State *state, std::string name, bool getter, bool setter, bool direct);

    int slot;
};

#endif // COMPONENTBASE_H
