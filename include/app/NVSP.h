#pragma once
#include <app/Screen.h>
#include <iostream>
#include <SDL3/SDL.h>
#include <app/ProductListView.h>

class Button;
class Label;
class TextBox;

class NVSP : public Screen
{
private:
    ProductListView* productListView;

public:
    NVSP(App* app);
    ~NVSP();
    bool Init() override;

    void handleEvent(const SDL_Event& e) override;
    void update() override;
    void render(SDL_Renderer* renderer) override;
    void onEnter() override;
    void onExit() override;
};
