#pragma once
#include <app/Screen.h>
#include <iostream>
#include <SDL3/SDL.h>
#include <string>

class Button;
class Label;
class TextBox;

class Admin_AddProduct : public Screen
{
private:
    // Labels
    Label* label_title;
    Label* label_productCode;
    Label* label_productName;
    Label* label_price;
    Label* label_errorCode;
    Label* label_errorName;
    Label* label_errorPrice;
    Label* label_success;
    Label* label_image;

    // TextBoxes
    TextBox* textbox_code;
    TextBox* textbox_name;
    TextBox* textbox_price;

    // Buttons
    Button* button_save;
    Button* button_back;
    Button* button_addImage;

    // Image path for selected image
    std::string imagePath;
    SDL_Texture* imageTexture;

    // Error flags
    bool errorCode;
    bool errorName;
    bool errorPrice;
    bool success;

public:
    Admin_AddProduct(App* app);
    ~Admin_AddProduct();
    bool Init() override;

    void handleEvent(const SDL_Event& e) override;
    void update() override;
    void render(SDL_Renderer* renderer) override;
    void onEnter() override;
    void onExit() override;

private:
    bool validateInput();
    bool saveProduct();
    void clearForm();
};
