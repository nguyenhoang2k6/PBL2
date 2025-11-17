#pragma once
#include <iostream>
#include <string>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
using namespace std;

class Button {
private:
    SDL_FRect rect;
    SDL_Color bgColor;
    string label;
    bool hovered;
    bool pressed;
    bool wasClicked;

    SDL_Texture* textTexture;
    TTF_Font* font;
    SDL_Color textColor;
    SDL_Renderer* renderer;

public:
    Button(float x, float y, float w, float h, SDL_Color color,const string& text, SDL_Renderer* renderer, TTF_Font* f, SDL_Color tColor);
    ~Button();

    void render(SDL_Renderer* renderer);
    void handleEvent(const SDL_Event& e);
    void update();

    bool isClicked() const;
    bool isHovered(float mouseX, float mouseY) const;

    void setLabel(const string& text);
    void setColor(SDL_Color color);
    string getLabel() const;
};

