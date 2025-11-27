#include <app/AdSP.h>
#include <app/App.h>        
#include <app/Button.h>
#include <app/Label.h>
#include <app/TextBox.h>
#include <app/Color.h>
#include <app/AdImage.h>

AdSP::AdSP(App* app) :Screen(app) {
    button_test = nullptr;

}
bool AdSP::Init() {
    TTF_Font* font1 = app->getFont1();
    TTF_Font* font2 = app->getFont2();
    TTF_Font* font3 = app->getFont3();
    SDL_Renderer* renderer = app->getRenderer();

    if (!font1 || !font2 || !font3 || !renderer) {
        std::cerr << "Lỗi: Không lấy được tài nguyên từ App!" << std::endl;
        return false;
    }

    button_test = new Button(300, 600, 470, 220,COLOR_UI_BLUE,"Test",renderer,font2,COLOR_WHITE);

    if (!button_test) {
        std::cerr << "Tạo UI thất bại!" << std::endl;
        return false;
    }
    return true;
}



AdSP::~AdSP() {
    delete button_test;
}

void AdSP::handleEvent(const SDL_Event& e) {
    if(button_test) {button_test->handleEvent(e);}


}

void AdSP::update() {
    if(button_test){button_test->update();}
}

void AdSP::render(SDL_Renderer* renderer) {
    if (button_test) {
        button_test->render(renderer);
    }
}

void AdSP::onEnter() {

}

void AdSP::onExit() {

}