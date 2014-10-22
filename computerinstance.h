#ifndef COMPUTERINSTANCE_H
#define COMPUTERINSTANCE_H

#include <memory>
#include <iostream>
#include <sstream>
#include <vector>

class ComputerInstance;

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

#include <gtkmm.h>
#include <time.h>
#include <wchar.h>
#include "gui/screenswidget.h"

#include <lua.hpp>
#include <stdlib.h>
#include "components/componentbase.h"

#define cIterator std::vector<std::shared_ptr<ComponentBase> >::iterator

#include "components/componentfilesystem.h"
#include "components/componentscreen.h"
#include "components/componentgpu.h"
#include "components/componentkeyboard.h"
#include "eventhandler.h"

class ComputerInstance {
public:
    ComputerInstance(std::string path, std::string name, ScreensWidget &screensW);

    ~ComputerInstance();

    size_t getUsedMemory();

    size_t getMaxMemory();

    void setUsedMemory(size_t memory);

    void setMaxMemory(size_t memory);

    std::vector<std::shared_ptr<ComponentBase> > components;
    ScreensWidget *screenW;
private:
    lua_State *state;
    lua_State *thread;
    size_t maxMemory;
    size_t usedMemory;
    int energy;
    int maxEnergy;
    int uptime;
    bool robot;
    std::string bootAddress;
    std::vector<std::string> users; // Users have no effect

    timespec startTime;

    std::string uuid;
    std::string basePath;

    // Component API
    static int listComponents(lua_State *state);

    static int componentDocs(lua_State *state);

    static int componentInvoke(lua_State *state);

    static int listMethods(lua_State *state);

    static int componentType(lua_State *state);

    static int componentSlot(lua_State *state);

    // Computer API
    static int getComputerAddress(lua_State *state);

    static int isRobot(lua_State *state);

    static int getTmpAddress(lua_State *state);

    static int getFreeMemory(lua_State *state);

    static int getTotalMemory(lua_State *state);

    static int getUptime(lua_State *state);

    static int getRealTime(lua_State *state);

    static int getEnergy(lua_State *state);

    static int getMaxEnergy(lua_State *state);

    static int getBootAddress(lua_State *state);

    static int setBootAddress(lua_State *state);

    static int addUser(lua_State *state);

    static int removeUser(lua_State *state);

    static int pushSignal(lua_State *state);

    // System API
    static int getAllowBytecode(lua_State *state);

    static int getTimeout(lua_State *state);

    static int getMaxPatternLength(lua_State *state);

    // Unicode API
    static int unicodeChar(lua_State *state);

    static int unicodeLength(lua_State *state);

    static int unicodeLower(lua_State *state);

    static int unicodeReverse(lua_State *state);

    static int unicodeSub(lua_State *state);

    static int unicodeUpper(lua_State *state);

    static int unicodeIsWide(lua_State *state);

    static int unicodeCharWidth(lua_State *state);

    static int unicodeWLength(lua_State *state);

    static int unicodeWTrunc(lua_State *state);

    Glib::Threads::Thread *eventThread;
    EventHandler eventHandler;
};

#endif // COMPUTERINSTANCE_H
