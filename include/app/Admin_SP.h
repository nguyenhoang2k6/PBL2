#pragma once
#include <app/Screen.h>
#include <iostream>
#include <SDL3/SDL.h>
#include <app/ProductListView.h>

class Button;
class Label;
class TextBox;

class Admin_SP : public Screen
{
private:
    ProductListView* productListView;
    Button* button_back;
    Button* button_add;
public:
    Admin_SP(App* app);
    ~Admin_SP();
    bool Init() override;

    void handleEvent(const SDL_Event& e) override;
    void update() override;
    void render(SDL_Renderer* renderer) override;
    void onEnter() override;
    void onExit() override;
};
