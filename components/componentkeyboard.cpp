#include "componentkeyboard.h"

ComponentKeyboard::ComponentKeyboard(int nSlot, EventHandler *eh) : ComponentBase(nSlot) {
    eventH = eh;

    kbMap = {
            {GDK_KEY_BackSpace, 14},
            {GDK_KEY_Tab, 15},
            {GDK_KEY_Return, 28},
            {GDK_KEY_Delete, 211},
            {GDK_KEY_Home, 199},
            {GDK_KEY_Left, 203},
            {GDK_KEY_Up, 200},
            {GDK_KEY_Right, 205},
            {GDK_KEY_Down, 208},
            {GDK_KEY_End, 207},

            {GDK_KEY_F1, 59},
            {GDK_KEY_F2, 60},
            {GDK_KEY_F3, 61},
            {GDK_KEY_F4, 62},
            {GDK_KEY_F5, 63},
            {GDK_KEY_F6, 64},
            {GDK_KEY_F7, 65},
            {GDK_KEY_F8, 66},
            {GDK_KEY_F9, 67},
            {GDK_KEY_F10, 68},
            {GDK_KEY_F12, 88},

            {GDK_KEY_Shift_L, 42},
            {GDK_KEY_Shift_R, 54},
            {GDK_KEY_Control_L, 29},
            {GDK_KEY_Control_R, 157},
            {GDK_KEY_Caps_Lock, 58},
            {GDK_KEY_Alt_L, 56},
            {GDK_KEY_Alt_R, 184},
            {GDK_KEY_space, 57},

            {GDK_KEY_0, 11},
            {GDK_KEY_1, 2},
            {GDK_KEY_2, 3},
            {GDK_KEY_3, 4},
            {GDK_KEY_4, 5},
            {GDK_KEY_5, 6},
            {GDK_KEY_6, 7},
            {GDK_KEY_7, 8},
            {GDK_KEY_8, 9},
            {GDK_KEY_9, 10},

            {GDK_KEY_A, 30},
            {GDK_KEY_B, 48},
            {GDK_KEY_C, 46},
            {GDK_KEY_D, 32},
            {GDK_KEY_E, 18},
            {GDK_KEY_F, 33},
            {GDK_KEY_G, 34},
            {GDK_KEY_H, 35},
            {GDK_KEY_I, 23},
            {GDK_KEY_J, 36},
            {GDK_KEY_K, 37},
            {GDK_KEY_L, 38},
            {GDK_KEY_M, 50},
            {GDK_KEY_N, 49},
            {GDK_KEY_O, 24},
            {GDK_KEY_P, 35},
            {GDK_KEY_Q, 16},
            {GDK_KEY_R, 19},
            {GDK_KEY_S, 31},
            {GDK_KEY_T, 20},
            {GDK_KEY_U, 22},
            {GDK_KEY_V, 47},
            {GDK_KEY_W, 17},
            {GDK_KEY_X, 45},
            {GDK_KEY_Y, 21},
            {GDK_KEY_Z, 44},

            {GDK_KEY_a, 30},
            {GDK_KEY_b, 48},
            {GDK_KEY_c, 46},
            {GDK_KEY_d, 32},
            {GDK_KEY_e, 18},
            {GDK_KEY_f, 33},
            {GDK_KEY_g, 34},
            {GDK_KEY_h, 35},
            {GDK_KEY_i, 23},
            {GDK_KEY_j, 36},
            {GDK_KEY_k, 37},
            {GDK_KEY_l, 38},
            {GDK_KEY_m, 50},
            {GDK_KEY_n, 49},
            {GDK_KEY_o, 24},
            {GDK_KEY_p, 35},
            {GDK_KEY_q, 16},
            {GDK_KEY_r, 19},
            {GDK_KEY_s, 31},
            {GDK_KEY_t, 20},
            {GDK_KEY_u, 22},
            {GDK_KEY_v, 47},
            {GDK_KEY_w, 17},
            {GDK_KEY_x, 45},
            {GDK_KEY_y, 21},
            {GDK_KEY_z, 44}
    };
}

void ComponentKeyboard::keyEvent(GdkEventKey *event) {
    eevent ev;

    eventParameter ep1, ep2, ep3, ep4, ep5;
    ep1.type = 2;
    if (event->type == GDK_KEY_PRESS)
        ep1.string = "key_down";
    else if (event->type == GDK_KEY_RELEASE)
        ep1.string = "key_up";
    ev.push_back(ep1);

    ep2.type = 2;
    ep2.string = getUUID();
    ev.push_back(ep2);

    ep3.type = 0;
    ep3.number = (event->keyval < 500) ? event->keyval : 0;
    ev.push_back(ep3);

    ep4.type = 0;
    ep4.number = kbMap[event->keyval];
    ev.push_back(ep4);

    ep5.type = 2;
    ep5.string = "user";
    ev.push_back(ep5);

    eventH->queueEvent(ev);
}

ComponentKeyboard::~ComponentKeyboard() {

}

std::string ComponentKeyboard::getType() {
    return "keyboard";
}

int ComponentKeyboard::invokeMethod(lua_State *state) {
    std::string method = luaL_checkstring(state, 2);
    std::cout << "kb: " << method << std::endl;
    return 0;
}

int ComponentKeyboard::getDocumentation(lua_State *state) {
    return 0;
}

int ComponentKeyboard::listMethods(lua_State *state) {
    lua_newtable(state);
    return 1;
}
