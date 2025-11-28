#ifdef _WIN32
#include <windows.h>
#include <iostream>
#endif
#include <app/App.h>
#include <app/MainMenuScreen.h>
#include <app/LoginScreenNV.h>
#include <app/LoginScreenAdmin.h>
#include <app/NVDashBoard.h>
#include <app/AdminDashBoard.h>
#include <app/NVSP.h>
#include <app/NVHD.h>
#include <app/NV_NewPass.h>
#include <app/Admin_NewPass.h>
#include <app/Admin_NV.h>
#include <app/Admin_ThongKe.h>
#include <app/Admin_SP.h>
#include <app/Color.h>

App::App() : 
    running(false),
    window(nullptr), 
    renderer(nullptr),
    currentScreen(nullptr),
    font1(nullptr),
    font2(nullptr),
    font3(nullptr),
    current_username(""),
    current_pasword("")
{
}

bool App::init() {
#ifdef _WIN32
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

    window = SDL_CreateWindow("Quản Lí Quán Cà Phê", 2732, 1536, SDL_WINDOW_RESIZABLE);
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
    
    font1 = TTF_OpenFont("fonts/Inter_28pt-Regular.ttf", 45);
    if (!font1) {
        std::cerr << "Không mở được font1: " << SDL_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return false;
    }

    font2 = TTF_OpenFont("fonts/Inter_28pt-Regular.ttf", 60);
    if (!font2) {
        std::cerr << "Không mở được font2: " << SDL_GetError() << std::endl;
        TTF_CloseFont(font1); 
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return false;
    }

    font3 = TTF_OpenFont("fonts/Inter_28pt-Regular.ttf", 100);
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
    Screen* mainMenu = new MainMenuScreen(this);
    if (!mainMenu->Init()) {
        std::cerr << "Không khởi tạo được MainMenuScreen" << std::endl;
        delete mainMenu;
        TTF_CloseFont(font1);
        TTF_CloseFont(font2);
        TTF_CloseFont(font3);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return false;
    }
    Screen* loginNV = new LoginScreenNV(this);
    if (!loginNV->Init()) {
        std::cerr << "Không khởi tạo được LoginScreenNV" << std::endl;
        delete loginNV;
        delete mainMenu;
        TTF_CloseFont(font1);
        TTF_CloseFont(font2);
        TTF_CloseFont(font3);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return false;
    }
    Screen* loginAdmin = new LoginScreenAdmin(this);
    if (!loginAdmin->Init()) {
        std::cerr << "Không khởi tạo được LoginScreenAdmin" << std::endl;
        delete loginAdmin;
        delete loginNV;
        delete mainMenu;
        TTF_CloseFont(font1);
        TTF_CloseFont(font2);
        TTF_CloseFont(font3);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return false;
    }
    Screen* nvDashBoard = new NVDashBoard(this);
    if (!nvDashBoard->Init()) {
        std::cerr << "Không khởi tạo được NVDashBoard" << std::endl;
        delete nvDashBoard;
        delete loginAdmin;
        delete loginNV;
        delete mainMenu;
        TTF_CloseFont(font1);
        TTF_CloseFont(font2);
        TTF_CloseFont(font3);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return false;
    }
    Screen* adminDashBoard = new AdminDashBoard(this);
    if (!adminDashBoard->Init()) {
        std::cerr << "Không khởi tạo được AdminDashBoard" << std::endl;
        delete adminDashBoard;
        delete nvDashBoard;
        delete loginAdmin;
        delete loginNV;
        delete mainMenu;
        TTF_CloseFont(font1);
        TTF_CloseFont(font2);
        TTF_CloseFont(font3);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return false;
    }
    Screen* nvsp = new NVSP(this);
    if (!nvsp->Init()) {
        std::cerr << "Không khởi tạo được NVSP" << std::endl;
        delete nvsp;
        delete adminDashBoard;
        delete nvDashBoard;
        delete loginAdmin;
        delete loginNV;
        delete mainMenu;
        TTF_CloseFont(font1);
        TTF_CloseFont(font2);
        TTF_CloseFont(font3);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return false;
    }
    Screen* nvhd = new NVHD(this);
    if (!nvhd->Init()) {
        std::cerr << "Không khởi tạo được NVHD" << std::endl;
        delete nvhd;
        delete nvsp;
        delete adminDashBoard;
        delete nvDashBoard;
        delete loginAdmin;
        delete loginNV;
        delete mainMenu;
        TTF_CloseFont(font1);
        TTF_CloseFont(font2);
        TTF_CloseFont(font3);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return false;
    }
    Screen* nvNewPass = new NV_NewPass(this);
    if (!nvNewPass->Init()) {
        std::cerr << "Không khởi tạo được NV_NewPass" << std::endl;
        delete nvNewPass;
        delete nvhd;
        delete nvsp;
        delete adminDashBoard;
        delete nvDashBoard;
        delete loginAdmin;
        delete loginNV;
        delete mainMenu;
        TTF_CloseFont(font1);
        TTF_CloseFont(font2);
        TTF_CloseFont(font3);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return false;
    }
    Screen* adminNewPass = new Admin_NewPass(this);
    if (!adminNewPass->Init()) {
        std::cerr << "Không khởi tạo được Admin_NewPass" << std::endl;
        delete adminNewPass;
        delete nvNewPass;
        delete nvhd;
        delete nvsp;
        delete adminDashBoard;
        delete nvDashBoard;
        delete loginAdmin;
        delete loginNV;
        delete mainMenu;
        TTF_CloseFont(font1);
        TTF_CloseFont(font2);
        TTF_CloseFont(font3);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return false;
    }
    Screen* adminNV = new Admin_NV(this);
    if (!adminNV->Init()) {
        std::cerr << "Không khởi tạo được Admin_NV" << std::endl;
        delete adminNV;
        delete adminNewPass;
        delete nvNewPass;
        delete nvhd;
        delete nvsp;
        delete adminDashBoard;
        delete nvDashBoard;
        delete loginAdmin;
        delete loginNV;
        delete mainMenu;
        TTF_CloseFont(font1);
        TTF_CloseFont(font2);
        TTF_CloseFont(font3);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return false;
    }
    Screen* adminThongKe = new Admin_ThongKe(this);
    if (!adminThongKe->Init()) {
        std::cerr << "Không khởi tạo được Admin_ThongKe" << std::endl;
        delete adminThongKe;
        delete adminNV;
        delete adminNewPass;
        delete nvNewPass;
        delete nvhd;
        delete nvsp;
        delete adminDashBoard;
        delete nvDashBoard;
        delete loginAdmin;
        delete loginNV;
        delete mainMenu;
        TTF_CloseFont(font1);
        TTF_CloseFont(font2);
        TTF_CloseFont(font3);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return false;
    }
    Screen* adminSP = new Admin_SP(this);
    if (!adminSP->Init()) {
        std::cerr << "Không khởi tạo được Admin_SP" << std::endl;
        delete adminSP;
        delete adminThongKe;
        delete adminNV;
        delete adminNewPass;
        delete nvNewPass;
        delete nvhd;
        delete nvsp;
        delete adminDashBoard;
        delete nvDashBoard;
        delete loginAdmin;
        delete loginNV;
        delete mainMenu;
        TTF_CloseFont(font1);
        TTF_CloseFont(font2);
        TTF_CloseFont(font3);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return false;
    }
    ScreenCache["Admin_SP"] = adminSP;
    ScreenCache["Admin_ThongKe"] = adminThongKe;
    ScreenCache["Admin_NV"] = adminNV;
    ScreenCache["Admin_NewPass"] = adminNewPass;
    ScreenCache["NV_NewPass"] = nvNewPass;
    ScreenCache["NVHD"] = nvhd;
    ScreenCache["NVSP"] = nvsp;
    ScreenCache["AdminDashBoard"] = adminDashBoard;
    ScreenCache["NVDashBoard"] = nvDashBoard;
    ScreenCache["LoginAdmin"] = loginAdmin;
    ScreenCache["LoginNV"] = loginNV;
    ScreenCache["MainMenu"] = mainMenu;
    // còn các màn hình khác sẽ được khởi tạo tương tự ở đây và thêm vào ScreenCache
    currentScreen = ScreenCache["MainMenu"];
    this->running = true;
    return true;
}

App::~App() {
    for (auto& pair : ScreenCache) {
        delete pair.second;
    }
    ScreenCache.clear();

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
        if(!running) break;
        if (currentScreen) {
            currentScreen->update();
        }
        if(!running) break;
        SDL_SetRenderDrawColor(renderer,235, 235, 210, 255);
        SDL_RenderClear(renderer);

        if (currentScreen) {
            currentScreen->render(renderer);
        }
        SDL_RenderPresent(renderer);
    }
}

void App::changeScreen(const std::string& screenName) {
    if(currentScreen) {
        currentScreen->onExit();
    }
    if(ScreenCache.find(screenName) != ScreenCache.end()) {
        currentScreen = ScreenCache[screenName];
    } else {
        std::cerr << "Màn hình " << screenName << " không tồn tại trong bộ nhớ đệm." << std::endl;
    }
    if(currentScreen) {
        currentScreen->onEnter();
    }
}
