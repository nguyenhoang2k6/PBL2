#include <app/NVHD.h>
#include <app/App.h>        
#include <app/Button.h>
#include <app/Label.h>
#include <app/TextBox.h>
#include <app/Color.h>
#include <app/ProductListViewSmall.h>

NVHD::NVHD(App* app) :Screen(app) {
    label_title = nullptr;
    label_tongtien =nullptr;
    textbox_maHD = nullptr;
    textbox_ngayHD = nullptr;
    textbox_maNV = nullptr;
    button_submit = nullptr;
    button_back = nullptr;
    hoadon = nullptr;
    productList = nullptr;
}
bool NVHD::Init() {
    TTF_Font* font1 = app->getFont1();
    TTF_Font* font2 = app->getFont2();
    TTF_Font* font3 = app->getFont3();
    SDL_Renderer* renderer = app->getRenderer();

    if (!font1 || !font2 || !font3 || !renderer) {
        std::cerr << "Lỗi: Không lấy được tài nguyên từ App!" << std::endl;
        return false;
    }
    hoadon = new Hoadon();
    label_title = new Label("Thêm Hóa Đơn", COLOR_WARNING, 550.0f, 50.0f, font3, renderer);
    label_tongtien = new Label("Tổng Tiền: 0 VND", COLOR_GREEN, 600.0f, 150.0f, font2, renderer);
    textbox_maHD = new TextBox(100.0f, 250.0f, 350.0f, 75.0f, COLOR_WHITE, COLOR_BLACK,app, font1, "Mã Hóa Đơn", COLOR_GRAY_SILVER);
    textbox_ngayHD = new TextBox(500.0f, 250.0f, 350.0f, 75.0f, COLOR_WHITE, COLOR_BLACK,app, font1, "Thời gian", COLOR_GRAY_SILVER);
    textbox_maNV = new TextBox(900.0f, 250.0f, 350.0f, 75.0f, COLOR_WHITE, COLOR_BLACK,app, font1, "Mã Nhân viên", COLOR_GRAY_SILVER);
    button_submit = new Button(2532.0f, 0.0f, 200.0f, 100.0f, COLOR_UI_GREEN, "Lưu", renderer, font1, COLOR_WHITE);
    button_back = new Button(0.0f, 0.0f, 200.0f, 100.0f, COLOR_RED, "<-", renderer, font2, COLOR_WHITE);


    if (!label_title || !label_tongtien || !textbox_maHD || !textbox_ngayHD || !textbox_maNV || !button_submit || !button_back) {
        std::cerr << "Tạo UI thất bại!" << std::endl;
        return false;
    }
    // create compact product list so user can add items to the invoice
    productList = new ProductListViewSmall(app);
    productList->loadFromFile(renderer, "data/Item/Item.txt");
    return true;
}



NVHD::~NVHD() {
    delete label_title;
    delete label_tongtien;
    delete textbox_maHD;
    delete textbox_ngayHD;
    delete textbox_maNV;
    delete button_submit;
    delete button_back;
    if (productList) { delete productList; productList = nullptr; }
}

void NVHD::handleEvent(const SDL_Event& e) {
    if(textbox_maHD){textbox_maHD->handleEvent(e);}
    if(textbox_ngayHD){textbox_ngayHD->handleEvent(e);}
    if(textbox_maNV){textbox_maNV->handleEvent(e);}
    if(button_submit){button_submit->handleEvent(e);}
    if(button_back){button_back->handleEvent(e);}

    // forward scroll / click events to product list, and handle row clicks to add to hoadon
    if (productList) {
        if (e.type == SDL_EVENT_MOUSE_WHEEL) {
            productList->handleScroll((float)e.wheel.y);
        }
        if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN && e.button.button == SDL_BUTTON_LEFT) {
            float mx = (float)e.button.x;
            float my = (float)e.button.y;
            // If click was on delete button, remove item from data
            std::string delcode = productList->checkClick(mx, my);
            if (!delcode.empty()) {
                bool ok = productList->removeItem(delcode, app->getRenderer());
                if (!ok) std::cerr << "Failed to remove item: " << delcode << std::endl;
            } else {
                // else if click on row, add to hoadon
                std::string code = productList->checkItemClick(mx, my);
                if (!code.empty()) {
                    Item item = productList->getItemByCode(code);
                    if (hoadon && item.getmaItem().size() > 0) {
                        hoadon->themVatPham(item, 1);
                        long long total = hoadon->getTongTien();
                        if (label_tongtien) label_tongtien->setText(std::string("Tổng Tiền: ") + std::to_string(total) + " VND");
                    }
                }
            }
        }
    }

    if(button_back->isClicked()) {
        app->changeScreen("NVDashBoard");
    }
}

void NVHD::update() {
    if(textbox_maHD){textbox_maHD->update();}
    if(textbox_ngayHD){textbox_ngayHD->update();}
    if(textbox_maNV){textbox_maNV->update();}
    if(button_submit){button_submit->update();}
    if(button_back){button_back->update();}
}

void NVHD::renderHoaDon(SDL_Renderer* renderer) {
    if (!renderer || !hoadon) return;

    TTF_Font* font2 = app->getFont2();
    if (!font2) return;

    // Starting position for rendering invoice lines
    float startX = 40.0f;
    float startY = 220.0f;
    float lineHeight = 48.0f;

    int total = hoadon->getSoLuongMatHang();
    for (int i = 0; i < total; ++i) {
        Item item = hoadon->getItemAt(i);
        int quantity = hoadon->getSoLuongAt(i);

        // Left: item name
        std::string name = item.getTenItem();

        // Right: quantity + price (if available)
        std::string right = "x" + std::to_string(quantity);
        // append price if available
        int price = item.getprice();
        if (price > 0) {
            right += "   " + std::to_string(price) + " VND";
        }

        // Create temporary labels and render them
        Label lblName(name, COLOR_BLACK, startX, startY + i * lineHeight, font2, renderer);
        Label lblRight(right, COLOR_BLACK, startX + 520.0f, startY + i * lineHeight, font2, renderer);
        lblName.render(renderer);
        lblRight.render(renderer);
    }
}
void NVHD::render(SDL_Renderer* renderer) {
    if(label_title){label_title->render(renderer);}
    if(label_tongtien){label_tongtien->render(renderer);}
    if(textbox_maHD){textbox_maHD->render(renderer);}
    if(textbox_ngayHD){textbox_ngayHD->render(renderer);}
    if(textbox_maNV){textbox_maNV->render(renderer);}
    if(button_submit){button_submit->render(renderer);}
    if(button_back){button_back->render(renderer);}

    // render invoice items dynamically so the view updates when items change
    renderHoaDon(renderer);
    // render product list so user can click to add items
    if (productList) productList->render(renderer);
}

void NVHD::onEnter() {
    label_tongtien->setText("Tổng Tiền: 0 VND");
    textbox_maHD->setText("");
    textbox_ngayHD->setText("");
    textbox_maNV->setText("");
    if(hoadon) {
        delete hoadon;
    }
    hoadon = new Hoadon();
}

void NVHD::onExit() {

}