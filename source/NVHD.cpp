#include <app/NVHD.h>
#include <app/App.h>        
#include <app/Button.h>
#include <app/Label.h>
#include <app/TextBox.h>
#include <app/Color.h>
#include <app/ProductListViewSmall.h>
#include <algorithm>

// Helper: format money with '.' as thousands separator, returns like "22.000 VND"
static std::string formatMoney(long long v) {
    if (v == 0) return std::string("0 VND");
    bool neg = v < 0;
    if (neg) v = -v;
    std::string s = std::to_string(v);
    std::string out;
    int cnt = 0;
    for (int i = (int)s.size() - 1; i >= 0; --i) {
        out.push_back(s[i]);
        ++cnt;
        if (cnt == 3 && i != 0) {
            out.push_back('.');
            cnt = 0;
        }
    }
    if (neg) out.push_back('-');
    reverse(out.begin(), out.end());
    out += " VND";
    return out;
}

// Helper: validate date format (dd/mm/yyyy or d/m/yyyy)
static bool isValidDate(const std::string& dateStr) {
    if (dateStr.empty()) return false;
    int day = 0, month = 0, year = 0;
    int count = sscanf(dateStr.c_str(), "%d/%d/%d", &day, &month, &year);
    if (count != 3) return false;
    if (day < 1 || day > 31) return false;
    if (month < 1 || month > 12) return false;
    if (year < 2000 || year > 2099) return false;
    return true;
}

// Helper: check if employee ID exists in data/Nhanvien/Nhanvien.txt
static bool isValidEmployeeID(const std::string& empID) {
    if (empID.empty()) return false;
    std::ifstream file("data/Nhanvien/Nhanvien.txt");
    if (!file) return false;
    std::string line;
    while (std::getline(file, line)) {
        // trim whitespace
        line.erase(0, line.find_first_not_of(" \t\r\n"));
        line.erase(line.find_last_not_of(" \t\r\n") + 1);
        if (line == empID) {
            file.close();
            return true;
        }
    }
    file.close();
    return false;
}

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
    notificationLabel = nullptr;
    notificationTimer = 0.0f;
    notificationDuration = 0.0f;
    notificationPersist = false;
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
    // keep total label at original designer position (unchanged layout)
    label_tongtien = new Label(std::string("Tổng Tiền: ") + formatMoney(0), COLOR_GREEN, 100.0f, 150.0f, font2, renderer);
    textbox_maHD = new TextBox(100.0f, 330.0f, 350.0f, 75.0f, COLOR_WHITE, COLOR_BLACK,app, font1, "Mã Hóa Đơn", COLOR_GRAY_SILVER);
    textbox_ngayHD = new TextBox(500.0f, 330.0f, 350.0f, 75.0f, COLOR_WHITE, COLOR_BLACK,app, font1, "Thời gian", COLOR_GRAY_SILVER);
    textbox_maNV = new TextBox(900.0f, 330.0f, 350.0f, 75.0f, COLOR_WHITE, COLOR_BLACK,app, font1, "Mã Nhân viên", COLOR_GRAY_SILVER);
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
    if (notificationLabel) { delete notificationLabel; notificationLabel = nullptr; }
}

void NVHD::handleEvent(const SDL_Event& e) {
    if(textbox_maHD){textbox_maHD->handleEvent(e);}
    if(textbox_ngayHD){textbox_ngayHD->handleEvent(e);}
    if(textbox_maNV){textbox_maNV->handleEvent(e);}
    if(button_submit){button_submit->handleEvent(e);}
    if(button_back){button_back->handleEvent(e);}

    // forward scroll / click events to product list, and handle row clicks to add to hoadon
    if (productList) {
        // allow rows/buttons to process hover/click state first
        productList->handleEvent(e);
        if (e.type == SDL_EVENT_MOUSE_WHEEL) {
            productList->handleScroll((float)e.wheel.y);
        }
        if (e.type == SDL_EVENT_MOUSE_BUTTON_UP && e.button.button == SDL_BUTTON_LEFT) {
            float mx = (float)e.button.x;
            float my = (float)e.button.y;
            // If click was on delete button, decrement that item in current invoice (if present)
            std::string delcode = productList->checkClick(mx, my);
            if (!delcode.empty()) {
                if (hoadon) {
                    // Decrease quantity by 1; if quantity reaches 0 it will be removed
                    hoadon->giamVatPham(delcode, 1);
                    long long total = hoadon->getTongTien();
                            if (label_tongtien) label_tongtien->setText(std::string("Tổng Tiền: ") + formatMoney(total));
                }
            } else {
                // else if click on row, add to hoadon
                std::string code = productList->checkItemClick(mx, my);
                if (!code.empty()) {
                    Item item = productList->getItemByCode(code);
                    if (hoadon && item.getmaItem().size() > 0) {
                        hoadon->themVatPham(item, 1);
                        long long total = hoadon->getTongTien();
                        if (label_tongtien) label_tongtien->setText(std::string("Tổng Tiền: ") + formatMoney(total));
                    }
                }
            }
        }
    }

    if(button_back->isClicked()) {
        app->changeScreen("NVDashBoard");
    }
    
    // Handle save button click
    if (button_submit && button_submit->isClicked()) {
        std::string maHD = textbox_maHD ? textbox_maHD->getText() : "";
        std::string ngayHD = textbox_ngayHD ? textbox_ngayHD->getText() : "";
        std::string maNV = textbox_maNV ? textbox_maNV->getText() : "";
        
        // Clear previous notification at start of save attempt
        if (notificationLabel) {
            delete notificationLabel;
            notificationLabel = nullptr;
        }
        notificationTimer = 0.0f;
        notificationPersist = false;
        
        // Validation 1: Check if invoice ID is empty
        if (maHD.empty()) {
            notificationPersist = true; // persist until next save attempt
            SDL_Color errColor = {255, 0, 0, 255}; // red
            notificationLabel = new Label("Lỗi: Vui lòng nhập Mã Hóa Đơn", errColor, 100.0f, 240.0f, app->getFont1(), app->getRenderer());
            return;
        }
        
        // Validation 2: Check date format
        if (!isValidDate(ngayHD)) {
            notificationPersist = true;
            SDL_Color errColor = {255, 0, 0, 255};
            notificationLabel = new Label("Lỗi: Định dạng ngày không hợp lệ (dd/mm/yyyy)", errColor, 100.0f, 240.0f, app->getFont1(), app->getRenderer());
            return;
        }
        
        // Validation 3: Check employee ID validity
        if (!isValidEmployeeID(maNV)) {
            notificationPersist = true;
            SDL_Color errColor = {255, 0, 0, 255};
            notificationLabel = new Label("Lỗi: Mã Nhân viên không tồn tại", errColor, 100.0f, 240.0f, app->getFont1(), app->getRenderer());
            return;
        }
        
        // Validation 4: Check if invoice has items
        if (hoadon && hoadon->getSoLuongMatHang() == 0) {
            notificationPersist = true;
            SDL_Color errColor = {255, 0, 0, 255};
            notificationLabel = new Label("Lỗi: Hóa đơn không có sản phẩm", errColor, 100.0f, 240.0f, app->getFont1(), app->getRenderer());
            return;
        }
        
        // All validations passed - save invoice
        if (hoadon) {
            // Parse date from string
            int day = 0, month = 0, year = 0;
            sscanf(ngayHD.c_str(), "%d/%d/%d", &day, &month, &year);
            hoadon->setThongTinChung(maHD, maNV, day, month, year);
            
            // Save to file
            if (hoadon->luuHoadonVaoFile()) {
                // Show success message for 10 seconds, then reset invoice
                notificationPersist = false;
                notificationTimer = 10.0f;
                notificationDuration = 10.0f;
                SDL_Color successColor = {0, 200, 0, 255}; // green
                notificationLabel = new Label("Lưu hóa đơn thành công!", successColor, 100.0f, 240.0f, app->getFont1(), app->getRenderer());
                // Do NOT reset yet - will reset in update() when timer expires
            } else {
                notificationPersist = true;
                SDL_Color errColor = {255, 0, 0, 255};
                notificationLabel = new Label("Lỗi: Lưu hóa đơn thất bại (mã trùng?)", errColor, 100.0f, 240.0f, app->getFont1(), app->getRenderer());
            }
        }
    }
}

void NVHD::update() {
    if(textbox_maHD){textbox_maHD->update();}
    if(textbox_ngayHD){textbox_ngayHD->update();}
    if(textbox_maNV){textbox_maNV->update();}
    if(button_submit){button_submit->update();}
    if(button_back){button_back->update();}
    if (productList) productList->update();
    
    // Update notification timer (for success message only)
    if (notificationTimer > 0.0f && !notificationPersist) {
        notificationTimer -= 0.016f; // ~60fps
    }
    
    // When success timer expires, reset invoice
    if (notificationTimer <= 0.0f && notificationDuration > 0.0f) {
        textbox_maHD->setText("");
        textbox_ngayHD->setText("");
        textbox_maNV->setText("");
        if (hoadon) delete hoadon;
        hoadon = new Hoadon();
        label_tongtien->setText(std::string("Tổng Tiền: ") + formatMoney(0));
        notificationDuration = 0.0f;
        if (notificationLabel) {
            delete notificationLabel;
            notificationLabel = nullptr;
        }
    }
}

void NVHD::renderHoaDon(SDL_Renderer* renderer) {
    if (!renderer || !hoadon) return;

    TTF_Font* font2 = app->getFont2();
    if (!font2) return;

    // Starting position for rendering invoice lines (moved lower so it doesn't overlap top UI)
        // Compute a small right offset so invoice content sits more centered
        float startX = 40.0f;
        if (app && app->getWindow()) {
            int winW = 0, winH = 0;
            SDL_GetWindowSize(app->getWindow(), &winW, &winH);
            // ProductListViewSmall uses a sidebar ~1000px on the right; compute left area width
            float leftArea = (float)winW - 1000.0f;
            if (leftArea > 0) {
                // shift right by 15% of available left area so the list looks centered in the light area
                startX += leftArea * 0.15f;
            }
        }
        float startY = 440.0f; // moved down so invoice content doesn't overlap UI
        float lineHeight = 72.0f; // spacing between items

    int total = hoadon->getSoLuongMatHang();
    for (int i = 0; i < total; ++i) {
        Item item = hoadon->getItemAt(i);
        int quantity = hoadon->getSoLuongAt(i);

        // Left: item name
        std::string name = item.getTenItem();

        // Right: quantity + formatted price (if available)
        std::string right = "x" + std::to_string(quantity);
        int price = item.getprice();
        if (price > 0) {
            right += "   " + formatMoney(price);
        }

        // Create temporary labels and render them
        // Name column should start at absolute X = 100px per design request
        float nameX = 100.0f;
        Label lblName(name, COLOR_BLACK, nameX, startY + i * lineHeight, font2, renderer);
        // Keep price column calculation as before (relative to startX)
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
    
    // Render notification if active (either persistent error or timed success)
    if (notificationLabel && (notificationPersist || notificationTimer > 0.0f)) {
        notificationLabel->render(renderer);
    }
}

void NVHD::onEnter() {
    label_tongtien->setText(std::string("Tổng Tiền: ") + formatMoney(0));
    textbox_maHD->setText("");
    textbox_ngayHD->setText("");
    textbox_maNV->setText("");
    if(hoadon) {
        delete hoadon;
    }
    hoadon = new Hoadon();
    notificationTimer = 0.0f;
    notificationDuration = 0.0f;
    notificationPersist = false;
    if (notificationLabel) {
        delete notificationLabel;
        notificationLabel = nullptr;
    }
    // Reload product list to reflect any deletions/additions from admin screen
    if (productList && app && app->getRenderer()) {
        productList->loadFromFile(app->getRenderer(), "data/Item/Item.txt");
    }
}

void NVHD::onExit() {

}