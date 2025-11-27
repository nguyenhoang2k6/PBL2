#pragma once
#include <app/Screen.h>
#include <iostream>
#include <SDL3/SDL.h>

class Button;
class Label;
class TextBox;

class NV_NewPass : public Screen
{
private:
    Label* label_newpass;
    Label* label_eror1;
    Label* label_eror2;
    Label* label_success;
    TextBox* textbox_passwordOld;
    TextBox* textbox_passwordNew1;
    TextBox* textbox_passwordNew2;
    Button* button_save;
    Button* button_back;

    bool error1;
    bool error2;
    bool success;
public:
    NV_NewPass(App* app);
    ~NV_NewPass();
    bool Init() override;

    void handleEvent(const SDL_Event& e) override;
    void update() override;
    void render(SDL_Renderer* renderer) override;
    void onEnter() override;
    void onExit() override;
};
