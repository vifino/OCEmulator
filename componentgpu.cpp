#include "componentgpu.h"

ComponentGPU::ComponentGPU(int slot, ComputerInstance *cal) : ComponentBase(slot)
{
  caller = cal;
  cForeground = 0x00FFFFFF;
  cBackground = 0;
}

ComponentGPU::~ComponentGPU()
{

}

std::string ComponentGPU::getType()
{
  return "gpu";
}

int ComponentGPU::invokeMethod(lua_State* state)
{
  std::string method = luaL_checkstring(state, 2);
  
  if (method == "bind")
    return bindScreen(state);
  else if (method == "getForeground")
    return getForeground(state);
  else if (method == "getBackground")
    return getBackground(state);
  else if (method == "setForeground")
    return setForeground(state);
  else if (method == "setBackground")
    return setBackground(state);
  else if (method == "getResolution")
    return getResolution(state);
  else if (method == "setResolution")
    return setResolution(state);
  else if (method == "fill")
    return fillScreen(state);
  else if (method == "set")
    return setScreen(state);
  else if (method == "getDepth")
    return getDepth(state);
  else if (method == "copy")
    return copyScreen(state);
  else if (method == "get")
    return getScreen(state);
}

int ComponentGPU::listMethods(lua_State* state)
{
  lua_newtable(state);
  
  pushMethod(state, "address", true, false, true);
  pushMethod(state, "bind", false, false, true);
  pushMethod(state, "getForeground", false, false, true);
  pushMethod(state, "getBackground", false, false, true);
  pushMethod(state, "setForeground", false, false, true);
  pushMethod(state, "setBackground", false, false, true);
  pushMethod(state, "getResolution", false, false, true);
  pushMethod(state, "setResolution", false, false, true);
  pushMethod(state, "fill", false, false, true);
  pushMethod(state, "set", false, false, true);
  pushMethod(state, "getDepth", false, false, true);
  pushMethod(state, "copy", false, false, true);
  pushMethod(state, "get", false, false, true);
  return 1; 
}

int ComponentGPU::getDocumentation(lua_State* state)
{
  return 0;
}

int ComponentGPU::getResolution(lua_State* state)
{
  lua_pushboolean(state, true);
  if (!bScreen)
  {
    lua_pushinteger(state, 0);
    lua_pushinteger(state, 0);
    return 3;
  }
  lua_pushinteger(state, bScreen->getWidth());
  lua_pushinteger(state, bScreen->getHeight());
  return 3;
}

int ComponentGPU::getScreen(lua_State* state)
{
  int x = luaL_checkinteger(state, 3) - 1;
  int y = luaL_checkinteger(state, 4) - 1;
  
  if (x >= 0 && y >= 0 && x < bScreen->getWidth() && y < bScreen->getHeight())
  {
    lua_pushboolean(state, true);
    lua_pushstring(state, Glib::ustring(1, bScreen->pixels[x][y].text).c_str());
    lua_pushinteger(state, bScreen->pixels[x][y].bColor);
    lua_pushinteger(state, bScreen->pixels[x][y].tColor);
    return 4;
  }
  return 0;
}

int ComponentGPU::fillScreen(lua_State* state)
{
  if (!bScreen)
    return 0;
  Glib::Threads::Mutex::Lock lock(bScreen->mut);
  int x = luaL_checkinteger(state, 3) - 1;
  int y = luaL_checkinteger(state, 4) - 1;
  int w = luaL_checkinteger(state, 5);
  int h = luaL_checkinteger(state, 6);
  Glib::ustring str = luaL_checkstring(state, 7);
  
  if (w < 0)
    w = 0;
  if (h < 0)
    h = 0;
  if (w > bScreen->getWidth() - x)
    w = bScreen->getWidth() - x;
  if (h > bScreen->getHeight() - y)
    h = bScreen->getHeight() - y;
  
  for (int xOn = x; xOn != x + w; ++xOn)
  {
    for (int yOn = y; yOn != y + h; ++yOn)
    {
      if (xOn >= 0 && xOn < bScreen->getWidth() && yOn >= 0 && yOn < bScreen->getHeight())
      {
	bScreen->pixels[xOn][yOn].bColor = cBackground;
	bScreen->pixels[xOn][yOn].tColor = cForeground;
	bScreen->pixels[xOn][yOn].text = str[0];
      }
    }
  }
  
  caller->screenW->queue_draw();
  lua_pushboolean(state, true);
  return 1;
}

int ComponentGPU::setScreen(lua_State* state)
{
  if (!bScreen)
    return 0;
  
  // Glib::Threads::Mutex::Lock lock(bScreen->mut);
  int x = luaL_checkinteger(state, 3) - 1;
  int y = luaL_checkinteger(state, 4) - 1;
  Glib::ustring str = luaL_checkstring(state, 5);
  bool vertical = false;
  if (lua_isboolean(state, 6))
    vertical = lua_toboolean(state, 6);
  
  // std::cout << "Setting '" << str << "' at " << x << ", " << y << " Vertical: " << vertical << std::endl;

  if (vertical == false)
  {
    for (int on = 0; on < str.length(); ++on)
    {
      if (on + x < bScreen->getWidth() && y >= 0 && y < bScreen->getHeight())
      {
	bScreen->pixels[on + x][y].bColor = cBackground;
	bScreen->pixels[on + x][y].tColor = cForeground;
	bScreen->pixels[on + x][y].text = str[on];
      }
    }
  }
  else
  {
    for (int on = 0; on < str.length(); ++on)
    {
      if (on + y < bScreen->getHeight() && x > 0 && x < bScreen->getWidth())
      {
	bScreen->pixels[x][on + y].bColor = cBackground;
	bScreen->pixels[x][on + y].tColor = cForeground;
	bScreen->pixels[x][on + y].text = str[on];
      }
    }
  }
  
  caller->screenW->queue_draw();
  lua_pushboolean(state, true);
  return 1;
}

int ComponentGPU::copyScreen(lua_State* state)
{
  if (!bScreen)
    return 0;
  int x = luaL_checkinteger(state, 3) - 1;
  int y = luaL_checkinteger(state, 4) - 1;
  int w = luaL_checkinteger(state, 5);
  int h = luaL_checkinteger(state, 6);
  int tx = luaL_checkinteger(state, 7);
  int ty = luaL_checkinteger(state, 8);
  
  if (x < 0)
  {
    w += x;
    x = 0;
  }
  if (y < 0)
  {
    h += y;
    y = 0;
  }
  if (w + x >= bScreen->getWidth())
    w = bScreen->getWidth() - x;
  if (h + y >= bScreen->getHeight())
    h = bScreen->getHeight()- y;
  
  /*std::vector<std::vector<pixel> > cBuffer(bScreen->pixels);
  
  for (int xOn = x; xOn < w + x; ++xOn)
  {
    if (xOn >= 0 && xOn < bScreen->getWidth() && xOn + tx >= 0 && xOn + tx < bScreen->getWidth())
    {
      bScreen->pixels[xOn + tx].insert(bScreen->pixels[xOn + tx].begin() + y + ty, cBuffer[xOn + tx].begin() + y, cBuffer[xOn].begin() + y + h);
    }
  }*/
  
  std::unordered_map<int,std::unordered_map<int,pixel> > tmpBuffer;
  std::unordered_map<int,std::unordered_map<int,bool> > tmpBufferE;
  
  for (int xOn = x; xOn < x + w; ++xOn)
  {
    for (int yOn = y; yOn < y + h; ++yOn)
    {
      if (xOn + tx >= 0 && yOn + ty >= 0 && xOn < bScreen->getWidth() && yOn < bScreen->getWidth() && xOn + tx < bScreen->getWidth() && yOn + ty < bScreen->getHeight())
      {
	if (tmpBufferE[xOn][yOn])
	{
	  tmpBuffer[xOn + tx][yOn + ty] = bScreen->pixels[xOn + tx][yOn + ty];
	  tmpBufferE[xOn + tx][yOn + ty] = true;
	  bScreen->pixels[xOn + tx][yOn + ty] = tmpBuffer[xOn][yOn];
	}
	else
	{
	  tmpBuffer[xOn + tx][yOn + ty] = bScreen->pixels[xOn + tx][yOn + ty];
	  tmpBufferE[xOn + tx][yOn + ty] = true;
	  bScreen->pixels[xOn + tx][yOn + ty] = bScreen->pixels[xOn][yOn];
	}
      }
    }
  }
  
  lua_pushboolean(state, true);
  lua_pushboolean(state, true);
  return 2;
}

int ComponentGPU::getDepth(lua_State* state)
{
  lua_pushboolean(state, true);
  lua_pushnumber(state, 4);
  return 2;
}

int ComponentGPU::setResolution(lua_State* state)
{
  Glib::Threads::Mutex::Lock lock(bScreen->mut);
  bScreen->resize(luaL_checkinteger(state, 3), luaL_checkinteger(state, 4));
  lua_pushboolean(state, true);
  caller->screenW->queue_draw();
  return 1;
}

int ComponentGPU::getBackground(lua_State* state)
{
  lua_pushboolean(state, true);
  lua_pushinteger(state, cBackground);
  return 2;
}

int ComponentGPU::getForeground(lua_State* state)
{
  lua_pushboolean(state, true);
  lua_pushinteger(state, cForeground);
  return 2;
}

int ComponentGPU::setBackground(lua_State* state)
{
  if (!lua_isnumber(state, 3))
  {
    std::cout << "Not number" << std::endl;
  }
  int ccBackground = cBackground;
  cBackground = luaL_checkinteger(state, 3);
  lua_pushboolean(state, true);
  lua_pushinteger(state, ccBackground);
  return 2;
}

int ComponentGPU::setForeground(lua_State* state)
{
  if (!lua_isnumber(state, 3))
  {
    std::cout << "Not number" << std::endl;
  }
  int ccForeground = cForeground;
  cForeground = luaL_checkinteger(state, 3);
  lua_pushboolean(state, true);
  lua_pushinteger(state, ccForeground);
  return 2;
}

int ComponentGPU::bindScreen(lua_State* state)
{
  std::string address = luaL_checkstring(state, 3);
  
  for (cIterator m = caller->components.begin(); m != caller->components.end(); ++m)
  {
    if ((*m)->getUUID() == address && (*m)->getType() == "screen")
    {
      bScreen = std::dynamic_pointer_cast<ComponentScreen>(*m);
      lua_pushboolean(state, true);
      return 1;
    }
  }
}