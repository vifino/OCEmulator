#include "filesystemcomponent.h"

std::string stripString(std::string path) {
    std::string newpath;

    int firstC = 0;
    int lastC = path.size() - 1;

    if (path.substr(0, 1) == " ") {
        for (size_t on = 0; on < path.size(); on++) {
            if (path.substr(on, 1) != " ") {
                firstC = on;
                break;
            }
        }
    }

    if (path.substr(path.size() - 1, 1) == " ") {
        for (size_t on = firstC; on < path.size(); on++) {
            if (path.substr(on, 1) != " ") {
                lastC = on;
            }
        }
    }

    std::string paths = path;

    newpath = paths.substr(firstC, lastC - firstC + 1);
    return newpath;
}

std::string normalize(std::string path)
{
    //"foo//\\\bar////bar2///../.\bar2" -> foo/bar/bar2
    std::size_t found1;
    std::size_t found2;
    std::size_t curchar = 0;
    std::string final;
    std::string buffer;
    bool notdone = true;
    bool wok = false;
    int dun = 0;

    while (notdone) {
        //std::cout << "loop" << std::endl;
        //find the current element
        if (curchar < path.size()) {
            found1 = path.find("/", curchar);
            found2 = path.find("\\",curchar);

            if (found1 != std::string::npos && found2 != std::string::npos) {
                if (found1 < found2){
                    //read from the curchar to the slash
                    wok = false;
                    if (curchar-found1 > 1){
                        buffer = stripString(path.substr(curchar,found1-curchar));
                        if (buffer != ".") {
                            if (buffer == "..") {
                                if (dun != 0) {
                                    final = final.substr(0,final.rfind("/"));
                                    dun--;
                                }else{
                                    final = final + "/" + buffer;
                                }
                            }
                            else
                            {
                                final = final + "/" + buffer;
                                //curchar = found1+2;
                                dun++;
                            }
                        }
                    }
                    curchar = found1+1;
                }else{
                    wok = false;
                    if (curchar-found2 > 1){
                        buffer = stripString(path.substr(curchar,found2-curchar));
                        if (buffer != ".") {
                            if (buffer == "..") {
                                if (dun != 0) {
                                    final = final.substr(0,final.rfind("/"));
                                    dun--;
                                }else{
                                    final = final + "/" + buffer;
                                }
                            }
                            else
                            {
                                final = final + "/" + buffer;
                                //curchar = found1+2;
                                dun++;
                            }
                        }
                    }
                    curchar = found2+1;
                }
            }else if(found1 != std::string::npos){
                wok = false;
                if (curchar-found1 > 1){
                    buffer = stripString(path.substr(curchar,found1-curchar));
                    if (buffer != ".") {
                        if (buffer == "..") {
                            if (dun != 0) {
                                final = final.substr(0,final.rfind("/"));
                                dun--;
                            }else{
                                final = final + "/" + buffer;
                            }
                        }
                        else
                        {
                            final = final + "/" + buffer;
                            //curchar = found1+2;
                            dun++;
                        }
                }
                    //add to path
                }
                curchar = found1+1;
            }else if(found2 != std::string::npos){
                wok = false;
                if (curchar-found2 > 1){
                    buffer = stripString(path.substr(curchar,found2-curchar));
                    if (buffer != ".") {
                        if (buffer == "..") {
                            if (dun != 0) {
                                final = final.substr(0,final.rfind("/"));
                                dun--;
                            }else{
                                final = final + "/" + buffer;
                            }
                        }
                        else
                        {
                            final = final + "/" + buffer;
                            //curchar = found1+2;
                            dun++;
                        }
                    }
                    //add to path
                }
                curchar = found2+1;
            }else{
                buffer = stripString(path.substr(curchar, path.size()-curchar));
                if (buffer != ".") {
                    if (buffer == "..") {
                        if (dun != 0) {
                            final = final.substr(0,final.rfind("/"));
                            dun--;
                        }else{
                            final = final + "/" + buffer;
                        }
                    }
                    else
                    {
                        final = final + "/" + buffer;
                        //curchar = found1+2;
                        dun++;
                    }
                }
                //final = final + "/" + stripString(path.substr(curchar,path.size()-curchar));
                notdone = false;
            }
        }else{
            //final = final + "/" + stripString(path.substr(curchar,path.size()-curchar));
            notdone = false;
        }
    }

    if (final.substr(0, 1) == "/") {
        final = final.substr(1);
    }

    if (final == ".." || final.substr(0, 3) == "../")
    {
        final = "";
    }

    return final;
}

FilesystemComponent::FilesystemComponent(std::string path)
{
    fpath = path + "/" + address;
    boost::filesystem::create_directories(fpath);
}

FilesystemComponent::FilesystemComponent(std::string path, std::string addr) : Component(addr)
{
    fpath = path + "/" + address;
    boost::filesystem::create_directories(fpath);
}

FilesystemComponent::~FilesystemComponent()
{
    for (std::vector<fileD>::iterator t = files.begin(); t != files.end(); ++t)
    {
        fclose(t->file);
    }
}

int FilesystemComponent::listMethods(lua_State *L)
{
    lua_newtable(L);
    lua_pushstring(L, "makeDirectory");
    lua_pushboolean(L, 0);
    lua_settable(L, -3);
    return 1;
}

int FilesystemComponent::onInvoke(lua_State *L)
{
    std::string method = luaL_checkstring(L, 2);

    if (method == "open")
    {
        std::string path = luaL_checkstring(L, 3);
        path = normalize(path);
        std::string rpath = fpath + "/" + path;
        if (path == "")
        {
            lua_pushnil(L);
            lua_pushstring(L, "file not found");
            return 2;
        }

        std::string type = "r";
        if (lua_isstring(L, 4))
        {
            type = lua_tostring(L, 4);
        }

        if (files.capacity() >= 20)
        {
            lua_pushnil(L);
            lua_pushstring(L, "too many open handles");
            return 2;
        }

        fileD file;
        file.mode = type;
        file.descriptor = rand() % 4294967295; // It will always be the same pattern but I don't give a damn right now
        file.file = fopen(rpath.c_str(), type.c_str());
        if (file.file == NULL)
        {
            lua_pushnil(L);
            lua_pushstring(L, "file not found");
            return 2;
        }
        files.push_back(file);
        lua_pushboolean(L, 1);
        lua_pushnumber(L, file.descriptor);

        std::cout << "FD: " << file.descriptor << std::endl;
        return 2;
    }
    return 0;
}

std::string FilesystemComponent::getName()
{
    return "filesystem";
}
