#include "screencomponent.h"

ScreenComponent::ScreenComponent(CompInstance *ins)
{
    com = ins;
    SDL_Init(SDL_INIT_VIDEO);
    if (TTF_Init()==-1)
    {
        std::cerr << "TTF ERROR: " << TTF_GetError() << std::endl;
    }

    font = TTF_OpenFont("unifont.ttf", 16);
    if (!font)
    {
        std::cerr << "TTF_OpenFont error: " << TTF_GetError() << std::endl;
    }

    SDL_Color m;

    pixW = 10;
    pixH = 15;
    pixX = 160;
    pixY = 50;

    defPix.bColor.r = 0; defPix.bColor.g = 0; defPix.bColor.b = 0; defPix.bColor.a = 255;
    defPix.tColor.r = 255; defPix.tColor.g = 255; defPix.tColor.b = 255; defPix.tColor.a = 255;
    defPix.ch = ' ';

    resize(pixX, pixY);

    window = SDL_CreateWindow("Screen", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, pixX * pixW, pixY * pixH, 0);
    wsurface = SDL_GetWindowSurface(window);
    surface = SDL_CreateRGBSurface(0, pixX * pixW, pixY * pixH, 32, 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);
    thread = SDL_CreateThread(ScreenComponent::drawThread, "DrawThread", (void*)this);
    draw();
}

ScreenComponent::~ScreenComponent()
{
    SDL_DestroyWindow(window);
}

int ScreenComponent::drawThread(void *screenptr)
{
    ScreenComponent *screen = (ScreenComponent*)screenptr;

    SDL_Event event;
    bool cont = true;
    while (cont)
    {
        while(SDL_PollEvent(&event))
        {
            switch(event.type)
            {
                case SDL_KEYDOWN:
                {
                    switch(event.key.keysym.sym)
                    {
                        case SDLK_a:
                        {
                            signal mine;
                            mine.push_back(strToPar("key_down"));
                            mine.push_back(strToPar("unimplementedkeyboarduuid"));
                            mine.push_back(intToPar(97));
                            mine.push_back(intToPar(30));
                            mine.push_back(strToPar("user"));

                            screen->com->sigs.push(mine);
                            screen->com->tryResume();
                        }
                    }

                    break;
                }
            }
        }
    }
}

void ScreenComponent::resize(int x, int y, bool wres)
{
    pixels.resize(x);
    int on;
    for (std::vector<std::vector<pixInfo> >::iterator it = pixels.begin(); it != pixels.end(); ++it)
    {
        it->resize(y);
    }

    pixX = x;
    pixY = y;

    if (wres)
    {
        SDL_SetWindowSize(window, x * pixW, y * pixH);
        wsurface = SDL_GetWindowSurface(window);
        SDL_FreeSurface(surface);
        surface = SDL_CreateRGBSurface(0, pixX * pixW, pixY * pixH, 32, 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);
        draw();
    }

}

void ScreenComponent::draw()
{
    SDL_Rect rec;
    rec.h = pixH;
    rec.w = pixW;
    rec.x = 0;
    rec.y = 0;

    SDL_Rect fRec;
    fRec.h = 0;
    fRec.w = 0;
    fRec.x = 0;
    fRec.y = 0;

    Uint32 col;

    SDL_Surface *fchar;

    for (std::vector<std::vector<pixInfo> >::iterator it = pixels.begin(); it != pixels.end(); ++it)
    {
        rec.y = 0;
        fRec.y = 0;
        for (std::vector<pixInfo>::iterator im = it->begin(); im != it->end(); ++im)
        {
            col = SDL_MapRGB(surface->format, im->bColor.r, im->bColor.g, im->bColor.b);
            SDL_FillRect(surface, &rec, col);
            fchar = TTF_RenderGlyph_Solid(font, im->ch, im->tColor);
            SDL_BlitSurface(fchar, NULL, surface, &fRec);
            rec.y += pixH;
            fRec.y += pixH;
        }
        rec.x += pixW;
        fRec.x += pixW;
    }

    SDL_BlitSurface(surface, NULL, wsurface, NULL);

    SDL_UpdateWindowSurface(window);
}

int ScreenComponent::listMethods(lua_State *L)
{
    lua_newtable(L);
    lua_pushstring(L, "isTouchModeInverted");
    lua_pushboolean(L, 1);
    lua_settable(L, -3);

    lua_pushstring(L, "setTouchModeInverted");
    lua_pushboolean(L, 0);
    lua_settable(L, -3);

    lua_pushstring(L, "getKeyboards");
    lua_pushboolean(L, 0);
    lua_settable(L, -3);
    return 1;
}

int ScreenComponent::onInvoke(lua_State *L)
{
    std::string method = luaL_checkstring(L, 2);

    // isTouchModeInverted and setTouchModeInverted aren't needed in emulation
    if (method == "isTouchModeInverted")
    {
        lua_pushboolean(L, 1);
        lua_pushboolean(L, 1);
        return 2;
    }
    else if (method == "setTouchMoveInverted")
    {
        lua_pushboolean(L, 1);
        return 1;
    }
    else if (method == "getKeyboards")
    {
        lua_pushboolean(L, 1);
        lua_newtable(L);
        lua_pushstring(L, "bleh");
        lua_pushboolean(L, 1);
        lua_settable(L, -3);
        return 2;
    }
    lua_pushboolean(L, 0);
    return 1;
}

std::string ScreenComponent::getName()
{
    return "screen";
}
