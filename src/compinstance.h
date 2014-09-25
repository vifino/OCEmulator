#ifndef COMPINSTANCE_H
#define COMPINSTANCE_H

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <queue>
#include <cstdio>
class CompInstance;

struct signalPar
{
    unsigned char type;
    void *data;
};

typedef std::vector<signalPar> signal;

signalPar strToPar(std::string str);
signalPar intToPar(int num);
signalPar boolToPar(int num);
signalPar nilToPar();

#include <boost/bind.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/filesystem.hpp>
#include <boost/shared_ptr.hpp>

#include "component.h"
typedef boost::shared_ptr<Component> CompPtr;
#include "filesystemcomponent.h"
#include "screencomponent.h"
#include "componentgpu.h"

#include "lua.hpp"

#include <QTime>
#include <QTimer>
#include <QTimerEvent>

#include <unicode/ustring.h>
#include <unicode/unistr.h>
class CompInstance : QObject
{
public:
    CompInstance(std::string ipath);
    std::string address;
    std::vector<CompPtr> components;

    std::queue<signal> sigs; // 'signals' is reserved
    void tryResume(int timer = 0);
protected:
    std::string bootAddress;
    std::string path;
    int usedMemory;
    int maxMemory;

    QTime startTime;
    int timerId;

    void timerEvent(QTimerEvent *event);
    void resumeThread(int args, int timer = 0);
private:
    lua_State *state;
    lua_State *thread;
    boost::uuids::uuid addressu;

    void onTimer();

    static int componentList(lua_State *L);
    static int methodsList(lua_State *L);
    static int componentInvoke(lua_State *L);
    static int componentType(lua_State *L);

    static int onAddress(lua_State *L);
    static int getBootAddress(lua_State *L);
    static int setBootAddress(lua_State *L);
    static int getRealTime(lua_State *L);
    static int getFreeMemory(lua_State *L);
    static int getTotalMemory(lua_State *L);
    static int getUptime(lua_State *L);
    static int getTmpAddress(lua_State *L);
    static int pushSignal(lua_State *L);

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
