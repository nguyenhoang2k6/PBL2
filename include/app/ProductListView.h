#pragma once
#include <vector>
#include <SDL3/SDL.h>
#include <app/Itemrow.h>
#include <app/App.h>

class ProductListView {
private:
    App* app;
    std::string dataFilePath;
    std::vector<Itemrow*> rows;
    float scrollOffsetY = 0.0f;
    float rowHeight = 180.0f;
    float startX = 0.0f;
    float startY = 150.0f;
    float viewHeight = 1450.0f;
    float headerHeight = 150.0f;
    Label* hdrCode = nullptr;
    Label* hdrName = nullptr;
    Label* hdrPrice = nullptr;
    bool showDelete = true;

public:
    ProductListView() : app(nullptr), dataFilePath("") {}
    ProductListView(App* app) : app(app), dataFilePath("") {}

    ~ProductListView() {
        for (auto row : rows) delete row;
        rows.clear();
        if (hdrCode) delete hdrCode;
        if (hdrName) delete hdrName;
        if (hdrPrice) delete hdrPrice;
    }

    void syncWithData(SDL_Renderer* renderer, const std::vector<Item>& dataItems) {
        for (auto row : rows) delete row;
        rows.clear();
        
        scrollOffsetY = 0.0f;
        
        if (app && app->getWindow()) {
            int winW = 0, winH = 0;
            SDL_GetWindowSize(app->getWindow(), &winW, &winH);
            viewHeight = static_cast<float>(winH) - headerHeight;
        } 

        float rowW = 700.0f;
        if (app && app->getWindow()) {
            int winW = 0, winH = 0;
            SDL_GetWindowSize(app->getWindow(), &winW, &winH);
            rowW = static_cast<float>(winW);
        }

        for (int i = 0; i < (int)dataItems.size(); ++i) {
            // Tính toán vị trí Y ban đầu (chưa tính scroll)
            float currentY = startY + (i * rowHeight);

            Itemrow* newRow = new Itemrow(renderer, dataItems[i], startX, currentY, rowW, rowHeight, app, showDelete);
            rows.push_back(newRow);
        }
    }

    void handleScroll(float wheelY) {
        float scrollSpeed = 30.0f;
        scrollOffsetY += wheelY * scrollSpeed;
        
        if (scrollOffsetY > 0) {
            scrollOffsetY = 0;
        }
        
        // Tính tổng chiều cao nội dung
        float totalContentHeight = rows.size() * rowHeight;
        
        if (totalContentHeight > viewHeight) {
            float minScroll = -(totalContentHeight - viewHeight);
            if (scrollOffsetY < minScroll) {
                scrollOffsetY = minScroll;
            }
        } else {
            scrollOffsetY = 0;
        }
    }

    std::string checkClick(float mouseX, float mouseY) {
        if (!showDelete) return "";
        for (auto row : rows) {
            if (row->checkDeleteClick(mouseX, mouseY)) {
                return row->getCode();
            }
        }
        return "";
    }

    std::string checkItemClick(float mouseX, float mouseY) {
        for (auto row : rows) {
            if (row->contains(mouseX, mouseY)) {
                return row->getCode();
            }
        }
        return "";
    }

    Item getItemByCode(const std::string& code) {
        for (auto row : rows) {
            if (row->getCode() == code) return row->getItem();
        }
        return Item();
    }

    void render(SDL_Renderer* renderer) {
        if (hdrCode && hdrName && hdrPrice) {
            float rowW = 700.0f;
            if (app && app->getWindow()) {
                int winW = 0, winH = 0;
                SDL_GetWindowSize(app->getWindow(), &winW, &winH);
                rowW = static_cast<float>(winW);
            }
            float fixedHeaderY = 0.0f;
            SDL_FRect hbg = { startX, fixedHeaderY, rowW, headerHeight };
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderFillRect(renderer, &hbg);
            
            hdrCode->setPosition(startX + 10.0f, fixedHeaderY + 50.0f);
            hdrName->setPosition(startX + 200.0f, fixedHeaderY + 50.0f);
            hdrPrice->setPosition(startX + 600.0f, fixedHeaderY + 50.0f);
            
            hdrCode->render(renderer);
            hdrName->render(renderer);
            hdrPrice->render(renderer);
            
            SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
            SDL_RenderLine(renderer, (int)startX, (int)(fixedHeaderY + headerHeight), (int)(startX + rowW), (int)(fixedHeaderY + headerHeight));
        }
        float rowW = 700.0f;
        if (app && app->getWindow()) {
            int winW = 0, winH = 0;
            SDL_GetWindowSize(app->getWindow(), &winW, &winH);
            rowW = static_cast<float>(winW);
        }
        
        SDL_Rect clipRect = { (int)startX, (int)startY, (int)rowW, (int)viewHeight };
        SDL_SetRenderClipRect(renderer, &clipRect);

        for (int i = 0; i < (int)rows.size(); ++i) {
            // Tính vị trí vẽ thực tế: Vị trí gốc + Offset cuộn
            float drawY = (startY + (i * rowHeight)) + scrollOffsetY;
            
            rows[i]->setY(drawY); 
            
            if (drawY + rowHeight > startY && drawY < startY + viewHeight) {
                rows[i]->render(renderer);
            }
        }

        SDL_SetRenderClipRect(renderer, nullptr);
    }

    void update() {
        for (auto row : rows) {
            if (row) row->update();
        }
    }

    void handleEvent(const SDL_Event& e) {
        if (e.type == SDL_EVENT_MOUSE_MOTION || e.type == SDL_EVENT_MOUSE_BUTTON_DOWN || e.type == SDL_EVENT_MOUSE_BUTTON_UP) {
            for (auto row : rows) {
                if (row) {
                    row->update();
                    row->handleEvent(e);
                }
            }
        }
    }

    // Load items from a simple text file: each item is 3 lines: code, name, price
    void loadFromFile(SDL_Renderer* renderer, const std::string& filepath);

    // without the deleted item, and reloads the view. Returns true on success.
    bool removeItem(const std::string& code, SDL_Renderer* renderer);

    void setShowDelete(bool enabled) { showDelete = enabled; }
};
