#ifndef COMPINSTANCE_H
#define COMPINSTANCE_H

#include <iostream>
#include <fstream>
#include <lua.hpp>

class CompInstance
{
public:
    CompInstance();
private:
    lua_State *state;
    lua_State *thread;
};

#endif // COMPINSTANCE_H
