#include <app/NVSP.h>
#include <app/App.h>        
#include <app/Button.h>
#include <app/Label.h>
#include <app/TextBox.h>
#include <app/Color.h>
#include <SDL3_image/SDL_image.h>

// Khởi tạo màn hình sản phẩm cho nhân viên
NVSP::NVSP(App* app) :Screen(app) {
    productListView = nullptr;
    button_back = nullptr;
}
// Khởi tạo danh sách sản phẩm và nút quay lại
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
    productListView->setShowDelete(false);
    productListView->loadFromFile(renderer, "data/Item/Item.txt");
    button_back = new Button(2532.0f, 0.0f, 200.0f, 150.0f,COLOR_UI_RED, "<-", renderer, font2, COLOR_WHITE);
    
    if (!productListView || !button_back) {
        std::cerr << "Tạo UI thất bại!" << std::endl;
        return false;
    }
    return true;
}

// Giải phóng tài nguyên màn hình NVSP
NVSP::~NVSP() {
    if (productListView) {
        delete productListView;
        productListView = nullptr;
    }
    delete button_back;
}

// Xử lý cuộn và điều hướng ở màn hình NVSP
void NVSP::handleEvent(const SDL_Event& e) {
    if (productListView) {
        productListView->handleEvent(e);
        if (e.type == SDL_EVENT_MOUSE_WHEEL) {
            productListView->handleScroll((float)e.wheel.y);
        }
    }
    if (button_back) {
        button_back->handleEvent(e);
    }
    if (button_back->isClicked()) {
        app->changeScreen("NVDashBoard");
    }

}

// Cập nhật trạng thái nút và danh sách
void NVSP::update() {
    if(button_back) {
        button_back->update();
    }
    if (productListView) productListView->update();
}

// Vẽ danh sách sản phẩm và nút quay lại
void NVSP::render(SDL_Renderer* renderer) {
    if(productListView) {
        productListView->render(renderer);
    }
    if (button_back) {
        button_back->render(renderer);
    }
}

// Tải lại dữ liệu khi vào màn hình
void NVSP::onEnter() {
    if (productListView && app && app->getRenderer()) {
        productListView->loadFromFile(app->getRenderer(), "data/Item/Item.txt");
    }
}

// Xử lý khi rời màn hình NVSP
void NVSP::onExit() {

}
