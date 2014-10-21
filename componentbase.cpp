#include "componentbase.h"

ComponentBase::ComponentBase(int nSlot) {
    boost::uuids::uuid uuidd = boost::uuids::random_generator()();
    std::stringstream uuidStream;
    uuidStream << uuidd;
    uuid = uuidStream.str();

    slot = nSlot;
}

ComponentBase::~ComponentBase() {

}

std::string ComponentBase::getUUID() {
    return uuid;
}

void ComponentBase::pushMethod(lua_State *state, std::string name, bool getter, bool setter, bool direct) {
    lua_pushstring(state, name.c_str());
    lua_newtable(state);
    lua_pushstring(state, "getter");
    lua_pushboolean(state, getter);
    lua_settable(state, -3);
    lua_pushstring(state, "setter");
    lua_pushboolean(state, setter);
    lua_settable(state, -3);
    lua_pushstring(state, "direct");
    lua_pushboolean(state, direct);
    lua_settable(state, -3);
    lua_settable(state, -3);
}

int ComponentBase::getSlot() {
    return slot;
}