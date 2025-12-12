#include <app/NVSP.h>
#include <app/App.h>        
#include <app/Button.h>
#include <app/Label.h>
#include <app/TextBox.h>
#include <app/Color.h>
#include <SDL3_image/SDL_image.h>

NVSP::NVSP(App* app) :Screen(app) {
    productListView = nullptr;
    button_back = nullptr;
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
    // NV screen: không cho hiện nút xóa
    productListView->setShowDelete(false);
    // Load items from data folder
    productListView->loadFromFile(renderer, "data/Item/Item.txt");
    button_back = new Button(2532.0f, 0.0f, 200.0f, 150.0f,COLOR_UI_RED, "<-", renderer, font2, COLOR_WHITE);
    


    if (!productListView || !button_back) {
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
    delete button_back;
}

void NVSP::handleEvent(const SDL_Event& e) {
    // Forward events to the product list view (scroll, clicks)
    if (productListView) {
        // give rows a chance to update hover state and process events
        productListView->handleEvent(e);
        if (e.type == SDL_EVENT_MOUSE_WHEEL) {
            productListView->handleScroll((float)e.wheel.y);
        }
        // NV không có quyền xóa sản phẩm: bỏ xử lý click nút xóa
    }
    if (button_back) {
        button_back->handleEvent(e);
    }
    if (button_back->isClicked()) {
        app->changeScreen("NVDashBoard");
    }

}

void NVSP::update() {
    if(button_back) {
        button_back->update();
    }
    if (productListView) productListView->update();
}

void NVSP::render(SDL_Renderer* renderer) {
    if(productListView) {
        productListView->render(renderer);
    }
    if (button_back) {
        button_back->render(renderer);
    }
}

void NVSP::onEnter() {
    // Reload product data from file to reflect any deletions/additions from admin screen
    if (productListView && app && app->getRenderer()) {
        productListView->loadFromFile(app->getRenderer(), "data/Item/Item.txt");
    }
}

void NVSP::onExit() {

}