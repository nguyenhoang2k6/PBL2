#include <SDL3/SDL.h>
#include <SDL3/SDL_ttf.h>
#include <iostream>
#include "Button.h"

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    if (TTF_Init() != 0) {
        std::cout << "TTF_Init Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Test Button", 800, 600, SDL_WINDOW_RESIZABLE);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);

    if (!window || !renderer) {
        std::cout << "SDL_CreateWindow/Renderer Error: " << SDL_GetError() << std::endl;
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    TTF_Font* font = TTF_OpenFont("C:/Windows/Fonts/arial.ttf", 24);
    if (!font) {
        std::cout << "TTF_OpenFont Error: " << SDL_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    // Tạo 1 nút
    Button btn(300, 250, 200, 60,
               SDL_Color{0, 200, 0, 255},
               "Click Me",
               renderer, font,
               SDL_Color{255, 255, 255, 255});

    bool running = true;
    SDL_Event e;

    // ✅ Vòng lặp chính
    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) {
                running = false; // tắt cửa sổ
            }
        }

        // Clear màn hình
        SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
        SDL_RenderClear(renderer);

        // Vẽ nút
        btn.render(renderer);

        // Cập nhật màn hình
        SDL_RenderPresent(renderer);

        SDL_Delay(16); // ~60fps
    }

    // Dọn dẹp
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
    return 0;
}
