#ifndef COMPONENTGPU_H
#define COMPONENTGPU_H

#include <memory>
#include <iostream>
#include "componentbase.h"
#include "componentscreen.h"
#include "computerinstance.h"
#include <unordered_map>

class ComponentGPU : public ComponentBase
{
public:
  ComponentGPU(int slot, ComputerInstance *cal);
  ~ComponentGPU();
  std::string getType();
  int invokeMethod(lua_State* state);
  int getDocumentation(lua_State* state);
  int listMethods(lua_State* state);
  int bindScreen(lua_State *state);
  
  int getForeground(lua_State *state);
  int getBackground(lua_State *state);
  int setForeground(lua_State *state);
  int setBackground(lua_State *state);
  int getResolution(lua_State *state);
  int setResolution(lua_State *state);
  int fillScreen(lua_State *state);
  int setScreen(lua_State *state);
  int getDepth(lua_State *state);
  int copyScreen(lua_State *state);
  int getScreen(lua_State *state);
private:
  std::shared_ptr<ComponentScreen> bScreen;
  ComputerInstance *caller;
  int cBackground;
  int cForeground;
};

#endif // COMPONENTGPU_H
