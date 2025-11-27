#pragma once
#include <app/Screen.h>
#include <iostream>
#include <SDL3/SDL.h>

class Button;
class Label;
class TextBox;

class LoginScreenNV : public Screen
{
private:
    Label* label_LoginNV;
    Label* label_eror;
    TextBox* textbox_username;
    TextBox* textbox_password;
    Button* button_login;
    Button* button_back;

    bool loginError;
public:
    LoginScreenNV(App* app);
    ~LoginScreenNV();
    bool Init() override;

    void handleEvent(const SDL_Event& e) override;
    void update() override;
    void render(SDL_Renderer* renderer) override;
    void onEnter() override;
    void onExit() override;
};
