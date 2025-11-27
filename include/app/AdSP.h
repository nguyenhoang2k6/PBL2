#pragma once
#include <app/Screen.h>
#include <iostream>
#include <SDL3/SDL.h>

class Button;
class Label;
class TextBox;

class AdSP : public Screen
{
private:
    Button* button_test;

public:
    AdSP(App* app);
    ~AdSP();
    bool Init() override;

    void handleEvent(const SDL_Event& e) override;
    void update() override;
    void render(SDL_Renderer* renderer) override;
    void onEnter() override;
    void onExit() override;
};
