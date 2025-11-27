#pragma once
#include <iostream>
#include <string>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
using namespace std;

class Label
{
private:
    string text;
    SDL_Color textColor;
    float x,y;
    TTF_Font* font;
    SDL_Texture* textTexture;
    int texW,texH;
    SDL_Renderer* renderer;
public:
    Label(const string& text, SDL_Color textColor,float x,float y,TTF_Font* font, SDL_Renderer* renderer);
    ~Label();
    void setText(const string& newText);
    void setPosition(float x, float y) {
        this->x = x;
        this->y = y;
    }
    void render(SDL_Renderer* renderer) const;
};

