#include <app/MainMenuScreen.h>
#include <app/App.h>        
#include <app/Button.h>
#include <app/Label.h>
#include <app/TextBox.h>
#include <app/Color.h>

// Khởi tạo màn hình menu chính
MainMenuScreen::MainMenuScreen(App* app) :Screen(app) {
    button_NV = nullptr;
    button_admin = nullptr;
    button_exit = nullptr;

}
// Khởi tạo các nút menu chính
bool MainMenuScreen::Init() {
    TTF_Font* font1 = app->getFont1();
    TTF_Font* font2 = app->getFont2();
    TTF_Font* font3 = app->getFont3();
    SDL_Renderer* renderer = app->getRenderer();

    if (!font1 || !font2 || !font3 || !renderer) {
        std::cerr << "Lỗi: Không lấy được tài nguyên từ App!" << std::endl;
        return false;
    }

    button_NV = new Button(300, 600, 470, 220,COLOR_UI_BLUE,"Đăng nhập với quyền nhân viên",renderer,font2,COLOR_WHITE);
    button_admin = new Button(1962, 600, 470, 220, COLOR_UI_GREEN,"Đăng nhập với quyền admin",renderer,font2,COLOR_WHITE);
    button_exit = new Button(2432, 1436, 300, 100, COLOR_UI_RED, "Thoát", renderer, font2, COLOR_WHITE);

    if (!button_NV || !button_admin || !button_exit) {
        std::cerr << "Tạo UI thất bại!" << std::endl;
        return false;
    }
    return true;
}

// Giải phóng tài nguyên màn hình menu
MainMenuScreen::~MainMenuScreen() {
    delete button_NV;
    delete button_admin;
    delete button_exit;
}

// Xử lý sự kiện cho các nút menu
void MainMenuScreen::handleEvent(const SDL_Event& e) {
    if(button_NV){button_NV->handleEvent(e);}
    if(button_admin){button_admin->handleEvent(e);}
    if (button_exit) { button_exit->handleEvent(e); }

    if (button_NV && button_NV->isClicked()) {
        app->changeScreen("LoginNV");
    }
    if (button_admin && button_admin->isClicked()) {
        app->changeScreen("LoginAdmin");
    }
    if (button_exit && button_exit->isClicked()) {
        app->quit();
    }
}

// Cập nhật trạng thái nút mỗi frame
void MainMenuScreen::update() {
    if(button_NV){button_NV->update();}
    if(button_admin){button_admin->update();}
    if (button_exit) { button_exit->update(); }
}

// Vẽ các nút trên menu chính
void MainMenuScreen::render(SDL_Renderer* renderer) {
    if (button_NV) {
        button_NV->render(renderer);
    }
    if (button_admin) {
        button_admin->render(renderer);
    }
    if (button_exit) {
        button_exit->render(renderer);
    }
}

// Xử lý khi vào màn hình menu
void MainMenuScreen::onEnter() {

}

// Xử lý khi rời màn hình menu
void MainMenuScreen::onExit() {

}
