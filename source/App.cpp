#include <app/App.h>
#include <app/MainMenuScreen.h>
#ifdef _WIN32
#include <windows.h>
#endif

App::App() : 
    running(false),
    window(nullptr), 
    renderer(nullptr), 
    font1(nullptr),
    font2(nullptr),
    font3(nullptr)
{
}

bool App::init() {
#ifdef _WIN32
    // Set console to UTF-8 so Vietnamese (UTF-8) output isn't garbled
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "SDL_Init failed: " << SDL_GetError() << std::endl;
        return false;
    }

    if (!TTF_Init()) {
        std::cerr << "TTF_Init failed: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return false;
    }

    window = SDL_CreateWindow("Quan Ly Quan Ca Phe", 800, 600, 0);
    if (!window) {
        std::cerr << "Không tạo được window: " << SDL_GetError() << std::endl;
        TTF_Quit();
        SDL_Quit();
        return false;
    }

    renderer = SDL_CreateRenderer(window, nullptr);
    if (!renderer) {
        std::cerr << "Không tạo được renderer: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return false;
    }
    
    font1 = TTF_OpenFont("fonts/Inter_28pt-Regular.ttf", 30);
    if (!font1) {
        std::cerr << "Không mở được font1: " << SDL_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return false;
    }

    font2 = TTF_OpenFont("fonts/Inter_28pt-Regular.ttf", 45);
    if (!font2) {
        std::cerr << "Không mở được font2: " << SDL_GetError() << std::endl;
        TTF_CloseFont(font1); 
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return false;
    }

    font3 = TTF_OpenFont("fonts/Inter_28pt-Regular.ttf", 60);
    if (!font3) {
        std::cerr << "Không mở được font3: " << SDL_GetError() << std::endl;
        TTF_CloseFont(font1);
        TTF_CloseFont(font2);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return false;

    }
    this->currentScreen = new MainMenuScreen(this);

    if (this->currentScreen == nullptr || currentScreen->Init()==false) {
        std::cerr << "Không thể tạo màn hình MainMenuScreen!" << std::endl;
        return false;
    }
    this->running = true;
    return true;
}

App::~App() {
    if (font1) TTF_CloseFont(font1);
    if (font2) TTF_CloseFont(font2);
    if (font3) TTF_CloseFont(font3);

    if (renderer) SDL_DestroyRenderer(renderer);
    if (window) SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
}

void App::run() {
    if (currentScreen == nullptr) {
        running = false;
        std::cerr << "Không có màn hình nào để chạy!" << std::endl;
    }
    while (running) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) {
                quit();
            } else {
                if (currentScreen) {
                    currentScreen->handleEvent(e);
                }
            }
        }
        if (currentScreen) {
            currentScreen->update();
        }
        SDL_SetRenderDrawColor(renderer, 240, 240, 240, 255);
        SDL_RenderClear(renderer);

        if (currentScreen) {
            currentScreen->render(renderer);
        }
        SDL_RenderPresent(renderer);
    }
}

void App::changeScreen(Screen* newScreen) {
    if (currentScreen != nullptr) {
        delete currentScreen;
        currentScreen = nullptr;
    }

    currentScreen = newScreen;

    if (currentScreen == nullptr) {
        std::cerr << "Lỗi: Màn hình mới là nullptr!" << std::endl;
        quit();
    }
}
