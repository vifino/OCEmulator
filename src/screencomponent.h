#ifndef SCREENCOMPONENT_H
#define SCREENCOMPONENT_H

class ScreenComponent;

#include "component.h"
#include "emuscreen.h"

#include <iostream>
#include <QGLWidget>
#include <QColor>
#include <QTime>
#include <QString>
#include <string>
#include <vector>
#include <unicode/ustring.h>

struct pixInfo
{
    float br, bg, bb;
    float tr, tg, tb;
    UnicodeString ch;
};

class ScreenComponent : public Component
{
public:
    ScreenComponent(EmuScreen *scr);
    void draw();
private:
    EmuScreen *screen;
    int pixX, pixY;
    int pixH, pixW;
    int posX, posY;
    std::vector<std::vector<pixInfo> > pixels;

    int listMethods(lua_State *L);
    int onInvoke(lua_State *L);
    std::string getName();

    void resize(int x, int y);
    pixInfo defPix;
};

#endif // SCREENCOMPONENT_H
