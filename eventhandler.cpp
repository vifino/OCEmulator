#include "eventhandler.h"

EventHandler::EventHandler() : m_Mutex() {
    stopHandler = false;
    timeout = 0;
}

EventHandler::~EventHandler() {

}

void EventHandler::start(lua_State *state) {
    while (stopHandler == false) {
        Glib::usleep(50000);

        if (timeout < 0.05) {
            // std::cout << "Resuming.." << std::endl;
            // TODO: Check event queue
            resumeThread(state);
        }
        else {
            timeout -= 0.05;
            if (!eventQueue.empty())
                resumeThread(state);
        }
    }
}

void EventHandler::resumeThread(lua_State *state) {
    Glib::Threads::Mutex::Lock lock(m_Mutex);

    while ((!eventQueue.empty() || timeout < 0.05) && stopHandler == false) {
        int args = 0;
        if (!eventQueue.empty()) {
            eevent ev = eventQueue.front();
            args = ev.size();
            for (int i = 0; i < ev.size(); i++) // Could use an iterator here
            {
                eventParameter par = ev[i];
                if (par.type == 0) // number
                {
                    lua_pushnumber(state, par.number);
                }
                else if (par.type == 1) // boolean
                {
                    lua_pushboolean(state, par.boolean);
                }
                else if (par.type == 2) // string
                {
                    lua_pushstring(state, par.string.c_str());
                }
                else {
                    lua_pushnil(state);
                }
            }
            eventQueue.pop();
        }

        resume:
        // std::cout << "Resuming " << lua_status(state) << ", " << args << std::endl;
        int suc = lua_resume(state, NULL, args);
        if (suc == LUA_YIELD) {
            if (lua_isfunction(state, 1)) {
                // std::cout << "Running function" << std::endl;
                lua_pushvalue(state, 1); // I guess I'm supposed to run the function?
                lua_pcall(state, 0, LUA_MULTRET, 0);

                args = lua_gettop(state);
                goto resume;
            }
            else if (lua_isboolean(state, 1)) {
                // Shutdown
            }
            else if (lua_isnumber(state, 1)) {
                timeout = lua_tonumber(state, 1);
            }
        }
        else if (suc == LUA_OK) {
            if (lua_isboolean(state, 1) && lua_toboolean(state, 1) == 0) {
                std::cerr << "Error: " << lua_tostring(state, 2) << std::endl;
            }
            stopHandler = true;
        }
        else if (suc == LUA_ERRRUN) {
            std::cerr << "Runtime error: " << lua_tostring(state, -1) << std::endl;
            stopHandler = true;
        }
    }
}

void EventHandler::queueEvent(eevent ev) {
    // Glib::Threads::Mutex::Lock lock(m_Mutex);
    eventQueue.push(ev);
}