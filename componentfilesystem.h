#ifndef COMPONENTFILESYSTEM_H
#define COMPONENTFILESYSTEM_H

#include "componentbase.h"
#include <lua.hpp>
#include <boost/filesystem.hpp>

#include <vector>
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <cmath>

struct fileD
{
  FILE* file;
  unsigned int fd;
  std::string mode;
};

#define fileIterator std::vector<fileD>::iterator

class ComponentFilesystem : public ComponentBase
{
public:
  ComponentFilesystem(int nSlot, std::string path);
  ~ComponentFilesystem();
  
  int listMethods(lua_State *state);
  int getDocumentation(lua_State *state);
  int invokeMethod(lua_State *state);
  int openFile(lua_State *state);
  int readFile(lua_State *state);
  int writeFile(lua_State *state);
  int closeFile(lua_State *state);
  int checkExists(lua_State *state);
  int isDirectory(lua_State *state);
  int getSize(lua_State *state);
  int lastModified(lua_State *state);
  int listFiles(lua_State *state);
  int deleteFile(lua_State *state);
  int makeDirectory(lua_State *state);
  int rename(lua_State *state);
  int getLabel(lua_State *state);
  int isReadOnly(lua_State *state);
  
  std::string getType();
private:
  std::string label;
  std::string basePath;
  std::vector<fileD> files;
};

#endif // COMPONENTFILESYSTEM_H
