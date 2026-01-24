#pragma once
#include <app/Image.h>
#include <app/Textbox.h>
#include <app/Label.h>
#include <app/Button.h>
#include <app/Item.h>
#include <app/Color.h>
#include <app/App.h>

class Itemrow {
private:
    App* app;
    Item data;
    Image* productImage;
    Label* lblCode;
    Label* lblName;
    Label* lblPrice;
    Button* btnDelete;
    
    float x, y, width, height;

public:
    Itemrow(SDL_Renderer* renderer, const Item& item, float startX, float startY, float rowW, float rowH, App* app, bool showDelete) {
        this->data = item;
        this->x = startX;
        this->y = startY;
        this->width = rowW;
        this->height = rowH;
        this->app = app;

        float imgPadding = 24.0f;
        float imgSize = rowH - imgPadding * 2;

        // Logic ghép đường dẫn: đặt ảnh trong data/Image/<code>.png
        std::string imagePath = "data/Image/" + item.getmaItem() + ".png";

        productImage = new Image(renderer, imagePath, x + imgPadding, y + imgPadding, imgSize, imgSize);

        float textOffsetX = imgSize + imgPadding * 2 + 30.0f; 

        float nameOffsetX = x + textOffsetX;
        float priceOffsetX = x + textOffsetX + 400.0f;
        // Use a smaller vertical offset for smaller font
        float labelY = y + (rowH / 2) - 10.0f;

        lblCode = nullptr;
        lblName = new Label(item.getTenItem(), COLOR_BLACK, nameOffsetX, labelY, app->getFont1(), renderer);
        lblPrice = new Label(std::to_string(item.getprice()) + " VND", COLOR_BLACK, priceOffsetX, labelY, app->getFont1(), renderer);
        
        float btnW = 100.0f;
        float btnH = rowH - (imgPadding * 2);
        btnDelete = showDelete ? new Button(x + rowW - btnW - 20.0f, y + imgPadding, btnW, btnH, COLOR_RED, "X", renderer, app->getFont2(), COLOR_WHITE) : nullptr;
    }

    ~Itemrow() {
        delete productImage;
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

        productImage->setPosition(x + imgPadding, newY + imgPadding);
        float labelY = newY + (height / 2) - 10.0f;
        if (lblName) lblName->setPosition(x + textOffsetX, labelY);
        if (lblPrice) lblPrice->setPosition(x + textOffsetX + 400.0f, labelY);
        if (btnDelete) btnDelete->setPosition(x + width - 100.0f - 20.0f, newY + imgPadding);
    }

    void render(SDL_Renderer* renderer) {
        
        SDL_FRect bgRect = {x, y, width, height};
        SDL_Color bg = COLOR_WHITE;
        SDL_SetRenderDrawColor(renderer, bg.r, bg.g, bg.b, bg.a);
        SDL_RenderFillRect(renderer, &bgRect);
        SDL_Color border = COLOR_GRAY_LIGHT; 
        SDL_SetRenderDrawColor(renderer, border.r, border.g, border.b, border.a);
        SDL_RenderRect(renderer, &bgRect);

        productImage->render(renderer);
        if (lblName) lblName->render(renderer);
        if (lblPrice) lblPrice->render(renderer);
        if (btnDelete) btnDelete->render(renderer);
    }

    void update() {
        if (btnDelete) btnDelete->update();
    }

    void handleEvent(const SDL_Event& e) {
        if (btnDelete) btnDelete->handleEvent(e);
    }
    bool checkDeleteClick(float mouseX, float mouseY) {
        return btnDelete ? btnDelete->isClicked() : false;
    }
    
    std::string getCode() { return data.getmaItem(); }
    
    Item getItem() const { return data; }

    bool contains(float mx, float my) const {
        return (mx >= x && mx <= x + width && my >= y && my <= y + height);
    }
};
