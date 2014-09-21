#ifndef COMPINSTANCE_H
#define COMPINSTANCE_H

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include "component.h"
#include "filesystemcomponent.h"
#include "lua.hpp"
#include <cstdio>

#include <QTime>

#include <unicode/ustring.h>
#include <unicode/unistr.h>

#include <boost/bind.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/filesystem.hpp>
#include <boost/shared_ptr.hpp>

typedef boost::shared_ptr<Component> CompPtr;

class CompInstance
{
public:
    CompInstance(std::string ipath);
    std::string address;
    std::vector<CompPtr> components;
protected:
    std::string bootAddress;
    std::string path;
    int usedMemory;
    int maxMemory;

    QTime startTime;
private:
    lua_State *state;
    lua_State *thread;

    boost::uuids::uuid addressu;

    static int componentList(lua_State *L);
    static int methodsList(lua_State *L);
    static int componentInvoke(lua_State *L);
    static int componentType(lua_State *L);

    static int onAddress(lua_State *L);
    static int getBootAddress(lua_State *L);
    static int setBootAddress(lua_State *L);
    static int getRealTime(lua_State *L);
    static int getFreeMemory(lua_State *L);
    static int getUptime(lua_State *L);

    static int allowBytecode(lua_State *L);
    static int getTimeout(lua_State *L);

    static int uniChar(lua_State *L);
    static int uniUpper(lua_State *L);
    static int uniLower(lua_State *L);
    static int uniReverse(lua_State *L);
    static int uniLen(lua_State *L);
    static int uniSub(lua_State *L);

    static void *l_alloc_restricted (void *ud, void *ptr, size_t osize, size_t nsize);
};

#endif // COMPINSTANCE_H
