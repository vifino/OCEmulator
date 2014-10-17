#include "componentscreen.h"

ComponentScreen::ComponentScreen(int slot) : ComponentBase(slot)
{
  resize(160, 50);
  pixels[4][4].bColor = 0x00FF0000;
  pixels[4][4].tColor = 0x00FFFFFF;
  pixels[4][4].text = 'a';
}

void ComponentScreen::addKeyboard(std::shared_ptr< ComponentKeyboard > kb)
{
  keyboards.push_back(kb);
}

ComponentScreen::~ComponentScreen()
{

}

std::string ComponentScreen::getType()
{
  return "screen";
}

int ComponentScreen::invokeMethod(lua_State* state)
{
  std::string method = luaL_checkstring(state, 2);
  if (method == "getKeyboards")
    return listKeyboards(state);
}

int ComponentScreen::getDocumentation(lua_State* state)
{

}

int ComponentScreen::listMethods(lua_State* state)
{
  lua_newtable(state);
  
  pushMethod(state, "address", true, false, true);
  pushMethod(state, "getKeyboards", false, false, true);
  return 1;
}

int ComponentScreen::listKeyboards(lua_State* state)
{
  lua_pushboolean(state, true);
  lua_newtable(state);
  
  for (int i = 0; i != keyboards.size(); ++i)
  {
    lua_pushinteger(state, i + 1);
    lua_pushstring(state, keyboards[i]->getUUID().c_str());
    lua_settable(state, -3);
  }
  
  lua_pushstring(state, "n");
  lua_pushinteger(state, keyboards.size());
  lua_settable(state, -3);
  return 2;
}

int ComponentScreen::getHeight()
{
  return height;
}

int ComponentScreen::getWidth()
{
  return width;
}

int ComponentScreen::resize(int w, int h)
{
  width = w;
  height = h;
  
  pixels.resize(w);
  for (int i = 0; i < pixels.size(); ++i)
  {
    pixels[i].resize(h);
  }
}