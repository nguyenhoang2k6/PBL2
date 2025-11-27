#pragma once
#include <app/Screen.h>
#include <iostream>
#include <SDL3/SDL.h>

class Button;
class Label;
class TextBox;

class MainMenuScreen : public Screen
{
private:
    Button* button_NV;
    Button* button_admin;

public:
    MainMenuScreen(App* app);
    ~MainMenuScreen();
    bool Init() override;

    void handleEvent(const SDL_Event& e) override;
    void update() override;
    void render(SDL_Renderer* renderer) override;
    void onEnter() override;
    void onExit() override;
};
