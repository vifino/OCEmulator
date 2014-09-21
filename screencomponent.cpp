#include "screencomponent.h"

ScreenComponent::ScreenComponent(EmuScreen *scr)
{
    screen = scr;
    pixW = 10;
    pixH = 15;
    pixX = 160;
    pixY = 50;

    defPix.br = 0; defPix.bg = 0; defPix.bb = 0;
    defPix.tr = 1; defPix.tg = 1; defPix.tb = 1;
    defPix.ch = "a";

    resize(pixX, pixY);
}

void ScreenComponent::resize(int x, int y)
{
    pixels.resize(x);
    int on;
    for (std::vector<std::vector<pixInfo> >::iterator it = pixels.begin(); it != pixels.end(); ++it)
    {
        it->resize(y, defPix);
    }

    pixX = x;
    pixY = y;
}

void ScreenComponent::draw()
{
    glColor3f(0, 0, 0);
    int xOn = 0;
    for (std::vector<std::vector<pixInfo> >::iterator it = pixels.begin(); it != pixels.end(); ++it)
    {
        int yOn = 0;
        for (std::vector<pixInfo>::iterator pt = it->begin(); pt != it->end(); ++pt)
        {
            glColor3f(pt->br, pt->bg, pt->bb);
            glBegin(GL_QUADS);
            glVertex2i(xOn * pixW, yOn * pixH);
            glVertex2i(xOn * pixW + pixW, yOn * pixH);
            glVertex2i(xOn * pixW + pixW, yOn * pixH + pixH);
            glVertex2i(xOn * pixW, yOn * pixH + pixH);
            glEnd();

            glColor3f(pt->tr, pt->tg, pt->tb);
            std::string str;
            pt->ch.toUTF8String(str);
            screen->renderText(xOn * pixW, yOn * pixH, 0, str.c_str(), screen->uniFont);
            yOn++;
        }
        xOn++;
    }
}

int ScreenComponent::listMethods(lua_State *L)
{
    return 0;
}

int ScreenComponent::onInvoke(lua_State *L)
{
    return 0;
}

std::string ScreenComponent::getName()
{
    return "screen";
}
