#ifndef COMPONENTSCREEN_H
#define COMPONENTSCREEN_H

#include <lua.hpp>

#include "componentbase.h"
#include "componentkeyboard.h"
#include <gtkmm.h>
#include <iostream>
#include <vector>
#include <memory>

struct pixel {
    unsigned int bColor;
    unsigned int tColor;
    gunichar text;
};

class ComponentScreen : public ComponentBase {
public:
    ComponentScreen(int slot);

    ~ComponentScreen();

    std::string getType();

    int invokeMethod(lua_State *state);

    int getDocumentation(lua_State *state);

    int listMethods(lua_State *state);

    int getWidth();

    int getHeight();

    int resize(int w, int h);

    int listKeyboards(lua_State *state);

    std::vector<std::vector<pixel> > pixels; // Should ONLY be used by ScreensWidget
    Glib::Threads::Mutex mut;

    void addKeyboard(std::shared_ptr<ComponentKeyboard> kb);

    std::vector<std::shared_ptr<ComponentKeyboard> > keyboards; // Should ONLY be used by ScreensWidget

    Cairo::RefPtr<Cairo::ImageSurface> img;
    Cairo::RefPtr<Cairo::Context> cont;
    int pixW, pixH;

    Pango::FontDescription font;
    Glib::RefPtr<Pango::Layout> fontLayout;
private:
    int width, height;
};

#endif // COMPONENTSCREEN_H
