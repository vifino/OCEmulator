#include "computerinstance.h"

static void *allocc(void *ud, void *ptr, size_t osize, size_t nsize) {
    ComputerInstance *comp = (ComputerInstance *) ud;

    if (nsize == 0) {
        comp->setUsedMemory(comp->getUsedMemory() - osize);
        free(ptr);
        return NULL;
    }
    else {
        if (comp->getUsedMemory() + (nsize - osize) > comp->getMaxMemory()) {
            std::cout << "Too much memories" << std::endl;
            free(ptr);
            return NULL;
        }
        ptr = realloc(ptr, nsize);
        if (ptr)
            comp->setUsedMemory(comp->getUsedMemory() + nsize - osize);
        return ptr;
    }
}

ComputerInstance::ComputerInstance(std::string path, std::string name, ScreensWidget &screensW) {
    screenW = &screensW;
    basePath = path + "/instances/" + name;
    boost::uuids::uuid uuidd = boost::uuids::random_generator()();
    std::stringstream uuidStream;
    uuidStream << uuidd;
    uuid = uuidStream.str();

    maxMemory = 2048 * 1024;
    usedMemory = 0;
    energy = 1000;
    maxEnergy = 1000;
    robot = false;
    uptime = 0;

    boost::filesystem::create_directories(basePath);
    if (!boost::filesystem::exists(basePath + "/kernel.lua")) {
        boost::filesystem::copy(path + "/kernel.lua", basePath + "/kernel.lua");
    }
    std::shared_ptr<ComponentBase> fileC = std::dynamic_pointer_cast<ComponentBase>(std::shared_ptr<ComponentFilesystem>(new ComponentFilesystem(1, basePath + "/filesystems")));
    std::shared_ptr<ComponentBase> screenC = std::dynamic_pointer_cast<ComponentBase>(std::shared_ptr<ComponentScreen>(new ComponentScreen(2)));
    std::shared_ptr<ComponentBase> gpuC = std::dynamic_pointer_cast<ComponentBase>(std::shared_ptr<ComponentGPU>(new ComponentGPU(3, this)));
    std::shared_ptr<ComponentBase> kbC = std::dynamic_pointer_cast<ComponentBase>(std::shared_ptr<ComponentKeyboard>(new ComponentKeyboard(3, &eventHandler)));
    screenW->addScreen(std::dynamic_pointer_cast<ComponentScreen>(screenC));
    std::dynamic_pointer_cast<ComponentScreen>(screenC)->addKeyboard(std::dynamic_pointer_cast<ComponentKeyboard>(kbC));
    components.push_back(fileC);
    components.push_back(screenC);
    components.push_back(gpuC);
    components.push_back(kbC);

    // state = lua_newstate(allocc, this);
    state = luaL_newstate();
    // Initialize base APIs

    luaL_requiref(state, "base", luaopen_base, 0);
    lua_pop(state, 1);
    luaL_requiref(state, "math", luaopen_math, 1);
    lua_pop(state, 1);
    luaL_requiref(state, "table", luaopen_table, 1);
    lua_pop(state, 1);
    luaL_requiref(state, "string", luaopen_string, 1);
    lua_pop(state, 1);
    luaL_requiref(state, "coroutine", luaopen_coroutine, 1);
    lua_pop(state, 1);
    luaL_requiref(state, "debug", luaopen_debug, 1);
    lua_pop(state, 1);
    luaL_requiref(state, "bit32", luaopen_bit32, 1);
    lua_pop(state, 1);
    luaL_requiref(state, "os", luaopen_bit32, 1); // TODO: Clear uneccesary stuff
    lua_pop(state, 1);

    // Component API
    lua_newtable(state);
    lua_pushlightuserdata(state, this);
    lua_pushcclosure(state, &ComputerInstance::listComponents, 1);
    lua_setfield(state, -2, "list");

    lua_pushlightuserdata(state, this);
    lua_pushcclosure(state, &ComputerInstance::componentDocs, 1);
    lua_setfield(state, -2, "doc");

    lua_pushlightuserdata(state, this);
    lua_pushcclosure(state, &ComputerInstance::componentInvoke, 1);
    lua_setfield(state, -2, "invoke");

    lua_pushlightuserdata(state, this);
    lua_pushcclosure(state, &ComputerInstance::listMethods, 1);
    lua_setfield(state, -2, "methods");

    lua_pushlightuserdata(state, this);
    lua_pushcclosure(state, &ComputerInstance::componentSlot, 1);
    lua_setfield(state, -2, "slot");

    lua_pushlightuserdata(state, this);
    lua_pushcclosure(state, &ComputerInstance::componentType, 1);
    lua_setfield(state, -2, "type");

    lua_setglobal(state, "component");

    //Computer API
    lua_newtable(state);
    lua_pushlightuserdata(state, this);
    lua_pushcclosure(state, &ComputerInstance::isRobot, 1);
    lua_setfield(state, -2, "isRobot");

    lua_pushlightuserdata(state, this);
    lua_pushcclosure(state, &ComputerInstance::getComputerAddress, 1);
    lua_setfield(state, -2, "address");

    lua_pushlightuserdata(state, this);
    lua_pushcclosure(state, &ComputerInstance::getTmpAddress, 1);
    lua_setfield(state, -2, "tmpAddress");

    lua_pushlightuserdata(state, this);
    lua_pushcclosure(state, &ComputerInstance::getFreeMemory, 1);
    lua_setfield(state, -2, "freeMemory");

    lua_pushlightuserdata(state, this);
    lua_pushcclosure(state, &ComputerInstance::getTotalMemory, 1);
    lua_setfield(state, -2, "totalMemory");

    lua_pushlightuserdata(state, this);
    lua_pushcclosure(state, &ComputerInstance::getUptime, 1);
    lua_setfield(state, -2, "uptime");

    lua_pushlightuserdata(state, this);
    lua_pushcclosure(state, &ComputerInstance::getRealTime, 1);
    lua_setfield(state, -2, "realTime");

    lua_pushlightuserdata(state, this);
    lua_pushcclosure(state, &ComputerInstance::getEnergy, 1);
    lua_setfield(state, -2, "energy");

    lua_pushlightuserdata(state, this);
    lua_pushcclosure(state, &ComputerInstance::getMaxEnergy, 1);
    lua_setfield(state, -2, "maxEnergy");

    lua_pushlightuserdata(state, this);
    lua_pushcclosure(state, &ComputerInstance::getBootAddress, 1);
    lua_setfield(state, -2, "getBootAddress");

    lua_pushlightuserdata(state, this);
    lua_pushcclosure(state, &ComputerInstance::setBootAddress, 1);
    lua_setfield(state, -2, "setBootAddress");

    lua_pushlightuserdata(state, this);
    lua_pushcclosure(state, &ComputerInstance::addUser, 1);
    lua_setfield(state, -2, "addUser");

    lua_pushlightuserdata(state, this);
    lua_pushcclosure(state, &ComputerInstance::removeUser, 1);
    lua_setfield(state, -2, "removeUser");

    lua_pushlightuserdata(state, &eventHandler);
    lua_pushcclosure(state, &ComputerInstance::pushSignal, 1);
    lua_setfield(state, -2, "pushSignal");

    lua_setglobal(state, "computer");

    // System API
    lua_newtable(state);
    lua_pushlightuserdata(state, this);
    lua_pushcclosure(state, &ComputerInstance::getAllowBytecode, 1);
    lua_setfield(state, -2, "allowBytecode");

    lua_pushlightuserdata(state, this);
    lua_pushcclosure(state, &ComputerInstance::getMaxPatternLength, 1);
    lua_setfield(state, -2, "maxPatternInputLength");

    lua_pushlightuserdata(state, this);
    lua_pushcclosure(state, &ComputerInstance::getTimeout, 1);
    lua_setfield(state, -2, "timeout");
    lua_setglobal(state, "system");

    // Unicode API
    lua_newtable(state);
    lua_pushcclosure(state, &ComputerInstance::unicodeChar, 0);
    lua_setfield(state, -2, "char");

    lua_pushcclosure(state, &ComputerInstance::unicodeLower, 0);
    lua_setfield(state, -2, "lower");

    lua_pushcclosure(state, &ComputerInstance::unicodeUpper, 0);
    lua_setfield(state, -2, "upper");

    lua_pushcclosure(state, &ComputerInstance::unicodeLength, 0);
    lua_setfield(state, -2, "len");

    lua_pushcclosure(state, &ComputerInstance::unicodeReverse, 0);
    lua_setfield(state, -2, "reverse");

    lua_pushcclosure(state, &ComputerInstance::unicodeSub, 0);
    lua_setfield(state, -2, "sub");

    lua_pushcclosure(state, &ComputerInstance::unicodeIsWide, 0);
    lua_setfield(state, -2, "isWide");

    lua_pushcclosure(state, &ComputerInstance::unicodeCharWidth, 0);
    lua_setfield(state, -2, "charWidth");

    lua_pushcclosure(state, &ComputerInstance::unicodeWLength, 0);
    lua_setfield(state, -2, "wlen");

    lua_pushcclosure(state, &ComputerInstance::unicodeWTrunc, 0);
    lua_setfield(state, -2, "wtrunc");
    lua_setglobal(state, "unicode");

    thread = lua_newthread(state);
    clock_gettime(CLOCK_MONOTONIC, &startTime);
    int suc = luaL_loadfile(thread, (basePath + "/kernel.lua").c_str());
    // int suc = luaL_loadstring(thread, "computer.pushSignal('string', 'string2', 30, true) print(coroutine.yield())");
    if (suc == LUA_ERRSYNTAX) {
        std::cerr << lua_tostring(thread, -1);
    }

    eventThread = Glib::Threads::Thread::create(sigc::bind(sigc::mem_fun(eventHandler, &EventHandler::start), thread));
    /*eventHandler.resumeThread(thread);
    usedMemory = 0; // Set memory baseline
    eventHandler.resumeThread(thread);*/
    std::cout << "Thread is working" << std::endl;
}

int ComputerInstance::listComponents(lua_State *state) {
    ComputerInstance *comp = (ComputerInstance *) lua_touserdata(state, lua_upvalueindex(1));
    std::string filter;
    if (lua_isstring(state, 1))
        filter = luaL_checkstring(state, 1);
    bool exact = false;
    if (lua_isboolean(state, 2)) {
        if (lua_toboolean(state, 2) == 1) {
            exact = true;
        }
    }

    lua_newtable(state);

    std::string address;
    // int nOn = 0;

    for (cIterator m = comp->components.begin(); m != comp->components.end(); ++m) {
        address = (*m)->getType();
        if ((exact == true && address == filter) || (exact == false && address.substr(0, filter.length()) == filter)) {
            // lua_pushnumber(state, ++nOn);
            lua_pushstring(state, (*m)->getUUID().c_str());
            lua_pushstring(state, (*m)->getType().c_str());
            lua_settable(state, -3);
        }
    }

    return 1;
}

int ComputerInstance::componentDocs(lua_State *state) {
    ComputerInstance *comp = (ComputerInstance *) lua_touserdata(state, lua_upvalueindex(1));
    std::string address = luaL_checkstring(state, 1);

    for (cIterator m = comp->components.begin(); m != comp->components.end(); ++m) {
        if ((*m)->getUUID() == address) {
            return (*m)->getDocumentation(state);
        }
    }

    lua_pushnil(state);
    lua_pushstring(state, "Address not found"); // TODO: Official error message
    return 2;
}

int ComputerInstance::componentInvoke(lua_State *state) {
    ComputerInstance *comp = (ComputerInstance *) lua_touserdata(state, lua_upvalueindex(1));
    std::string address = luaL_checkstring(state, 1);

    for (cIterator m = comp->components.begin(); m != comp->components.end(); ++m) {
        if ((*m)->getUUID() == address) {
            return (*m)->invokeMethod(state);
        }
    }

    lua_pushnil(state);
    lua_pushstring(state, "Address not found"); // TODO: Official error message
    return 2;
}

int ComputerInstance::listMethods(lua_State *state) {
    ComputerInstance *comp = (ComputerInstance *) lua_touserdata(state, lua_upvalueindex(1));
    std::string address = luaL_checkstring(state, 1);

    for (cIterator m = comp->components.begin(); m != comp->components.end(); ++m) {
        if ((*m)->getUUID() == address)
            return (*m)->listMethods(state);
    }

    lua_pushnil(state);
    lua_pushstring(state, "Address not found"); // TODO: Official error message
    return 2;
}

int ComputerInstance::componentSlot(lua_State *state) {
    ComputerInstance *comp = (ComputerInstance *) lua_touserdata(state, lua_upvalueindex(1));
    std::string address = luaL_checkstring(state, 1);

    for (cIterator m = comp->components.begin(); m != comp->components.end(); ++m) {
        if ((*m)->getUUID() == address) {
            lua_pushnumber(state, (*m)->getSlot());
            return 1;
        }
    }

    lua_pushnil(state);
    lua_pushstring(state, "Address not found"); // TODO: Official error message
    return 2;
}

int ComputerInstance::componentType(lua_State *state) {
    ComputerInstance *comp = (ComputerInstance *) lua_touserdata(state, lua_upvalueindex(1));
    std::string address = luaL_checkstring(state, 1);

    for (cIterator m = comp->components.begin(); m != comp->components.end(); ++m) {
        if ((*m)->getUUID() == address) {
            lua_pushstring(state, ((*m)->getType()).c_str());
            return 1;
        }
    }

    lua_pushnil(state);
    lua_pushstring(state, "Address not found"); // TODO: Official error message
    return 2;
}

int ComputerInstance::getComputerAddress(lua_State *state) {
    ComputerInstance *comp = (ComputerInstance *) lua_touserdata(state, lua_upvalueindex(1));
    lua_pushstring(state, comp->uuid.c_str());
    return 1;
}

int ComputerInstance::getEnergy(lua_State *state) {
    ComputerInstance *comp = (ComputerInstance *) lua_touserdata(state, lua_upvalueindex(1));
    lua_pushnumber(state, comp->energy);
    return 1;
}

int ComputerInstance::getFreeMemory(lua_State *state) {
    ComputerInstance *comp = (ComputerInstance *) lua_touserdata(state, lua_upvalueindex(1));
    lua_pushnumber(state, comp->maxMemory - comp->usedMemory);
    return 1;
}

int ComputerInstance::getMaxEnergy(lua_State *state) {
    ComputerInstance *comp = (ComputerInstance *) lua_touserdata(state, lua_upvalueindex(1));
    lua_pushnumber(state, comp->maxMemory);
    return 1;
}

int ComputerInstance::getTmpAddress(lua_State *state) {
    return 0;
}

int ComputerInstance::getTotalMemory(lua_State *state) {
    ComputerInstance *comp = (ComputerInstance *) lua_touserdata(state, lua_upvalueindex(1));
    lua_pushnumber(state, comp->maxMemory);
    return 1;
}

int ComputerInstance::getUptime(lua_State *state) {
    ComputerInstance *comp = (ComputerInstance *) lua_touserdata(state, lua_upvalueindex(1));
    double uptime;
    timespec cTime;
    clock_gettime(CLOCK_MONOTONIC, &cTime);
    uptime = ((double) cTime.tv_sec + cTime.tv_nsec / 1.0e9) - ((double) comp->startTime.tv_sec + comp->startTime.tv_nsec / 1.0e9);

    lua_pushnumber(state, uptime);
    return 1;
}

int ComputerInstance::getRealTime(lua_State *state) {
    ComputerInstance *comp = (ComputerInstance *) lua_touserdata(state, lua_upvalueindex(1));
    timespec spec;
    time_t cTime;
    clock_gettime(CLOCK_REALTIME, &spec);
    cTime = spec.tv_sec;
    long secs = (long) cTime + ((long) spec.tv_nsec / 1.0e9); // 1,000,000,000

    lua_pushnumber(state, secs);
    return 1;
}

int ComputerInstance::isRobot(lua_State *state) {
    ComputerInstance *comp = (ComputerInstance *) lua_touserdata(state, lua_upvalueindex(1));
    lua_pushboolean(state, comp->robot);
    return 1;
}

int ComputerInstance::getBootAddress(lua_State *state) {
    ComputerInstance *comp = (ComputerInstance *) lua_touserdata(state, lua_upvalueindex(1));
    if (comp->bootAddress.empty()) {
        lua_pushnil(state);
        return 1;
    }
    lua_pushstring(state, comp->bootAddress.c_str());
    return 1;
}

int ComputerInstance::setBootAddress(lua_State *state) {
    ComputerInstance *comp = (ComputerInstance *) lua_touserdata(state, lua_upvalueindex(1));
    std::string address;
    if (lua_isstring(state, 1))
        address = luaL_checkstring(state, 1);
    comp->bootAddress = address;
    lua_pushboolean(state, true);
    return 1;
}

int ComputerInstance::addUser(lua_State *state) {
    ComputerInstance *comp = (ComputerInstance *) lua_touserdata(state, lua_upvalueindex(1));
    std::string user = luaL_checkstring(state, 1);
    bool exists = false;
    for (std::vector<std::string>::iterator it = comp->users.begin(); it != comp->users.end(); ++it) {
        if (*it == user)
            exists = true;
    }

    if (exists == false) {
        comp->users.push_back(user);
        lua_pushboolean(state, true);
    }
    else
        lua_pushboolean(state, false);

    return 1;
}

int ComputerInstance::removeUser(lua_State *state) {
    ComputerInstance *comp = (ComputerInstance *) lua_touserdata(state, lua_upvalueindex(1));
    std::string user = luaL_checkstring(state, 1);
    bool exists = false;
    for (std::vector<std::string>::iterator it = comp->users.begin(); it != comp->users.end(); ++it) {
        if (*it == user) {
            comp->users.erase(it);
            lua_pushboolean(state, true);
            return 1;
        }
    }

    lua_pushboolean(state, false);
    return 1;
}

int ComputerInstance::pushSignal(lua_State *state) {
    EventHandler *eh = (EventHandler *) lua_touserdata(state, lua_upvalueindex(1));

    if (lua_gettop(state) == 0) {
        std::cerr << "Blank call to pushSignal.." << std::endl;
        return 0;
    }

    eevent ev;

    for (int i = 1; i != lua_gettop(state) + 1; ++i) {
        eventParameter par;
        if (lua_isboolean(state, i)) {
            par.type = 1;
            par.boolean = lua_toboolean(state, i);
            std::cout << par.boolean << ", ";
        }
        else if (lua_isnumber(state, i)) {
            par.type = 0;
            par.number = lua_tonumber(state, i);
            std::cout << par.number << ", ";
        }
        else if (lua_isstring(state, i)) {
            par.type = 2;
            par.string = lua_tostring(state, i);
        }
        else if (lua_isnil(state, i)) {
            par.type = 3;
        }
        else {
            std::cerr << "Invalid parameter passed to pushSignal.. passing as nil" << std::endl;
            par.type = 3;
        }

        ev.push_back(par);
    }

    eh->queueEvent(ev);
    return 0;
}

int ComputerInstance::getAllowBytecode(lua_State *state) {
    lua_pushboolean(state, false);
    return 1;
}

int ComputerInstance::getMaxPatternLength(lua_State *state) {
    lua_pushinteger(state, 100000); // I don't know.
    return 1;
}

int ComputerInstance::getTimeout(lua_State *state) {
    lua_pushnumber(state, 30);
    return 1;
}

int ComputerInstance::unicodeChar(lua_State *state) {
    Glib::ustring str;
    for (int i = 1; i != lua_gettop(state) + 1; ++i) {
        str.append(1, (gunichar) luaL_checkinteger(state, i));
    }

    lua_pushstring(state, str.c_str());
    return 1;
}

int ComputerInstance::unicodeLength(lua_State *state) {
    Glib::ustring str = luaL_checkstring(state, 1);

    lua_pushinteger(state, str.length());
    return 1;
}

int ComputerInstance::unicodeLower(lua_State *state) {
    Glib::ustring str = luaL_checkstring(state, 1);

    lua_pushstring(state, str.lowercase().c_str());
    return 1;
}

int ComputerInstance::unicodeReverse(lua_State *state) {
    Glib::ustring str = luaL_checkstring(state, 1);
    Glib::ustring newStr;

    for (Glib::ustring::reverse_iterator it = str.rbegin(); it != str.rend(); ++it) {
        newStr.append(1, *it);
    }

    lua_pushstring(state, newStr.c_str());
    return 1;
}

int ComputerInstance::unicodeSub(lua_State *state) {
    Glib::ustring str = luaL_checkstring(state, 1);
    int start = luaL_checkinteger(state, 2);
    if (start < 0) {
        start += str.length();
    }
    else {
        start -= 1;
    }
    if (start < 0)
        start = 0;


    int end = str.length();
    if (lua_isnumber(state, 3))
        end = luaL_checkinteger(state, 3);
    if (end < 0) {
        end += str.length() + 1;
    }
    if (end > str.length()) {
        end = str.length();
    }

    if (end <= start) {
        lua_pushstring(state, "");
        return 1;
    }

    lua_pushstring(state, str.substr(start, end - start).c_str());
    return 1;
}

int ComputerInstance::unicodeUpper(lua_State *state) {
    Glib::ustring str = luaL_checkstring(state, 1);

    lua_pushstring(state, str.uppercase().c_str());
    return 1;
}

int ComputerInstance::unicodeIsWide(lua_State *state) {
    Glib::ustring str = luaL_checkstring(state, 1);

    lua_pushboolean(state, Glib::Unicode::iswide(str[0]));
    return 1;
}

int ComputerInstance::unicodeCharWidth(lua_State *state) {
    Glib::ustring str = luaL_checkstring(state, 1);
    lua_pushinteger(state, wcwidth(str[0]));
    return 1;
}

int ComputerInstance::unicodeWLength(lua_State *state) {
    Glib::ustring str = luaL_checkstring(state, 1);
    int wi = 0;
    for (int i = 0; i < str.length(); ++i) {
        if (wcwidth(str[i]) < 1)
            wi++;
        else
            wi += wcwidth(str[i]);
    }

    lua_pushinteger(state, wi);
    return 1;
}

int ComputerInstance::unicodeWTrunc(lua_State *state) {
    Glib::ustring str = luaL_checkstring(state, 1);
    int count = luaL_checkinteger(state, 2);

    int width = 0;
    int end = 0;
    while (width < count) {
        width += wcwidth(str[end]);
        end++;
    }

    if (end > 0) {
        lua_pushstring(state, str.substr(0, end - 1).c_str());
        return 1;
    }
    lua_pushstring(state, "");
    return 1;
}

ComputerInstance::~ComputerInstance() {
    lua_close(state);
}

size_t ComputerInstance::getMaxMemory() {
    return maxMemory;
}

size_t ComputerInstance::getUsedMemory() {
    return usedMemory;
}

void ComputerInstance::setMaxMemory(size_t memory) {
    maxMemory = memory;
}

void ComputerInstance::setUsedMemory(size_t memory) {
    usedMemory = memory;
}