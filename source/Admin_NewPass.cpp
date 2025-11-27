#include <app/Admin_NewPass.h>
#include <app/App.h>        
#include <app/Button.h>
#include <app/Label.h>
#include <app/TextBox.h>
#include <app/Color.h>
#include <app/PasswordQL.h>

Admin_NewPass::Admin_NewPass(App* app) :Screen(app) {
    label_newpass = nullptr;
    label_eror1 = nullptr;
    label_eror2 = nullptr;
    label_success = nullptr;
    textbox_passwordOld = nullptr;
    textbox_passwordNew1 = nullptr;
    textbox_passwordNew2 = nullptr;
    button_save = nullptr;
    button_back = nullptr;
    error1 = false;
    error2 = false;
    success = false;

}
bool Admin_NewPass::Init() {
    TTF_Font* font1 = app->getFont1();
    TTF_Font* font2 = app->getFont2();
    TTF_Font* font3 = app->getFont3();
    SDL_Renderer* renderer = app->getRenderer();

    if (!font1 || !font2 || !font3 || !renderer) {
        std::cerr << "Lỗi: Không lấy được tài nguyên từ App!" << std::endl;
        return false;
    }

    label_newpass = new Label("Đổi mật khẩu quản trị viên", COLOR_WARNING, 850.0f, 400.0f, font3, renderer);
    label_eror1 = new Label("Mật khẩu cũ nhập sai, vui lòng nhập lại", COLOR_UI_RED, 850.0f, 1075.0f, font2, renderer);
    label_eror2 = new Label("2 lần nhập mật khẩu không trùng khớp, vui lòng nhập lại", COLOR_UI_RED, 600.0f, 1075.0f, font2, renderer);  
    label_success = new Label("Đổi mật khẩu thành công!", COLOR_UI_GREEN, 1000.0f, 1075.0f, font2, renderer);
    textbox_passwordOld = new TextBox(966.0f, 600.0f, 800.0f, 100.0f, COLOR_WHITE, COLOR_BLACK, app, font2, "Nhập mật khẩu cũ", COLOR_GRAY_SILVER);
    textbox_passwordNew1 = new TextBox(966.0f, 760.0f, 800.0f, 100.0f, COLOR_WHITE, COLOR_BLACK, app, font2, "Nhập mật khẩu mới", COLOR_GRAY_SILVER);
    textbox_passwordNew2 = new TextBox(966.0f, 920.0f, 800.0f, 100.0f, COLOR_WHITE, COLOR_BLACK, app, font2, "Nhập lại mật khẩu mới", COLOR_GRAY_SILVER);
    button_save = new Button(966.0f, 1200.0f, 350.0f, 80.0f, COLOR_UI_BLUE, "Lưu", renderer, font2, COLOR_WHITE);
    button_back = new Button(1416.0f, 1200.0f, 350.0f, 80.0f, COLOR_UI_RED, "Quay Lại", renderer, font2, COLOR_WHITE);

    if (!label_newpass || !label_eror1 || !label_eror2 || !label_success|| !textbox_passwordOld || !textbox_passwordNew1 || !textbox_passwordNew2 || !button_save || !button_back) {
        std::cerr << "Tạo UI thất bại!" << std::endl;
        return false;
    }
    return true;
}



Admin_NewPass::~Admin_NewPass() {
    delete label_newpass;
    delete label_eror1;
    delete label_eror2;
    delete label_success;
    delete textbox_passwordOld;
    delete textbox_passwordNew1;
    delete textbox_passwordNew2;
    delete button_save;
    delete button_back;
}

void Admin_NewPass::handleEvent(const SDL_Event& e) {
    if(textbox_passwordOld) {textbox_passwordOld->handleEvent(e);}
    if(textbox_passwordNew1) {textbox_passwordNew1->handleEvent(e);}
    if(textbox_passwordNew2) {textbox_passwordNew2->handleEvent(e);}
    if(button_save) {button_save->handleEvent(e);}
    if(button_back) {button_back->handleEvent(e);}

    if (button_back && button_back->isClicked()) {
        app->changeScreen("AdminDashBoard");
    }
    if(button_save->isClicked()) {
        std::string oldPassInput = textbox_passwordOld->getText();
        std::string newPass1 = textbox_passwordNew1->getText();
        std::string newPass2 = textbox_passwordNew2->getText();
        std::string currentUsername = app->getCurrentUsername();
        
        int result = resetPass(oldPassInput, newPass1, newPass2);
        switch (result)
        {
        case 0:
            error1 = false;
            error2 = false;
            success = true;
            break;
        case 1:
            error1 = true;
            error2 = false;
            success = false;
            break;
        case 2:
            error1 = false;
            error2 = true;
            success = false;
            break;
        default:
            break;
        }
    }
}

void Admin_NewPass::update() {
    if(textbox_passwordOld) {textbox_passwordOld->update();}
    if(textbox_passwordNew1) {textbox_passwordNew1->update();}
    if(textbox_passwordNew2) {textbox_passwordNew2->update();}
    if(button_save) {button_save->update();}
    if(button_back) {button_back->update();}
}

void Admin_NewPass::render(SDL_Renderer* renderer) {
    if(label_newpass) {label_newpass->render(renderer);}
    if(textbox_passwordOld) {textbox_passwordOld->render(renderer);}
    if(textbox_passwordNew1) {textbox_passwordNew1->render(renderer);}
    if(textbox_passwordNew2) {textbox_passwordNew2->render(renderer);}
    if(button_save) {button_save->render(renderer);}
    if(button_back) {button_back->render(renderer);}


    if (error1 && label_eror1) {
        label_eror1->render(renderer);
    }
    if (error2 && label_eror2) {
        label_eror2->render(renderer);
    }
    if (success && label_success) {
        label_success->render(renderer);
    }
}

void Admin_NewPass::onEnter() {
    textbox_passwordOld->setText("");
    textbox_passwordNew1->setText("");
    textbox_passwordNew2->setText("");
    error1 = false;
    error2 = false;
    success = false;
}

void Admin_NewPass::onExit() {
}