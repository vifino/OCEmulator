#include "compinstance.h"

CompInstance::CompInstance(std::string ipath) : addressu(boost::uuids::random_generator()())
{
    startTime.start();

    maxMemory = 1024 * 1024;
    usedMemory = 0;
    path = ipath;
    boost::filesystem::create_directories(path);

    bootAddress = "";
    std::stringstream ss;
    ss << addressu;
    address = ss.str();

    //For testing
    boost::shared_ptr<FilesystemComponent> c(new FilesystemComponent(path + "/filesystems", "f6ab49ce-b10c-4dba-ae8d-7b81a3925be5"));
    components.push_back((CompPtr)c);

    std::cout << "New computer with uuid '" << address << "'" << std::endl;

    /*std::ifstream n;
    n.open("init.lua", std::ios::ate | std::ios::binary);
    std::ifstream::pos_type length = n.tellg();
    n.seekg(0, std::ios_base::beg);

    std::vector<char> initb(length);
    n.read(&initb[0], length);*/

    FILE *initf = fopen("init.lua", "r");
    fseek(initf, 0, SEEK_END);
    int size = ftell(initf);
    fseek(initf, 0, SEEK_SET);

    char init[size + 1];
    fread(init, 1, size, initf);
    init[size] = '\0';

    fclose(initf);

    state = lua_newstate(CompInstance::l_alloc_restricted, this);

    thread = lua_newthread(state);
    //luaL_requiref(thread, "package", luaopen_package, 1); // require() and stuff
    //lua_pop(thread, 1);
    luaL_requiref(thread, "base", luaopen_base, 0);
    lua_pop(thread, 1);
    luaL_requiref(thread, "bit32", luaopen_bit32, 1);
    lua_pop(thread, 1);
    luaL_requiref(thread, "coroutine", luaopen_coroutine, 1);
    lua_pop(thread, 1);
    luaL_requiref(thread, "debug", luaopen_debug, 1);
    lua_pop(thread, 1);
    luaL_requiref(thread, "math", luaopen_math, 1);
    lua_pop(thread, 1);
    luaL_requiref(thread, "string", luaopen_string, 1);
    lua_pop(thread, 1);
    luaL_requiref(thread, "table", luaopen_table, 1);
    lua_pop(thread, 1);

    lua_newtable(thread);
    lua_setglobal(thread, "os");

    // Computer API
    lua_newtable(thread);
    lua_pushstring(thread, "address");
    lua_pushlightuserdata(thread, (void*)this); // If there was a way to avoid this, I would
    lua_pushcclosure(thread, CompInstance::onAddress, 1);
    lua_settable(thread, -3);

    lua_pushstring(thread, "getBootAddress");
    lua_pushlightuserdata(thread, (void*)this);
    lua_pushcclosure(thread, CompInstance::getBootAddress, 1);
    lua_settable(thread, -3);

    lua_pushstring(thread, "setBootAddress");
    lua_pushlightuserdata(thread, (void*)this);
    lua_pushcclosure(thread, CompInstance::setBootAddress, 1);
    lua_settable(thread, -3);

    lua_pushstring(thread, "realTime");
    // lua_pushlightuserdata(thread, (void*)this);
    lua_pushcclosure(thread, CompInstance::getRealTime, 0);
    lua_settable(thread, -3);

    lua_pushstring(thread, "uptime");
    lua_pushlightuserdata(thread, (void*)this);
    lua_pushcclosure(thread, CompInstance::getUptime, 1);
    lua_settable(thread, -3);

    lua_pushstring(thread, "freeMemory");
    lua_pushlightuserdata(thread, (void*)this);
    lua_pushcclosure(thread, CompInstance::getFreeMemory, 1);
    lua_settable(thread, -3);
    lua_setglobal(thread, "computer");

    // Component API
    lua_newtable(thread);
    lua_pushstring(thread, "list");
    lua_pushlightuserdata(thread, (void*)this);
    lua_pushcclosure(thread, CompInstance::componentList, 1);
    lua_settable(thread, -3);

    lua_pushstring(thread, "methods");
    lua_pushlightuserdata(thread, (void*)this);
    lua_pushcclosure(thread, CompInstance::methodsList, 1);
    lua_settable(thread, -3);

    lua_pushstring(thread, "invoke");
    lua_pushlightuserdata(thread, (void*)this);
    lua_pushcclosure(thread, CompInstance::componentInvoke, 1);
    lua_settable(thread, -3);

    lua_pushstring(thread, "type");
    lua_pushlightuserdata(thread, (void*)this);
    lua_pushcclosure(thread, CompInstance::componentType, 1);
    lua_settable(thread, -3);
    lua_setglobal(thread, "component");

    // System API
    lua_newtable(thread);
    lua_pushstring(thread, "allowBytecode");
    lua_pushlightuserdata(thread, (void*)this);
    lua_pushcclosure(thread, CompInstance::allowBytecode, 1);
    lua_settable(thread, -3);

    lua_pushstring(thread, "timeout");
    lua_pushlightuserdata(thread, (void*)this);
    lua_pushcclosure(thread, CompInstance::getTimeout, 1);
    lua_settable(thread, -3);
    lua_setglobal(thread, "system");

    // Unicode API
    lua_newtable(thread);
    lua_pushstring(thread, "char");
    lua_pushcfunction(thread, CompInstance::uniChar);
    lua_settable(thread, -3);

    lua_pushstring(thread, "upper");
    lua_pushcfunction(thread, CompInstance::uniUpper);
    lua_settable(thread, -3);

    lua_pushstring(thread, "lower");
    lua_pushcfunction(thread, CompInstance::uniLower);
    lua_settable(thread, -3);

    lua_pushstring(thread, "reverse");
    lua_pushcfunction(thread, CompInstance::uniReverse);
    lua_settable(thread, -3);

    lua_pushstring(thread, "len");
    lua_pushcfunction(thread, CompInstance::uniLen);
    lua_settable(thread, -3);

    lua_pushstring(thread, "sub");
    lua_pushcfunction(thread, CompInstance::uniSub);
    lua_settable(thread, -3);
    lua_setglobal(thread, "unicode");

    int err = luaL_loadstring(thread, init);
    if (err == LUA_ERRSYNTAX)
    {
        const char *bad = lua_tostring(thread, -1);
        std::cout << "SYNTAX ERROR: " << bad << std::endl;
    }
    else if (err == LUA_OK)
    {
        int args = 0;
        while ((err = lua_resume(thread, NULL, args)) == LUA_YIELD)
        {
            if (lua_gettop(thread) > 0)
            {

            }
            args = 0;
            if (lua_isfunction(thread, 1))
            {
                lua_pushvalue(thread, 1);
                lua_pcall(thread, 0, LUA_MULTRET, 0);
                args = 1;
            }
        }
        if (err == LUA_ERRRUN)
        {
            const char *bad = lua_tostring(thread, -1);
            std::cout << "ERROR: " << bad << std::endl;
        }
        else if (err == LUA_OK)
        {
            if (lua_toboolean(thread, 1) == 0)
            {
                std::cout << "ERROR: " << lua_tostring(thread, 2) << std::endl;
            }
        }
    }
}


int CompInstance::onAddress(lua_State *L)
{
    CompInstance *ins = (CompInstance*)lua_touserdata(L, lua_upvalueindex(1));
    lua_pushstring(L, ins->address.c_str());
    return 1;
}

int CompInstance::componentList(lua_State *L)
{
    CompInstance *ins = (CompInstance*)lua_touserdata(L, lua_upvalueindex(1));
    std::string filter = "";
    int exact = 0;
    if (lua_isstring(L, 1))
    {
        filter = lua_tostring(L, 1);
        if (lua_isboolean(L, 2))
        {
            exact = lua_toboolean(L, 2);
        }
    }
    lua_newtable(L);
    for(std::vector<boost::shared_ptr<Component> >::iterator it = ins->components.begin(); it != ins->components.end(); ++it)
    {
        if (filter.empty() || (*it)->getName() == filter || (exact == 1 && (*it)->getName().find(filter) != std::string::npos))
        {
            lua_pushstring(L, (*it)->address.c_str());
            lua_pushstring(L, (*it)->getName().c_str());
            lua_settable(L, -3);
        }
    }
    return 1;
}

int CompInstance::methodsList(lua_State *L)
{
    std::string address = luaL_checkstring(L, 1);
    CompInstance *ins = (CompInstance*)lua_touserdata(L, lua_upvalueindex(1));
    for(std::vector<boost::shared_ptr<Component> >::iterator it = ins->components.begin(); it != ins->components.end(); ++it)
    {
        if ((*it)->address == address)
        {
            return ((*it)->listMethods(L));
        }
    }
    return 0;
}

int CompInstance::componentInvoke(lua_State *L)
{
    std::string address = luaL_checkstring(L, 1);

    CompInstance *ins = (CompInstance*)lua_touserdata(L, lua_upvalueindex(1));
    for(std::vector<boost::shared_ptr<Component> >::iterator it = ins->components.begin(); it != ins->components.end(); ++it)
    {
        if ((*it)->address == address)
        {
            return (*it)->onInvoke(L);
        }
    }
    return 0;
}

int CompInstance::componentType(lua_State *L)
{
    std::string address = luaL_checkstring(L, 1);

    CompInstance *ins = (CompInstance*)lua_touserdata(L, lua_upvalueindex(1));
    for(std::vector<boost::shared_ptr<Component> >::iterator it = ins->components.begin(); it != ins->components.end(); ++it)
    {
        if ((*it)->address == address)
        {
            lua_pushstring(L, (*it)->getName().c_str());
            return 1;
        }
    }
    return 0;
}

int CompInstance::getBootAddress(lua_State *L)
{
    CompInstance *ins = (CompInstance*)lua_touserdata(L, lua_upvalueindex(1));
    if (ins->bootAddress == "")
        return 0;

    lua_pushstring(L, ins->bootAddress.c_str());
    return 1;
}

int CompInstance::setBootAddress(lua_State *L)
{
    CompInstance *ins = (CompInstance*)lua_touserdata(L, lua_upvalueindex(1));
    if (!lua_isstring(L, 1))
    {
        ins->bootAddress = "";
    }
    else
    {
        ins->bootAddress = lua_tostring(L, 1);
    }
    return 0;
}

int CompInstance::getRealTime(lua_State *L)
{
    lua_pushnumber(L, 67); // Hehe
    return 1;
}

int CompInstance::getUptime(lua_State *L)
{
    CompInstance *ins = (CompInstance*)lua_touserdata(L, lua_upvalueindex(1));
    lua_pushnumber(L, ins->startTime.elapsed());
    return 1;
}

int CompInstance::getFreeMemory(lua_State *L)
{
    CompInstance *ins = (CompInstance*)lua_touserdata(L, lua_upvalueindex(1));
    lua_pushnumber(L, ins->maxMemory - ins->usedMemory);
    return 1;
}

int CompInstance::allowBytecode(lua_State *L)
{
    lua_pushboolean(L, 0);
    return 1;
}

int CompInstance::getTimeout(lua_State *L)
{
    lua_pushnumber(L, 60);
    return 1;
}

int CompInstance::uniChar(lua_State *L)
{
    UnicodeString str;
    for (int i = 1; i != lua_gettop(L) + 1; ++i)
    {
        str += (UChar)luaL_checknumber(L, i);
    }

    std::string newString;
    str.toUTF8String(newString);
    lua_pushstring(L, newString.c_str());
    return 1;
}

int CompInstance::uniUpper(lua_State *L)
{
    UnicodeString str(luaL_checkstring(L, 1));
    str.toUpper();
    std::string newString;
    str.toUTF8String(newString);
    lua_pushstring(L, newString.c_str());

    return 1;
}

int CompInstance::uniLower(lua_State *L)
{
    UnicodeString str(luaL_checkstring(L, 1));
    str.toLower();
    std::string newString;
    str.toUTF8String(newString);
    lua_pushstring(L, newString.c_str());

    return 1;
}

int CompInstance::uniReverse(lua_State *L)
{
    UnicodeString str(luaL_checkstring(L, 1));
    str.reverse();
    std::string newString;
    str.toUTF8String(newString);
    lua_pushstring(L, newString.c_str());

    return 1;
}

int CompInstance::uniLen(lua_State *L)
{
    UnicodeString str(luaL_checkstring(L, 1));
    lua_pushnumber(L, str.length());

    return 1;
}

int CompInstance::uniSub(lua_State *L)
{
    UnicodeString str(luaL_checkstring(L, 1));
    int start = luaL_checknumber(L, 2);
    int length = luaL_checknumber(L, 3);
    UnicodeString newString(str, start, length);

    std::string newerString;
    newString.toUTF8String(newerString);
    lua_pushstring(L, newerString.c_str());

    return 1;
}

void *CompInstance::l_alloc_restricted (void *ud, void *ptr, size_t osize, size_t nsize)
{
  const int MAX_SIZE = 1024 * 1024; /* set limit here */
  CompInstance *ins = (CompInstance*)ud;

  if (nsize == 0)
  {
    free(ptr);
    ins->usedMemory -= osize; /* substract old size from used memory */
    return NULL;
  }
  else
  {
    if (ins->usedMemory + (nsize - osize) > MAX_SIZE) /* too much memory in use */
    {
      return NULL;
    }
    ptr = realloc(ptr, nsize);
    if (ptr) /* reallocation successful? */
      ins->usedMemory += (nsize - osize);
    return ptr;
  }
}
