#include <app/Admin_SP.h>
#include <app/App.h>        
#include <app/Button.h>
#include <app/Label.h>
#include <app/TextBox.h>
#include <app/Color.h>
#include <SDL3_image/SDL_image.h>

// Khởi tạo màn hình quản trị sản phẩm
Admin_SP::Admin_SP(App* app) :Screen(app) {
    productListView = nullptr;
    button_back = nullptr;
    button_add = nullptr;
}
// Khởi tạo UI danh sách sản phẩm và nút điều hướng
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
    productListView->loadFromFile(renderer, "data/Item/Item.txt");
    button_back = new Button(2532.0f, 0.0f, 200.0f, 150.0f,COLOR_UI_RED, "<-", renderer, font2, COLOR_WHITE);
    button_add = new Button(2332.0f, 0.0f, 200.0f, 150.0f,COLOR_UI_GREEN, "+", renderer, font2, COLOR_WHITE);
    
    if (!productListView || !button_back || !button_add) {
        std::cerr << "Tạo UI thất bại!" << std::endl;
        return false;
    }
    return true;
}

// Giải phóng tài nguyên màn hình sản phẩm
Admin_SP::~Admin_SP() {
    if (productListView) {
        delete productListView;
        productListView = nullptr;
    }
    delete button_back;
    delete button_add;
}

// Xử lý sự kiện click, cuộn và điều hướng
void Admin_SP::handleEvent(const SDL_Event& e) {
    if (productListView) {
        productListView->handleEvent(e);
        if (e.type == SDL_EVENT_MOUSE_WHEEL) {
            productListView->handleScroll((float)e.wheel.y);
        }
        if (e.type == SDL_EVENT_MOUSE_BUTTON_UP) {
            if (e.button.button == SDL_BUTTON_LEFT) {
                float mx = (float)e.button.x;
                float my = (float)e.button.y;
                std::string code = productListView->checkClick(mx, my);
                if (!code.empty()) {
                    std::cerr << "Clicked delete for item: " << code << std::endl;
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
    if (button_add->isClicked()) {
        app->changeScreen("Admin_AddProduct");
    }

}

// Cập nhật trạng thái nút mỗi frame
void Admin_SP::update() {
    if(button_back) {
        button_back->update();
    }
    if(button_add) {
        button_add->update();
    }
}

// Vẽ danh sách sản phẩm và các nút
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

// Tải lại dữ liệu khi vào màn hình
void Admin_SP::onEnter() {
    if (productListView && app && app->getRenderer()) {
        productListView->loadFromFile(app->getRenderer(), "data/Item/Item.txt");
    }
}

// Xử lý khi rời màn hình sản phẩm
void Admin_SP::onExit() {

}
