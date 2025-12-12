#pragma once
#include <app/Screen.h>
#include <iostream>
#include <SDL3/SDL.h>
#include <string>

class Button;
class Label;
class TextBox;

class Admin_AddEmploy : public Screen
{
private:
    // Labels
    Label* label_title;
    Label* label_maNV;
    Label* label_ten;
    Label* label_chucVu;
    Label* label_tuoi;
    Label* label_diaChi;
    Label* label_sdt;
    Label* label_password;
    Label* label_errorMaNV;
    Label* label_errorTen;
    Label* label_errorChucVu;
    Label* label_errorTuoi;
    Label* label_errorDiaChi;
    Label* label_errorSDT;
    Label* label_errorPassword;
    Label* label_success;

    // TextBoxes
    TextBox* textbox_maNV;
    TextBox* textbox_ten;
    TextBox* textbox_chucVu;
    TextBox* textbox_tuoi;
    TextBox* textbox_diaChi;
    TextBox* textbox_sdt;
    TextBox* textbox_password;

    // Buttons
    Button* button_save;
    Button* button_back;

    // Error flags
    bool errorMaNV;
    bool errorTen;
    bool errorChucVu;
    bool errorTuoi;
    bool errorDiaChi;
    bool errorSDT;
    bool errorPassword;
    bool success;

public:
    Admin_AddEmploy(App* app);
    ~Admin_AddEmploy();
    bool Init() override;

    void handleEvent(const SDL_Event& e) override;
    void update() override;
    void render(SDL_Renderer* renderer) override;
    void onEnter() override;
    void onExit() override;

private:
    bool validateInput();
    bool saveEmployee();
    void clearForm();
};
