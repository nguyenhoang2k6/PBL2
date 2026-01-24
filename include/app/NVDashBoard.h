#pragma once
#include <app/Screen.h>
#include <iostream>
#include <SDL3/SDL.h>

class Button;
class Label;
class TextBox;

class NVDashBoard : public Screen
{
private:
    Label* label_title;
    Button* button_item;
    Button* button_hoadon;
    Button* button_doimk;
    Button* button_back;

public:
    NVDashBoard(App* app);
    ~NVDashBoard();
    bool Init() override;

    void handleEvent(const SDL_Event& e) override;
    void update() override;
    void render(SDL_Renderer* renderer) override;
    void onEnter() override;
    void onExit() override;
};
