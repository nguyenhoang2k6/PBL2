#pragma once
#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3/SDL_ttf.h>
#include <string>
using namespace std;

class Button {
private:
    SDL_FRect rect;
    SDL_Color bgColor;
    string label;
    bool hovered;
    bool pressed;

    SDL_Texture* textTexture;
    TTF_Font* font;
    SDL_Color textColor;

public:
    Button(float x, float y, float w, float h, SDL_Color color,string text, SDL_Renderer* renderer, TTF_Font* f, SDL_Color tColor);
    ~Button();

    void render(SDL_Renderer* renderer);
    void handleEvent(const SDL_Event& e);

    bool isClicked(int mouseX, int mouseY) const;
    bool isHovered(int mouseX, int mouseY) const;

    void setLabel(const string& text);
    void setColor(SDL_Color color);
    string getLabel() const;
};

