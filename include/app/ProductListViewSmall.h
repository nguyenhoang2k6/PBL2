#pragma once
#include <vector>
#include <SDL3/SDL.h>
#include <app/Itemrow.h>
#include <app/App.h>

// A compact product list view for NVHD screen (narrower sidebar)
class ProductListViewSmall {
private:
    App* app;
    std::string dataFilePath;
    std::vector<Itemrow*> rows;
    float scrollOffsetY = 0.0f;
    float rowHeight = 140.0f;
    float startX = 0.0f;
    float startY = 100.0f;
    float viewHeight = 1000.0f;
    float headerHeight = 0.0f;
    float sidebarWidth = 1000.0f;

    Label* hdrCode = nullptr;
    Label* hdrName = nullptr;
    Label* hdrPrice = nullptr;

public:
    ProductListViewSmall() : app(nullptr), dataFilePath("") {}
    ProductListViewSmall(App* app) : app(app), dataFilePath("") {}
    ~ProductListViewSmall() {
        for (auto r : rows) delete r;
        rows.clear();
        if (hdrCode) delete hdrCode;
        if (hdrName) delete hdrName;
        if (hdrPrice) delete hdrPrice;
    }

    void syncWithData(SDL_Renderer* renderer, const std::vector<Item>& dataItems) {
        for (auto r : rows) delete r;
        rows.clear();
        scrollOffsetY = 0.0f;

        float rowW = sidebarWidth;
        if (app && app->getWindow()) {
            int winW = 0, winH = 0;
            SDL_GetWindowSize(app->getWindow(), &winW, &winH);
            rowW = sidebarWidth;
            startX = static_cast<float>(winW) - sidebarWidth;
            viewHeight = static_cast<float>(winH) - startY - 40.0f;
        }

        for (int i = 0; i < (int)dataItems.size(); ++i) {
            float currentY = startY + (i * rowHeight);
            Itemrow* newRow = new Itemrow(renderer, dataItems[i], startX, currentY, rowW, rowHeight, app, true);
            rows.push_back(newRow);
        }
    }

    void handleScroll(float wheelY) {
        float scrollSpeed = 30.0f;
        scrollOffsetY += wheelY * scrollSpeed;
        if (scrollOffsetY > 0) scrollOffsetY = 0;
        float totalContentHeight = rows.size() * rowHeight;
        if (totalContentHeight > viewHeight) {
            float minScroll = -(totalContentHeight - viewHeight);
            if (scrollOffsetY < minScroll) scrollOffsetY = minScroll;
        } else scrollOffsetY = 0;
    }

    std::string checkClick(float mouseX, float mouseY) {
        for (auto row : rows) {
            if (row->checkDeleteClick(mouseX, mouseY)) return row->getCode();
        }
        return "";
    }

    std::string checkItemClick(float mouseX, float mouseY) {
        for (auto row : rows) {
            if (row->contains(mouseX, mouseY)) return row->getCode();
        }
        return "";
    }

    Item getItemByCode(const std::string& code) {
        for (auto row : rows) if (row->getCode() == code) return row->getItem();
        return Item();
    }

    void render(SDL_Renderer* renderer) {
        if (!renderer) return;
        float rowW = sidebarWidth;
        if (app && app->getWindow()) {
            int winW = 0, winH = 0; SDL_GetWindowSize(app->getWindow(), &winW, &winH);
            startX = static_cast<float>(winW) - sidebarWidth;
            viewHeight = static_cast<float>(winH) - startY;
        }

        SDL_FRect bgRect = { startX, startY, rowW, viewHeight };
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &bgRect);

        SDL_Rect clipRect = { (int)startX, (int)startY, (int)rowW, (int)viewHeight };
        SDL_SetRenderClipRect(renderer, &clipRect);
        for (int i = 0; i < (int)rows.size(); ++i) {
            float drawY = (startY + (i * rowHeight)) + scrollOffsetY;
            rows[i]->setY(drawY);
            if (drawY + rowHeight > startY && drawY < startY + viewHeight) rows[i]->render(renderer);
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

    // file operations
    void loadFromFile(SDL_Renderer* renderer, const std::string& filepath);
    bool removeItem(const std::string& code, SDL_Renderer* renderer);
};
