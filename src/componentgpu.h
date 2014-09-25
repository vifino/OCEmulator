#ifndef COMPONENTGPU_H
#define COMPONENTGPU_H
#include "component.h"
#include "compinstance.h"
#include "SDL2/SDL.h"

class ComponentGPU : public Component
{
public:
    ComponentGPU(CompInstance *ins);

    int listMethods(lua_State *L);
    int onInvoke(lua_State *L);
    std::string getName();
private:
    boost::shared_ptr<ScreenComponent> bScreen;
    CompInstance *comp;

    SDL_Color bColor;
    SDL_Color tColor;
};

#endif // COMPONENTGPU_H
