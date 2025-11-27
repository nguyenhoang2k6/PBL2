#include <app/NVSP.h>
#include <app/App.h>        
#include <app/Button.h>
#include <app/Label.h>
#include <app/TextBox.h>
#include <app/Color.h>
#include <SDL3_image/SDL_image.h>

NVSP::NVSP(App* app) :Screen(app) {
    productListView = nullptr;
}
bool NVSP::Init() {
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
    


    if (!productListView) {
        std::cerr << "Tạo UI thất bại!" << std::endl;
        return false;
    }
    return true;
}



NVSP::~NVSP() {
    if (productListView) {
        delete productListView;
        productListView = nullptr;
    }
}

void NVSP::handleEvent(const SDL_Event& e) {
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

}

void NVSP::update() {
}

void NVSP::render(SDL_Renderer* renderer) {
    if(productListView) {
        productListView->render(renderer);
    }
}

void NVSP::onEnter() {

}

void NVSP::onExit() {

}