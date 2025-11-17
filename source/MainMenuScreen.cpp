#include <app/MainMenuScreen.h>
#include <app/App.h>        
#include <app/Button.h>
#include <app/Label.h>
#include <app/TextBox.h>
#include <app/Color.h>

MainMenuScreen::MainMenuScreen(App* app) :Screen(app) {
    test_button = nullptr;
    test_label = nullptr;
    test_textbox = nullptr;

}bool MainMenuScreen::Init() {
    TTF_Font* font1 = app->getFont1();
    TTF_Font* font2 = app->getFont2();
    TTF_Font* font3 = app->getFont3();
    SDL_Renderer* renderer = app->getRenderer();

    if (!font1 || !font2 || !font3 || !renderer) {
        std::cerr << "Lỗi: Không lấy được tài nguyên từ App!" << std::endl;
        return false;
    }

    test_button = new Button(100,200,150,50,COLOR_BLUE,"button test",renderer,font1,COLOR_RED);
    test_label = new Label("label test",COLOR_YELLOW,300,200,font2,renderer);
    test_textbox = new TextBox(400,200,150,50,COLOR_GRAY_LIGHT,COLOR_MAGENTA,app,font3,"placehoder test",COLOR_GRAY_MEDIUM);


    if (!test_button || !test_label|| !test_textbox) {
        std::cerr << "Tạo UI thất bại!" << std::endl;
        return false;
    }
    return true;
}



MainMenuScreen::~MainMenuScreen() {
    delete test_button;
    delete test_label;
    delete test_textbox;
}

void MainMenuScreen::handleEvent(const SDL_Event& e) {
    if(test_button){test_button->handleEvent(e);}
    if(test_textbox){test_textbox->handleEvent(e);}
}

void MainMenuScreen::update() {
    if(test_button) {
        test_button->update();
    }
    if(test_textbox) {
        test_textbox->update();
    }
    // thu nghiem
    if (test_button && test_button->isClicked()) 
    {
        std::cout << "Nút Test đã được nhấn!" << std::endl;
        
        std::string username = test_textbox->getText();
        std::cout << "Dữ liệu nhập vào là: " << username << std::endl;

    }
}

void MainMenuScreen::render(SDL_Renderer* renderer) {
    if (test_button) {
        test_button->render(renderer);
    }

    if (test_label) {
        test_label->render(renderer);
    }

    if (test_textbox) {
        test_textbox->render(renderer);
    }
}

void MainMenuScreen::onEnter() {

}

void MainMenuScreen::onExit() {

}