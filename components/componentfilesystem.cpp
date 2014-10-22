#include "componentfilesystem.h"

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

std::string normalize(std::string path) {
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
        //find the current element
        if (curchar < path.size()) {
            found1 = path.find("/", curchar);
            found2 = path.find("\\", curchar);

            if (found1 != std::string::npos && found2 != std::string::npos) {
                if (found1 < found2) {
                    //read from the curchar to the slash
                    wok = false;
                    if (curchar - found1 > 1) {
                        buffer = stripString(path.substr(curchar, found1 - curchar));
                        if (buffer != ".") {
                            if (buffer == "..") {
                                if (dun != 0) {
                                    final = final.substr(0, final.rfind("/"));
                                    dun--;
                                } else {
                                    final = final + "/" + buffer;
                                }
                            }
                            else {
                                final = final + "/" + buffer;
                                //curchar = found1+2;
                                dun++;
                            }
                        }
                    }
                    curchar = found1 + 1;
                } else {
                    wok = false;
                    if (curchar - found2 > 1) {
                        buffer = stripString(path.substr(curchar, found2 - curchar));
                        if (buffer != ".") {
                            if (buffer == "..") {
                                if (dun != 0) {
                                    final = final.substr(0, final.rfind("/"));
                                    dun--;
                                } else {
                                    final = final + "/" + buffer;
                                }
                            }
                            else {
                                final = final + "/" + buffer;
                                //curchar = found1+2;
                                dun++;
                            }
                        }
                    }
                    curchar = found2 + 1;
                }
            } else if (found1 != std::string::npos) {
                wok = false;
                if (curchar - found1 > 1) {
                    buffer = stripString(path.substr(curchar, found1 - curchar));
                    if (buffer != ".") {
                        if (buffer == "..") {
                            if (dun != 0) {
                                final = final.substr(0, final.rfind("/"));
                                dun--;
                            } else {
                                final = final + "/" + buffer;
                            }
                        }
                        else {
                            final = final + "/" + buffer;
                            //curchar = found1+2;
                            dun++;
                        }
                    }
                    //add to path
                }
                curchar = found1 + 1;
            } else if (found2 != std::string::npos) {
                wok = false;
                if (curchar - found2 > 1) {
                    buffer = stripString(path.substr(curchar, found2 - curchar));
                    if (buffer != ".") {
                        if (buffer == "..") {
                            if (dun != 0) {
                                final = final.substr(0, final.rfind("/"));
                                dun--;
                            } else {
                                final = final + "/" + buffer;
                            }
                        }
                        else {
                            final = final + "/" + buffer;
                            //curchar = found1+2;
                            dun++;
                        }
                    }
                    //add to path
                }
                curchar = found2 + 1;
            } else {
                buffer = stripString(path.substr(curchar, path.size() - curchar));
                if (buffer != ".") {
                    if (buffer == "..") {
                        if (dun != 0) {
                            final = final.substr(0, final.rfind("/"));
                            dun--;
                        } else {
                            final = final + "/" + buffer;
                        }
                    }
                    else {
                        final = final + "/" + buffer;
                        //curchar = found1+2;
                        dun++;
                    }
                }
                //final = final + "/" + stripString(path.substr(curchar,path.size()-curchar));
                notdone = false;
            }
        } else {
            //final = final + "/" + stripString(path.substr(curchar,path.size()-curchar));
            notdone = false;
        }
    }

    if (final.substr(0, 1) == "/") {
        final = final.substr(1);
    }

    if (final == ".." || final.substr(0, 3) == "../") {
        final = "";
    }

    return final;
}

ComponentFilesystem::ComponentFilesystem(int nSlot, std::string path) : ComponentBase(nSlot) {
    uuid = "debug"; // Temporary
    basePath = path + "/" + uuid;
    boost::filesystem::create_directories(basePath);
}

ComponentFilesystem::~ComponentFilesystem() {

}

int ComponentFilesystem::listMethods(lua_State *state) {
    lua_newtable(state);

    pushMethod(state, "makeDirectory", false, false, true);
    pushMethod(state, "open", false, false, true);
    pushMethod(state, "read", false, false, true);
    pushMethod(state, "write", false, false, true);
    pushMethod(state, "close", false, false, true);
    pushMethod(state, "exists", false, false, true);
    pushMethod(state, "isDirectory", false, false, true);
    pushMethod(state, "getSize", false, false, true);
    pushMethod(state, "list", false, false, true);
    pushMethod(state, "remove", false, false, true);
    pushMethod(state, "rename", false, false, true);
    pushMethod(state, "getLabel", false, false, true);
    pushMethod(state, "isReadOnly", false, false, true);

    return 1;
}

int ComponentFilesystem::invokeMethod(lua_State *state) {
    std::string method = lua_tostring(state, 2);

    if (method == "open") {
        return openFile(state);
    }
    else if (method == "read") {
        return readFile(state);
    }
    else if (method == "close") {
        return closeFile(state);
    }
    else if (method == "exists") {
        return checkExists(state);
    }
    else if (method == "isDirectory") {
        return isDirectory(state);
    }
    else if (method == "size") {
        return getSize(state);
    }
    else if (method == "lastModified") {
        return lastModified(state);
    }
    else if (method == "list") {
        return listFiles(state);
    }
    else if (method == "remove") {
        return deleteFile(state);
    }
    else if (method == "makeDirectory") {
        return makeDirectory(state);
    }
    else if (method == "rename") {
        return rename(state);
    }
    else if (method == "getLabel") {
        return getLabel(state);
    }
    else if (method == "isReadOnly") {
        return isReadOnly(state);
    }
    else if (method == "write") {
        return writeFile(state);
    }
    else {
        lua_pushboolean(state, false);
        return 1;
    }
}

int ComponentFilesystem::openFile(lua_State *state) {
    std::string path = normalize(luaL_checkstring(state, 3));
    std::string mode = "r";
    if (lua_isstring(state, 4)) {
        mode = lua_tostring(state, 4);
    }

    // std::cout << "fopen(\"" << (basePath + "/" + path) << "\", \"" << mode<< "\")" << std::endl;

    fileD file;
    file.fd = rand() % 4294967295;
    file.mode = mode;
    file.file = fopen((basePath + "/" + path).c_str(), mode.c_str());
    if (file.file == NULL) {
        std::cout << "fopen(\"" << basePath + "/" + path << "\", \"" << mode << "\") failed" << std::endl;
        lua_pushboolean(state, true);
        lua_pushnil(state);
        lua_pushstring(state, "file not found");
        return 3;
    }

    files.push_back(file);
    lua_pushboolean(state, true);
    lua_pushinteger(state, file.fd);
    return 2;
}

int ComponentFilesystem::isReadOnly(lua_State *state) {
    lua_pushboolean(state, true);
    lua_pushboolean(state, false);
    return 2;
}

int ComponentFilesystem::readFile(lua_State *state) {
    int fd = luaL_checkinteger(state, 3);
    int amount = luaL_checkinteger(state, 4);
    fileD file;
    bool found = false;
    for (fileIterator it = files.begin(); it != files.end(); ++it) {
        if ((*it).fd == fd) {
            file = *it;
            found = true;
            break;
        }
    }

    if (found == false) {
        lua_pushboolean(state, true);
        lua_pushnil(state);
        lua_pushstring(state, "file not open");
        return 3;
    }

    if (file.file == NULL) {
        lua_pushboolean(state, true);
        lua_pushnil(state);
        lua_pushstring(state, "file not open");
        return 3;
    }

    if (amount == 0 || amount > 100000) {
        int cPos = ftell(file.file);
        fseek(file.file, 0, SEEK_END);
        amount = ftell(file.file) - cPos;
        fseek(file.file, cPos, SEEK_SET);
    }

    char buffer[amount];
    int read = fread(buffer, 1, amount, file.file);
    lua_pushboolean(state, true);

    if (read == 0) {
        lua_pushnil(state);
        return 2;
    }

    lua_pushlstring(state, buffer, read);
    return 2;
}

int ComponentFilesystem::writeFile(lua_State *state) {
    int fd = luaL_checkinteger(state, 3);
    std::string data = luaL_checkstring(state, 4);
    fileD file;
    bool found = false;
    for (fileIterator it = files.begin(); it != files.end(); ++it) {
        if ((*it).fd == fd) {
            file = *it;
            found = true;
            break;
        }
    }

    if (found == false) {
        lua_pushboolean(state, true);
        return 1;
    }

    if (file.file == NULL) {
        lua_pushboolean(state, true);
        return 1;
    }

    int wrote = fwrite(data.c_str(), 1, data.length(), file.file);
    if (wrote < 1) {
        lua_pushboolean(state, true);
        lua_pushnil(state);
        lua_pushstring(state, "No data written");
        return 3;
    }
    lua_pushboolean(state, true);
    lua_pushboolean(state, true);
    return 2;
}

int ComponentFilesystem::closeFile(lua_State *state) {
    int fd = luaL_checkinteger(state, 3);
    fileD file;

    for (fileIterator it = files.begin(); it != files.end(); ++it) {
        if ((*it).fd == fd) {
            file = *it;
            if (file.file == NULL) {
                lua_pushboolean(state, false);
                return 1;
            }

            fclose(file.file);
            files.erase(it);
            lua_pushboolean(state, true);
            return 1;
        }
    }

    lua_pushboolean(state, false);
    return 1;
}

int ComponentFilesystem::checkExists(lua_State *state) {
    std::string path = basePath + "/" + normalize(luaL_checkstring(state, 3));
    // std::cout << "Exists(\"" << path << "\")" << std::endl;
    lua_pushboolean(state, true);
    lua_pushboolean(state, boost::filesystem::exists(path));
    return 2;
}

int ComponentFilesystem::isDirectory(lua_State *state) {
    std::string path = basePath + "/" + normalize(luaL_checkstring(state, 3));
    lua_pushboolean(state, true);
    lua_pushboolean(state, boost::filesystem::is_directory(path));
    return 2;
}

int ComponentFilesystem::getSize(lua_State *state) {
    std::string path = basePath + "/" + normalize(luaL_checkstring(state, 3));
    if (boost::filesystem::is_regular_file(path)) {
        lua_pushboolean(state, true);
        lua_pushinteger(state, boost::filesystem::file_size(path));
        return 2;
    }

    lua_pushboolean(state, false);
    return 1;
}

int ComponentFilesystem::lastModified(lua_State *state) {
    std::string path = basePath + "/" + normalize(luaL_checkstring(state, 3));

    if (boost::filesystem::is_regular_file(path)) {
        lua_pushboolean(state, true);
        lua_pushnumber(state, boost::filesystem::last_write_time(path));
        return 2;
    }

    lua_pushboolean(state, false);
}

int ComponentFilesystem::listFiles(lua_State *state) {
    std::string path = std::string("/") + normalize(basePath + "/" + normalize(luaL_checkstring(state, 3)));
    // std::cout << "list(\"" << path << "\")" << std::endl;

    boost::filesystem::directory_iterator end_it;

    if (boost::filesystem::is_directory(path)) {
        lua_pushboolean(state, true);
        lua_newtable(state);
        int on = 0;
        for (boost::filesystem::directory_iterator it(path); it != end_it; ++it) {
            lua_pushinteger(state, ++on);
            std::string final = std::string(it->path().string()).substr(path.length() + 1);
            if (boost::filesystem::is_directory(it->path())) {
                final += "/";
            }

            lua_pushstring(state, final.c_str());
            lua_settable(state, -3);
        }
        return 2;
    }
    lua_pushboolean(state, false);
    return 1;
}

int ComponentFilesystem::deleteFile(lua_State *state) {
    std::string path = basePath + "/" + normalize(luaL_checkstring(state, 3));

    if (boost::filesystem::exists(path)) {
        boost::filesystem::remove(path);
        lua_pushboolean(state, true);
        lua_pushboolean(state, true);
        return 2;
    }

    lua_pushboolean(state, true);
    return 1;
}

int ComponentFilesystem::makeDirectory(lua_State *state) {
    std::string path = basePath + "/" + normalize(luaL_checkstring(state, 3));
    boost::filesystem::create_directories(path);

    lua_pushboolean(state, true);
    return 1;
}

int ComponentFilesystem::rename(lua_State *state) {
    std::string from = basePath + "/" + normalize(luaL_checkstring(state, 3));
    std::string to = basePath + "/" + normalize(luaL_checkstring(state, 4));

    if (!boost::filesystem::exists(from) || boost::filesystem::exists(to)) {
        lua_pushboolean(state, true);
        lua_pushboolean(state, false);
        return 2;
    }

    boost::filesystem::rename(from, to);

    lua_pushboolean(state, true);
    lua_pushboolean(state, true);
    return 2;
}

int ComponentFilesystem::getLabel(lua_State *state) {
    lua_pushboolean(state, true);
    if (!label.empty()) {
        lua_pushstring(state, label.c_str());
        return 2;
    }
    return 1;
}

int ComponentFilesystem::getDocumentation(lua_State *state) {
    std::string method = luaL_checkstring(state, 2);
    if (method == "makeDirectory") {
        lua_pushstring(state, "Creates a directory"); // TODO: Use official documentation
        return 1;
    }
    else if (method == "open") {
        lua_pushstring(state, "Opens a file");
        return 1;
    }
    else {
        lua_pushnil(state);
        lua_pushstring(state, "That function does not exist"); // TODO: Use official error message
        return 2;
    }
}

std::string ComponentFilesystem::getType() {
    return "filesystem";
}