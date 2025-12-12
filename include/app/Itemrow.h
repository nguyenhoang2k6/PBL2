#pragma once
#include <app/Image.h>    // Class mới tạo
#include <app/Textbox.h>
#include <app/Label.h>
#include <app/Button.h>
#include <app/Item.h>
#include <app/Color.h>
#include <app/App.h>

class Itemrow {
private:
    App* app; // Thêm con trỏ App để truy cập font
    Item data;
    Image* productImage; // <--- Thành phần mới
    Label* lblCode;
    Label* lblName;
    Label* lblPrice;
    Button* btnDelete;
    
    float x, y, width, height;

public:
    // Cần truyền thêm SDL_Renderer vào Constructor để load ảnh
    Itemrow(SDL_Renderer* renderer, const Item& item, float startX, float startY, float rowW, float rowH, App* app, bool showDelete) {
        this->data = item;
        this->x = startX;
        this->y = startY;
        this->width = rowW;
        this->height = rowH;
        this->app = app;

        // --- 1. XỬ LÝ ẢNH ---
        // Giữ tỷ lệ ảnh theo chiều cao dòng, để có padding tốt hơn
        float imgPadding = 24.0f;
        float imgSize = rowH - imgPadding * 2; // để ảnh có khoảng trên/dưới

        // Logic ghép đường dẫn: đặt ảnh trong data/Image/<code>.png
        std::string imagePath = "data/Image/" + item.getmaItem() + ".png";

        // Tạo đối tượng Image (đặt cách lề một khoảng nhỏ)
        productImage = new Image(renderer, imagePath, x + imgPadding, y + imgPadding, imgSize, imgSize);

        // --- 2. XỬ LÝ LABEL (Chỉ tên sản phẩm và giá) ---
        // Offset (lề trái) mới = Kích thước ảnh + khoảng cách đệm
        float textOffsetX = imgSize + imgPadding * 2 + 30.0f; 

        // Bỏ mã sản phẩm, chỉ hiển thị tên và giá với font nhỏ hơn
        float nameOffsetX = x + textOffsetX;
        float priceOffsetX = x + textOffsetX + 400.0f;  // dãn rộng cột giá
        // Use a smaller vertical offset for smaller font
        float labelY = y + (rowH / 2) - 10.0f;

        // Use smallest font (font3) for more compact display
        lblCode = nullptr;  // Not used
        lblName = new Label(item.getTenItem(), COLOR_BLACK, nameOffsetX, labelY, app->getFont1(), renderer);
        lblPrice = new Label(std::to_string(item.getprice()) + " VND", COLOR_BLACK, priceOffsetX, labelY, app->getFont1(), renderer);
        
        // Nút xóa nằm cuối dòng, rộng hơn và lùi vào trong 10px
        float btnW = 100.0f;
        float btnH = rowH - (imgPadding * 2);
        btnDelete = showDelete ? new Button(x + rowW - btnW - 20.0f, y + imgPadding, btnW, btnH, COLOR_RED, "X", renderer, app->getFont2(), COLOR_WHITE) : nullptr;
    }

    ~Itemrow() {
        delete productImage; // Nhớ xóa ảnh
        delete lblCode;
        delete lblName;
        delete lblPrice;
        delete btnDelete;
    }

    void setY(float newY) {
        this->y = newY;
        float imgPadding = 24.0f;
        float imgSize = height - imgPadding * 2;
        float textOffsetX = imgSize + imgPadding * 2 + 30.0f;

        // Cập nhật vị trí Y cho ảnh và các label (canh giữa theo chiều dọc)
        productImage->setPosition(x + imgPadding, newY + imgPadding);
        float labelY = newY + (height / 2) - 10.0f;
        if (lblName) lblName->setPosition(x + textOffsetX, labelY);
        if (lblPrice) lblPrice->setPosition(x + textOffsetX + 400.0f, labelY);
        if (btnDelete) btnDelete->setPosition(x + width - 100.0f - 20.0f, newY + imgPadding);
    }

    void render(SDL_Renderer* renderer) {
        // Remove internal culling - let clip rect handle visibility
        // (clip rect will cut off any part outside the view area)
        
        // Vẽ nền dòng (dùng màu trắng)
        SDL_FRect bgRect = {x, y, width, height};
        SDL_Color bg = COLOR_WHITE;
        SDL_SetRenderDrawColor(renderer, bg.r, bg.g, bg.b, bg.a);
        SDL_RenderFillRect(renderer, &bgRect);
        // Vẽ viền
        SDL_Color border = COLOR_GRAY_LIGHT; 
        SDL_SetRenderDrawColor(renderer, border.r, border.g, border.b, border.a);
        SDL_RenderRect(renderer, &bgRect);

        // Vẽ các thành phần con (bỏ mã sản phẩm)
        productImage->render(renderer);
        if (lblName) lblName->render(renderer);
        if (lblPrice) lblPrice->render(renderer);
        if (btnDelete) btnDelete->render(renderer);
    }

    // Update interactive components (hover state)
    void update() {
        if (btnDelete) btnDelete->update();
    }

    // Forward events to interactive components (buttons)
    void handleEvent(const SDL_Event& e) {
        if (btnDelete) btnDelete->handleEvent(e);
    }
    bool checkDeleteClick(float mouseX, float mouseY) {
        return btnDelete ? btnDelete->isClicked() : false;
    }
    
    std::string getCode() { return data.getmaItem(); }
    
    // Return the stored Item object
    Item getItem() const { return data; }

    // Check if a point is inside this row's rectangle
    bool contains(float mx, float my) const {
        return (mx >= x && mx <= x + width && my >= y && my <= y + height);
    }
};