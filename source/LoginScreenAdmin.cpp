#include <app/LoginScreenAdmin.h>
#include <app/App.h>        
#include <app/Button.h>
#include <app/Label.h>
#include <app/TextBox.h>
#include <app/Color.h>
#include <app/PasswordQL.h>

LoginScreenAdmin::LoginScreenAdmin(App* app) :Screen(app) {
    label_LoginAdmin = nullptr;
    label_eror = nullptr;
    textbox_username = nullptr;
    textbox_password = nullptr;
    button_login = nullptr;
    button_back = nullptr;
    loginError = false;

}
bool LoginScreenAdmin::Init() {
    TTF_Font* font1 = app->getFont1();
    TTF_Font* font2 = app->getFont2();
    TTF_Font* font3 = app->getFont3();
    SDL_Renderer* renderer = app->getRenderer();

    if (!font1 || !font2 || !font3 || !renderer) {
        std::cerr << "Lỗi: Không lấy được tài nguyên từ App!" << std::endl;
        return false;
    }

    label_LoginAdmin = new Label("Đăng Nhập Quản trị viên", COLOR_WARNING, 890.0f, 400.0f, font3, renderer);
    label_eror = new Label("Tên đăng nhập hoặc mật khẩu sai, vui lòng nhập lại", COLOR_UI_RED, 700.0f, 880.0f, font2, renderer);  
    textbox_username = new TextBox(966.0f, 600.0f, 800.0f, 100.0f, COLOR_WHITE, COLOR_BLACK, app, font2, "Tên đăng nhập, ví dụ: admin", COLOR_GRAY_SILVER);
    textbox_password = new TextBox(966.0f, 760.0f, 800.0f, 100.0f, COLOR_WHITE, COLOR_BLACK, app, font2, "Mật khẩu", COLOR_GRAY_SILVER);
    button_login = new Button(966.0f, 1000.0f, 350.0f, 80.0f, COLOR_UI_BLUE, "Đăng Nhập", renderer, font2, COLOR_WHITE);
    button_back = new Button(1416.0f, 1000.0f, 350.0f, 80.0f, COLOR_UI_RED, "Quay Lại", renderer, font2, COLOR_WHITE);

    if (!label_LoginAdmin ||!label_eror ||!textbox_username || !textbox_password || !button_login || !button_back) {
        std::cerr << "Tạo UI thất bại!" << std::endl;
        return false;
    }
    return true;
}



LoginScreenAdmin::~LoginScreenAdmin() {
    delete label_LoginAdmin;
    delete label_eror;
    delete textbox_username;
    delete textbox_password;
    delete button_login;
    delete button_back;
}


void LoginScreenAdmin::handleEvent(const SDL_Event& e) {
    if (textbox_username) {
        textbox_username->handleEvent(e);
    }
    if (textbox_password) {
        textbox_password->handleEvent(e);
    }
    if (button_login) {
        button_login->handleEvent(e);
    }
    if (button_back) {
        button_back->handleEvent(e);
    }

    if(button_back->isClicked()) {
        app->changeScreen("MainMenu");
    }
    if(button_login->isClicked()) {
        std::string username = textbox_username->getText();
        std::string password = textbox_password->getText();
        if(!Manager(username, password)) {
            loginError = true;
            return;
        } else {
            app->changeScreen("AdminDashBoard");
        }
        
    }
}

void LoginScreenAdmin::update() {
    if (textbox_username) {
        textbox_username->update();
    }
    if (textbox_password) {
        textbox_password->update();
    }
    if (button_login) {
        button_login->update();
    }
    if (button_back) {
        button_back->update();
    }
}

void LoginScreenAdmin::render(SDL_Renderer* renderer) {
    if (label_LoginAdmin) {
        label_LoginAdmin->render(renderer);
    }
    if (label_eror && loginError) {
        label_eror->render(renderer);
    }
    if (textbox_username) {
        textbox_username->render(renderer);
    }
    if (textbox_password) {
        textbox_password->render(renderer);
    }
    if (button_login) {
        button_login->render(renderer);
    }
    if (button_back) {
        button_back->render(renderer);
    }
}

void LoginScreenAdmin::onEnter() {
    textbox_username->setText("");
    textbox_password->setText("");
    loginError = false;
}

void LoginScreenAdmin::onExit() {
}