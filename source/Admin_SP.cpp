#include <app/Admin_SP.h>
#include <app/App.h>        
#include <app/Button.h>
#include <app/Label.h>
#include <app/TextBox.h>
#include <app/Color.h>
#include <SDL3_image/SDL_image.h>

Admin_SP::Admin_SP(App* app) :Screen(app) {
    productListView = nullptr;
    button_back = nullptr;
    button_add = nullptr;
}
bool Admin_SP::Init() {
    TTF_Font* font1 = app->getFont1();
    TTF_Font* font2 = app->getFont2();
    TTF_Font* font3 = app->getFont3();
    SDL_Renderer* renderer = app->getRenderer();

    if (!font1 || !font2 || !font3 || !renderer) {
        std::cerr << "Lỗi: Không lấy được tài nguyên từ App!" << std::endl;
        return false;
    }

    productListView = new ProductListView(app);
    // Load items from data folder
    productListView->loadFromFile(renderer, "data/Item/Item.txt");
    button_back = new Button(2532.0f, 0.0f, 200.0f, 150.0f,COLOR_UI_RED, "<-", renderer, font2, COLOR_WHITE);
    button_add = new Button(2332.0f, 0.0f, 200.0f, 150.0f,COLOR_UI_GREEN, "+", renderer, font2, COLOR_WHITE);
    


    if (!productListView || !button_back || !button_add) {
        std::cerr << "Tạo UI thất bại!" << std::endl;
        return false;
    }
    return true;
}



Admin_SP::~Admin_SP() {
    if (productListView) {
        delete productListView;
        productListView = nullptr;
    }
    delete button_back;
    delete button_add;
}

void Admin_SP::handleEvent(const SDL_Event& e) {
    // Forward events to the product list view (scroll, clicks)
    if (productListView) {
        if (e.type == SDL_EVENT_MOUSE_WHEEL) {
            productListView->handleScroll((float)e.wheel.y);
        }
        if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
            if (e.button.button == SDL_BUTTON_LEFT) {
                float mx = (float)e.button.x;
                float my = (float)e.button.y;
                std::string code = productListView->checkClick(mx, my);
                if (!code.empty()) {
                    std::cerr << "Clicked delete for item: " << code << std::endl;
                    // Attempt to remove from data file and reload
                    SDL_Renderer* rdr = app->getRenderer();
                    bool ok = productListView->removeItem(code, rdr);
                    if (!ok) {
                        std::cerr << "Failed to remove item: " << code << std::endl;
                    }
                }
            }
        }
    }
    if (button_back) {
        button_back->handleEvent(e);
    }
    if (button_add) {
        button_add->handleEvent(e);
    }
    if (button_back->isClicked()) {
        app->changeScreen("AdminDashBoard");
    }

}

void Admin_SP::update() {
    if(button_back) {
        button_back->update();
    }
    if(button_add) {
        button_add->update();
    }
}

void Admin_SP::render(SDL_Renderer* renderer) {
    if(productListView) {
        productListView->render(renderer);
    }
    if (button_back) {
        button_back->render(renderer);
    }
    if (button_add) {
        button_add->render(renderer);
    }
}

void Admin_SP::onEnter() {

}

void Admin_SP::onExit() {

}