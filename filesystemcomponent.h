#ifndef FILESYSTEMCOMPONENT_H
#define FILESYSTEMCOMPONENT_H

#include "component.h"
#include "lua.hpp"
#include <fstream>
#include <vector>
#include <cstdio>

struct fileD
{
    int descriptor;
    FILE *file;
    std::string mode;
};

class FilesystemComponent : public Component
{
public:
    FilesystemComponent(std::string path);
    FilesystemComponent(std::string path, std::string addr);
    ~FilesystemComponent();
    int listMethods(lua_State *L);
    int onInvoke(lua_State *L);
    std::string getName();
private:
    std::vector<fileD> files;
    std::string fpath;
};

#endif // FILESYSTEMCOMPONENT_H
