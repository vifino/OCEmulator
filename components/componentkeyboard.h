#ifndef COMPONENTKEYBOARD_H
#define COMPONENTKEYBOARD_H

#include "componentbase.h"
#include "eventhandler.h"
#include <unordered_map>

class ComponentKeyboard : public ComponentBase {
public:
    ComponentKeyboard(int nSlot, EventHandler *eh);

    ~ComponentKeyboard();

    virtual std::string getType();

    virtual int invokeMethod(lua_State *state);

    virtual int getDocumentation(lua_State *state);

    virtual int listMethods(lua_State *state);

    void keyEvent(GdkEventKey *event);

private:
    EventHandler *eventH;
    std::unordered_map<int, int> kbMap;
};

#endif // COMPONENTKEYBOARD_H
