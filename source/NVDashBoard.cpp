#include <app/NVDashBoard.h>
#include <app/App.h>        
#include <app/Button.h>
#include <app/Label.h>
#include <app/TextBox.h>
#include <app/Color.h>

// Khởi tạo màn hình bảng điều khiển nhân viên
NVDashBoard::NVDashBoard(App* app) :Screen(app) {
    label_title = nullptr;
    button_item = nullptr;
    button_hoadon = nullptr;
    button_doimk = nullptr;
    button_back = nullptr;
}
// Khởi tạo các nút chức năng trên dashboard nhân viên
bool NVDashBoard::Init() {
    TTF_Font* font1 = app->getFont1();
    TTF_Font* font2 = app->getFont2();
    TTF_Font* font3 = app->getFont3();
    SDL_Renderer* renderer = app->getRenderer();

    if (!font1 || !font2 || !font3 || !renderer) {
        std::cerr << "Lỗi: Không lấy được tài nguyên từ App!" << std::endl;
        return false;
    }

    label_title = new Label("Bảng Điều Khiển Nhân Viên", COLOR_WARNING, 750.0f, 300.0f, font3, renderer);
    button_item = new Button(720.0f, 550.0f, 500.0f, 175.0f, COLOR_UI_BLUE, "Sản Phẩm", renderer, font2, COLOR_WHITE);
    button_hoadon = new Button(720.0f, 875.0f, 500.0f, 175.0f, COLOR_UI_TURQUOISE, "Xuất Hóa Đơn", renderer, font2, COLOR_WHITE);
    button_doimk = new Button(1512.0f, 550.0f, 500.0f, 175.0f, COLOR_UI_GREEN, "Đổi Mật Khẩu", renderer, font2, COLOR_WHITE);
    button_back = new Button(1512.0f, 875.0f, 500.0f, 175.0f, COLOR_UI_RED, "Đăng Xuất", renderer, font2, COLOR_WHITE);

    if (!label_title ||!button_item ||!button_hoadon ||!button_doimk ||!button_back) {
        std::cerr << "Tạo UI thất bại!" << std::endl;
        return false;
    }
    return true;
}

// Giải phóng tài nguyên dashboard nhân viên
NVDashBoard::~NVDashBoard() {
    delete label_title;
    delete button_item; 
    delete button_hoadon;
    delete button_doimk;
    delete button_back;
}

// Xử lý sự kiện và điều hướng trên dashboard nhân viên
void NVDashBoard::handleEvent(const SDL_Event& e) {
    if(button_item){button_item->handleEvent(e);}
    if(button_hoadon){button_hoadon->handleEvent(e);}
    if(button_doimk){button_doimk->handleEvent(e);}
    if(button_back){button_back->handleEvent(e);}

    if(button_item->isClicked()) {
        app->changeScreen("NVSP");
    }
    if(button_hoadon->isClicked()) {
        app->changeScreen("NVHD");
    }
    if(button_doimk->isClicked()) {
        app->changeScreen("NV_NewPass");
    }
    if(button_back->isClicked()) {
        app->changeScreen("MainMenu");
    }
}

// Cập nhật trạng thái các nút trên dashboard
void NVDashBoard::update() {
    if(button_item){button_item->update();}
    if(button_hoadon){button_hoadon->update();}
    if(button_doimk){button_doimk->update();}
    if(button_back){button_back->update();}
}

// Vẽ giao diện dashboard nhân viên
void NVDashBoard::render(SDL_Renderer* renderer) {
    if(label_title){label_title->render(renderer);}
    if(button_item){button_item->render(renderer);}
    if(button_hoadon){button_hoadon->render(renderer);}
    if(button_doimk){button_doimk->render(renderer);}
    if(button_back){button_back->render(renderer);}
}

// Xử lý khi vào màn hình dashboard nhân viên
void NVDashBoard::onEnter() {

}

// Xử lý khi rời màn hình dashboard nhân viên
void NVDashBoard::onExit() {

}
