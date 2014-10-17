#include "componentgpu.h"

ComponentGPU::ComponentGPU(CompInstance *ins)
{
    comp = ins;
    bColor.r = 0; bColor.g = 0; bColor.b = 0; bColor.a = 255;
    tColor.r = 255; tColor.g = 255; tColor.b = 255; tColor.a = 255;
}

int ComponentGPU::listMethods(lua_State *L)
{
    lua_newtable(L);
    lua_pushstring(L, "bind");
    lua_pushboolean(L, 0);
    lua_settable(L, -3);

    lua_pushstring(L, "getResolution");
    lua_pushboolean(L, 0);
    lua_settable(L, -3);

    lua_pushstring(L, "setResolution");
    lua_pushboolean(L, 0);
    lua_settable(L, -3);

    lua_pushstring(L, "setBackground");
    lua_pushboolean(L, 0);
    lua_settable(L, -3);

    lua_pushstring(L, "getBackground");
    lua_pushboolean(L, 0);
    lua_settable(L, -3);

    lua_pushstring(L, "setForeground");
    lua_pushboolean(L, 0);
    lua_settable(L, -3);

    lua_pushstring(L, "getForeground");
    lua_pushboolean(L, 0);
    lua_settable(L, -3);

    lua_pushstring(L, "fill");
    lua_pushboolean(L, 0);
    lua_settable(L, -3);

    lua_pushstring(L, "set");
    lua_pushboolean(L, 0);
    lua_settable(L, -3);

    lua_pushstring(L, "getDepth");
    lua_pushboolean(L, 0);
    lua_settable(L, -3);

    lua_pushstring(L, "setDepth");
    lua_pushboolean(L, 0);
    lua_settable(L, -3);
    return 1;
}

int ComponentGPU::onInvoke(lua_State *L)
{
    std::string method = luaL_checkstring(L, 2);
    //std::cout << method << std::endl;

    if (method == "bind")
    {
        std::string addr = luaL_checkstring(L, 3);
        for (std::vector<CompPtr>::iterator it = comp->components.begin(); it != comp->components.end(); ++it)
        {
            if ((*it)->address == addr)
            {
                bScreen = boost::static_pointer_cast<ScreenComponent>(*it);
                lua_pushboolean(L, 1);
                return 1;
            }
        }
    }
    else if (method == "getResolution")
    {
        if (bScreen != NULL)
        {
            lua_pushboolean(L, 1);
            lua_pushnumber(L, bScreen->pixX);
            lua_pushnumber(L, bScreen->pixY);
            return 3;
        }
    }

    else if (method == "setResolution")
    {
        int x = luaL_checknumber(L, 3);
        int y = luaL_checknumber(L, 4);
        if (bScreen != NULL && x > 0 && y > 0)
        {
            lua_pushboolean(L, 1);
            std::cout << "setting size to " << x << ", " << y << std::endl;
            bScreen->resize(x, y, true);
            return 1;
        }
    }

    else if (method == "setBackground")
    {
        Uint32 num = luaL_checknumber(L, 3);
        lua_pushboolean(L, 1);
        lua_pushnumber(L, (bColor.r << 16) + (bColor.g << 8) + bColor.b);
        bColor.r = (num & 0xFF0000) >> 16;
        bColor.g = (num & 0xFF00) >> 8;
        bColor.b = num & 0xFF;
        return 2;
    }

    else if (method == "setForeground")
    {
        Uint32 num = luaL_checknumber(L, 3);
        lua_pushboolean(L, 1);
        lua_pushnumber(L, (tColor.r << 16) + (tColor.g << 8) + tColor.b);
        tColor.r = (num & 0xFF0000) >> 16;
        tColor.g = (num & 0xFF00) >> 8;
        tColor.b = num & 0xFF;
        return 2;
    }

    else if (method == "getForeground")
    {
        lua_pushboolean(L, 1);
        lua_pushnumber(L, (tColor.r << 16) + (tColor.g << 8) + tColor.b);
        return 2;
    }

    else if (method == "getBackground")
    {
        lua_pushboolean(L, 1);
        lua_pushnumber(L, (bColor.r << 16) + (bColor.g << 8) + bColor.b);
        return 2;
    }

    else if (method == "getDepth")
    {
        lua_pushboolean(L, 1);
        lua_pushnumber(L, 8); // Unimplemented
        return 2;
    }

    else if (method == "setDepth")
    {
        lua_pushboolean(L, 1);
        lua_pushnumber(L, 8); // Unimplemented
        return 2;
    }

    else if (method == "fill")
    {
        int px = luaL_checknumber(L, 3) - 1;
        int py = luaL_checknumber(L, 4) - 1;
        int w = luaL_checknumber(L, 5);
        int h = luaL_checknumber(L, 6);
        UnicodeString cha = luaL_checkstring(L, 7);
        UChar ch = cha[0];
        lua_pushboolean(L, 1);

        for (int ix = 0; ix != w; ix++)
        {
            for (int iy = 0; iy != h; iy++)
            {
                if (ix + px < bScreen->pixX && iy + py < bScreen->pixY)
                {
                    bScreen->pixels[ix + px][iy + py].bColor = bColor;
                    bScreen->pixels[ix + px][iy + py].tColor = tColor;
                    bScreen->pixels[ix + px][iy + py].ch = ch;
                }
            }
        }

        bScreen->draw();

        return 1;
    }

    else if (method == "set")
    {
        int px = luaL_checknumber(L, 3) - 1;
        int py = luaL_checknumber(L, 4) - 1;
        UnicodeString cha = luaL_checkstring(L, 5);
        lua_pushboolean(L, 1);

        for (int ix = 0; ix != cha.length(); ix++)
        {
            if (ix + px < bScreen->pixX && py < bScreen->pixY)
            {
                bScreen->pixels[ix + px][py].bColor = bColor;
                bScreen->pixels[ix + px][py].tColor = tColor;
                bScreen->pixels[ix + px][py].ch = cha[ix];
            }
        }
        bScreen->draw();

        return 1;
    }

    lua_pushboolean(L, 0);
    return 1;
}

std::string ComponentGPU::getName()
{
    return "gpu";
}
