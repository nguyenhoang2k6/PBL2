#pragma once
#include <iostream>
#include <string>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <app/App.h>

using namespace std;

class TextBox
{
private:
    App* app;
    SDL_FRect rect;
    SDL_Color bgColor;
    SDL_Color textColor;
    string text;

    SDL_Texture* textTexture;
    int texW, texH;

    bool focused;
    TTF_Font* font;
    Uint32 lastBlink;
    bool caretVisible;
    int caretPos;
    
    string placeholder;
    SDL_Color placeholderColor;
    SDL_Texture* placeholderTexture;

    SDL_Renderer* renderer;
    bool needsTextureUpdate;    
    float scrollOffset;

    void updateTexture();
public:
    TextBox(float x, float y, float w, float h, SDL_Color bgColor, SDL_Color textColor,App* app,TTF_Font* font,const string& placeholder, SDL_Color placeholderColor);
    ~TextBox();
    void handleEvent(const SDL_Event& e);
    void update();
    void render(SDL_Renderer* renderer);

    string getText() const {return text;}
    void setText(const string& t) {
        text = t;
        caretPos = (int)text.length();
        needsTextureUpdate = true;
    }
    void setFocus(bool f) {focused = f;}
    bool isFocused() {return focused;}
};
