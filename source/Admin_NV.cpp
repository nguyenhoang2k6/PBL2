#include <app/Admin_NV.h>
#include <app/App.h>        
#include <app/Button.h>
#include <app/Label.h>
#include <app/TextBox.h>
#include <app/Color.h>

Admin_NV::Admin_NV(App* app) :Screen(app) {
    m_table = nullptr;
    button_add = nullptr;
    button_back = nullptr;

}
bool Admin_NV::Init() {
    TTF_Font* font1 = app->getFont1();
    TTF_Font* font2 = app->getFont2();
    TTF_Font* font3 = app->getFont3();
    SDL_Renderer* renderer = app->getRenderer();

    if (!font1 || !font2 || !font3 || !renderer) {
        std::cerr << "Lỗi: Không lấy được tài nguyên từ App!" << std::endl;
        return false;
    }

    m_table = new EmployeeTable(0, 0, 2732, 1536,app->getFont1(),app->getRenderer());
    button_add = new Button(2432, 1436, 300, 100, COLOR_UI_GREEN, "+", renderer, font2, COLOR_WHITE);
    button_back = new Button(2132, 1436, 300, 100, COLOR_UI_RED, "<-", renderer, font2, COLOR_WHITE);


    if (!m_table || !button_add || !button_back) {
        std::cerr << "Tạo UI thất bại!" << std::endl;
        return false;
    }
    return true;
}



Admin_NV::~Admin_NV() {
    delete m_table;
    delete button_add;
    delete button_back;
}

void Admin_NV::handleEvent(const SDL_Event& e) {
    if(m_table){m_table->handleEvent(e);}
    if(button_add){button_add->handleEvent(e);}
    if(button_back){button_back->handleEvent(e);}

    if(button_add && button_add->isClicked()) {
        app->changeScreen("Admin_AddEmploy");
    }
    if(button_back && button_back->isClicked()) {
        app->changeScreen("AdminDashBoard");
    }
}

void Admin_NV::update() {
    if(m_table) {m_table->update();}
    if(button_add) {button_add->update();}
    if(button_back) {button_back->update();}
}

void Admin_NV::render(SDL_Renderer* renderer) {
    if(m_table) {m_table->render();}
    if(button_add) {button_add->render(renderer);}
    if(button_back) {button_back->render(renderer);}
}


void Admin_NV::onEnter() {
    // Reload employee data to reflect any additions/deletions
    if (m_table) {
        m_table->reloadData();
    }
}

void Admin_NV::onExit() {

}