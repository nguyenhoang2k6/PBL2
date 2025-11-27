#pragma once
#include <iostream>
#include <string>
#include <map>
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
    std::string current_username;
    std::string current_pasword;
    std::map<std::string, Screen*> ScreenCache;

    TTF_Font* font1;
    TTF_Font* font2;
    TTF_Font* font3;

public:
    App();
    ~App();
    
    bool init(); 
    
    void run();          
    void changeScreen(const std::string& screenName);
    void quit() { running = false; }

    SDL_Renderer* getRenderer() const { return renderer; }

    std::string getCurrentUsername() const { return current_username; }
    void setCurrentUsername(const std::string& username) { current_username = username; }
    std::string getCurrentPassword() const { return current_pasword; }
    void setCurrentPassword(const std::string& password) { current_pasword = password; }

    TTF_Font* getFont1() const { return font1;}
    TTF_Font* getFont2() const { return font2;}
    TTF_Font* getFont3() const { return font3;}

    SDL_Window* getWindow() const { return window; }
};