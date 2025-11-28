#pragma once
#include <app/Screen.h>
#include <iostream>
#include <SDL3/SDL.h>
#include <app/EmployeeTable.h>

class Button;
class Label;
class TextBox;

class Admin_NV : public Screen
{
private:
    EmployeeTable* m_table;
    Button* button_add;
    Button* button_back;

public:
    Admin_NV(App* app);
    ~Admin_NV();
    bool Init() override;

    void handleEvent(const SDL_Event& e) override;
    void update() override;
    void render(SDL_Renderer* renderer) override;
    void onEnter() override;
    void onExit() override;
};
