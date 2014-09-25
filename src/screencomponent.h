#ifndef SCREENCOMPONENT_H
#define SCREENCOMPONENT_H

class ScreenComponent;

#include "component.h"

#include <iostream>
#include <QColor>
#include <QTime>
#include <QString>
#include <string>
#include <vector>
#include "mainwindow.h"

#include "compinstance.h"

#include <unicode/ustring.h>
#include <unicode/unistr.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

struct pixInfo
{
    SDL_Color bColor;
    SDL_Color tColor;
    UChar ch;
};

class ScreenComponent : public Component
{
public:
    ScreenComponent(CompInstance *ins);
    ~ScreenComponent();
    void draw();
    int pixX, pixY;
    void resize(int x, int y, bool wres = false);
    std::vector<std::vector<pixInfo> > pixels;

    TTF_Font *font;
private:
    int pixH, pixW;
    int posX, posY;

    int listMethods(lua_State *L);
    int onInvoke(lua_State *L);
    std::string getName();

    pixInfo defPix;

    SDL_Window *window;
    SDL_Surface *wsurface;
    SDL_Surface *surface;
    SDL_Thread *thread;

    static int drawThread(void *screen);
    CompInstance *com;
};

#endif // SCREENCOMPONENT_H
