#pragma once
#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <app/Screen.h> 

class App
{
private:
    bool running;
    SDL_Window* window;
    SDL_Renderer* renderer;
    Screen* currentScreen; 

    TTF_Font* font1;
    TTF_Font* font2;
    TTF_Font* font3;

public:
    App();
    ~App();
    
    bool init(); 
    
    void run();          
    void changeScreen(Screen* newScreen);
    void quit() { running = false; }

    SDL_Renderer* getRenderer() const { return renderer; }

    TTF_Font* getFont1() const { return font1;}
    TTF_Font* getFont2() const { return font2;}
    TTF_Font* getFont3() const { return font3;}

    SDL_Window* getWindow() const { return window; }
};