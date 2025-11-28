#include <app/AdminDashBoard.h>
#include <app/App.h>        
#include <app/Button.h>
#include <app/Label.h>
#include <app/TextBox.h>
#include <app/Color.h>

AdminDashBoard::AdminDashBoard(App* app) :Screen(app) {
    label_title = nullptr;
    button_nv = nullptr;
    button_item = nullptr;
    button_thongke = nullptr;
    button_doimk = nullptr;
    button_back = nullptr;
}
bool AdminDashBoard::Init() {
    TTF_Font* font1 = app->getFont1();
    TTF_Font* font2 = app->getFont2();
    TTF_Font* font3 = app->getFont3();
    SDL_Renderer* renderer = app->getRenderer();

    if (!font1 || !font2 || !font3 || !renderer) {
        std::cerr << "Lỗi: Không lấy được tài nguyên từ App!" << std::endl;
        return false;
    }

    label_title = new Label("Bảng Điều Khiển Quản Trị Viên", COLOR_WARNING, 675.0f, 300.0f, font3, renderer);
    button_nv = new Button(720.0f, 550.0f, 500.0f, 175.0f, COLOR_UI_ORANGE, "Nhân Viên", renderer, font2, COLOR_WHITE);
    button_item = new Button(720.0f, 875.0f, 500.0f, 175.0f, COLOR_UI_BLUE, "Sản Phẩm", renderer, font2, COLOR_WHITE);
    button_thongke = new Button(1512.0f, 550.0f, 500.0f, 175.0f, COLOR_UI_PURPLE, "Thống Kê", renderer, font2, COLOR_WHITE);
    button_doimk = new Button(1512.0f, 875.0f, 500.0f, 175.0f, COLOR_UI_GREEN, "Đổi Mật Khẩu", renderer, font2, COLOR_WHITE);
    button_back = new Button(1116.0f, 1200.0f, 500.0f, 175.0f, COLOR_UI_RED, "Đăng Xuất", renderer, font2, COLOR_WHITE);


    if (!label_title ||!button_nv ||!button_item ||!button_thongke ||!button_doimk ||!button_back) {
        std::cerr << "Tạo UI thất bại!" << std::endl;
        return false;
    }
    return true;
}



AdminDashBoard::~AdminDashBoard() {
    delete label_title;
    delete button_nv;
    delete button_item; 
    delete button_thongke;
    delete button_doimk;
    delete button_back;
}

void AdminDashBoard::handleEvent(const SDL_Event& e) {
    if(button_nv){button_nv->handleEvent(e);}
    if(button_item){button_item->handleEvent(e);}
    if(button_thongke){button_thongke->handleEvent(e);}
    if(button_doimk){button_doimk->handleEvent(e);}
    if(button_back){button_back->handleEvent(e);}

    if(button_back->isClicked()) {
        app->changeScreen("MainMenu");
    }
    if(button_doimk->isClicked()) {
        app->changeScreen("Admin_NewPass");
    }
    if(button_nv->isClicked()) {
        app->changeScreen("Admin_NV");
    }
    if(button_thongke->isClicked()) {
        app->changeScreen("Admin_ThongKe");
    }
    if(button_item->isClicked()) {
        app->changeScreen("Admin_SP");
    }
}

void AdminDashBoard::update() {
    if(button_nv){button_nv->update();}
    if(button_item){button_item->update();}
    if(button_thongke){button_thongke->update();}
    if(button_doimk){button_doimk->update();}
    if(button_back){button_back->update();}
}

void AdminDashBoard::render(SDL_Renderer* renderer) {
    if(label_title){label_title->render(renderer);}
    if(button_nv){button_nv->render(renderer);}
    if(button_item){button_item->render(renderer);}
    if(button_thongke){button_thongke->render(renderer);}
    if(button_doimk){button_doimk->render(renderer);}
    if(button_back){button_back->render(renderer);}
}

void AdminDashBoard::onEnter() {

}

void AdminDashBoard::onExit() {

}