#pragma once
#include <vector>
#include <app/Itemrow.h>
#include <app/Hoadon.h>
#include <app/App.h>

class InvoiceListView {
private:
    std::vector<Itemrow*> rows;
    float x, y, width;
    App* app;

public:
    InvoiceListView(App* app, float x, float y, float w) 
        : app(app), x(x), y(y), width(w) {}

    ~InvoiceListView() {
        clearRows();
    }

    void syncWithHoadon(Hoadon* hoadon, SDL_Renderer* renderer) {
        clearRows();

        if (!hoadon) return;

        int n = hoadon->getSoLuongMatHang();
        float rowHeight = 80.0f; 

        for (int i = 0; i < n; i++) {
            Item item = hoadon->getItemAt(i);
            
            // Tính toán vị trí Y cho dòng này
            float currentY = this->y + (i * rowHeight);

            Itemrow* newRow = new Itemrow(renderer, item, this->x, currentY, this->width, rowHeight, app, true);
            
            rows.push_back(newRow);
        }
    }

    void render(SDL_Renderer* renderer) {
        for (auto row : rows) {
            row->render(renderer);
        }
    }

    std::string checkClickDelete(const SDL_Event& e) {
        if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN && e.button.button == SDL_BUTTON_LEFT) {
            float mx = (float)e.button.x;
            float my = (float)e.button.y;

            for (auto row : rows) {

                if (row->checkDeleteClick(mx, my)) {
                    return row->getCode();
                }
            }
        }
        return "";
    }

private:
    void clearRows() {
        for (auto row : rows) delete row;
        rows.clear();
    }
};
