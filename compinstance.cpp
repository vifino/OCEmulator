#include "compinstance.h"

CompInstance::CompInstance()
{

    std::ifstream n;
    n.open("init.lua");
    n.seekg(0, std::ios_base::end);
    unsigned int length = n.tellg() - 1;
    n.seekg(0, std::ios_base::beg);

    char init[length];
    n.close();

    state = luaL_newstate(); // Never really used

    thread = lua_newthread(state);
    luaL_requiref(thread, "package", luaopen_package, 1); // require() and stuff
    luaL_requiref(thread, "base", luaopen_base, 0); // base library...

    int err = luaL_loadstring(thread, init);
    if (err == LUA_ERRSYNTAX)
    {
        const char *bad = lua_tostring(thread, -1);
        std::cout << "SYNTAX ERROR: " << bad << std::endl;
    }
    else if (err == LUA_OK)
    {
        err = lua_resume(thread, NULL, 0);
        if (err == LUA_ERRRUN)
        {
            const char *bad = lua_tostring(thread, -1);
            std::cout << "ERROR: " << bad << std::endl;
        }
    }
}
