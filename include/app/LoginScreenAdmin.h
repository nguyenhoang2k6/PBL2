#pragma once
#include <app/Screen.h>
#include <iostream>
#include <SDL3/SDL.h>

class Button;
class Label;
class TextBox;

class LoginScreenAdmin : public Screen
{
private:
    Label* label_LoginAdmin;
    Label* label_eror;
    TextBox* textbox_username;
    TextBox* textbox_password;
    Button* button_login;
    Button* button_back;

    bool loginError;
public:
    LoginScreenAdmin(App* app);
    ~LoginScreenAdmin();
    bool Init() override;

    void handleEvent(const SDL_Event& e) override;
    void update() override;
    void render(SDL_Renderer* renderer) override;
    void onEnter() override;
    void onExit() override;
};
