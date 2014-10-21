#include "componentscreen.h"

ComponentScreen::ComponentScreen(int slot) : ComponentBase(slot) {
    pixW = 8;
    pixH = 16;

    img = Cairo::ImageSurface::create(Cairo::FORMAT_RGB24, 160 * pixW, 50 * pixH);
    cont = Cairo::Context::create(img);

    font.set_family("Unifont");
    font.set_weight(Pango::WEIGHT_NORMAL);
    font.set_size(1024 * 11);
    font.set_absolute_size(1024 * 16);
    fontLayout = Pango::Layout::create(cont);
    fontLayout->set_font_description(font);

    resize(160, 50);
}

void ComponentScreen::addKeyboard(std::shared_ptr<ComponentKeyboard> kb) {
    keyboards.push_back(kb);
}

ComponentScreen::~ComponentScreen() {

}

std::string ComponentScreen::getType() {
    return "screen";
}

int ComponentScreen::invokeMethod(lua_State *state) {
    std::string method = luaL_checkstring(state, 2);
    if (method == "getKeyboards")
        return listKeyboards(state);
}

int ComponentScreen::getDocumentation(lua_State *state) {

}

int ComponentScreen::listMethods(lua_State *state) {
    lua_newtable(state);

    pushMethod(state, "address", true, false, true);
    pushMethod(state, "getKeyboards", false, false, true);
    return 1;
}

int ComponentScreen::listKeyboards(lua_State *state) {
    lua_pushboolean(state, true);
    lua_newtable(state);

    for (int i = 0; i != keyboards.size(); ++i) {
        lua_pushinteger(state, i + 1);
        lua_pushstring(state, keyboards[i]->getUUID().c_str());
        lua_settable(state, -3);
    }

    lua_pushstring(state, "n");
    lua_pushinteger(state, keyboards.size());
    lua_settable(state, -3);
    return 2;
}

int ComponentScreen::getHeight() {
    return height;
}

int ComponentScreen::getWidth() {
    return width;
}

int ComponentScreen::resize(int w, int h) {
    width = w;
    height = h;

    pixels.resize(w);
    for (int i = 0; i < pixels.size(); ++i) {
        pixels[i].resize(h);
    }

    Cairo::RefPtr<Cairo::ImageSurface> nImg = Cairo::ImageSurface::create(img->get_data(), Cairo::FORMAT_RGB24, w * pixW, h * pixH, img->get_stride());
    img = nImg;
}