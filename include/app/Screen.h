#pragma once
#include <iostream>
#include <SDL3/SDL.h>

class App;

class Screen
{
protected:
    App* app;
public:
    Screen(App* app) : app(app) {}
    virtual ~Screen() {}
    virtual bool Init()=0;
    virtual void handleEvent(const SDL_Event& e) = 0;
    virtual void update() = 0;
    virtual void render(SDL_Renderer* renderer) = 0;
    virtual void onEnter() {}
    virtual void onExit() {}
};