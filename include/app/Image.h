#pragma once
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <string>
#include <iostream>

class Image {
private:
    SDL_Texture* texture;
    SDL_FRect dstRect;

public:
    Image(SDL_Renderer* renderer, std::string path, float x, float y, float w, float h) {
        texture = IMG_LoadTexture(renderer, path.c_str());
        
        if (texture == nullptr) {
            // Hoặc in lỗi ra console để debug
            std::cerr << "Khong load duoc anh: " << path << " - Loi: " << SDL_GetError() << std::endl;
        }

        dstRect = { x, y, w, h };
    }

    ~Image() {
        if (texture != nullptr) {
            SDL_DestroyTexture(texture);
        }
    }

    void setPosition(float x, float y) {
        dstRect.x = x;
        dstRect.y = y;
    }

    void render(SDL_Renderer* renderer) {
        if (texture != nullptr) {
            SDL_RenderTexture(renderer, texture, nullptr, &dstRect);
        } else {
            SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
            SDL_RenderRect(renderer, &dstRect);
        }
    }
};
