#pragma once
#include <vector>
#include <app/Itemrow.h> // Class Itemrow bạn đã tạo trước đó
#include <app/Hoadon.h>
#include <app/App.h>

class InvoiceListView {
private:
    std::vector<Itemrow*> rows; // Quản lý danh sách các dòng hiển thị
    float x, y, width;          // Vị trí vùng hiển thị hóa đơn
    App* app;

public:
    InvoiceListView(App* app, float x, float y, float w) 
        : app(app), x(x), y(y), width(w) {}

    ~InvoiceListView() {
        clearRows();
    }

    // Hàm này thay thế cho loadFromFile: Lấy dữ liệu từ RAM (Hoadon) chứ không phải File
    void syncWithHoadon(Hoadon* hoadon, SDL_Renderer* renderer) {
        clearRows(); // Xóa giao diện cũ

        if (!hoadon) return;

        int n = hoadon->getSoLuongMatHang();
        float rowHeight = 80.0f; 

        for (int i = 0; i < n; i++) {
            Item item = hoadon->getItemAt(i);
            
            // Tính toán vị trí Y cho dòng này
            float currentY = this->y + (i * rowHeight);

            // Tạo dòng mới (Tái sử dụng Itemrow). Hóa đơn cần nút xóa để gỡ món khỏi hóa đơn.
            Itemrow* newRow = new Itemrow(renderer, item, this->x, currentY, this->width, rowHeight, app, true);
            
            rows.push_back(newRow);
        }
    }

    void render(SDL_Renderer* renderer) {
        for (auto row : rows) {
            row->render(renderer);
        }
    }

    // Trả về Mã món cần xóa nếu người dùng click nút Xóa, ngược lại trả về ""
    std::string checkClickDelete(const SDL_Event& e) {
        if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN && e.button.button == SDL_BUTTON_LEFT) {
            float mx = (float)e.button.x;
            float my = (float)e.button.y;

            for (auto row : rows) {
                // Truyền event vào nếu cần thiết cho hiệu ứng button
                // row->handleEvent(e); 

                if (row->checkDeleteClick(mx, my)) {
                    return row->getCode(); // Trả về mã để bên ngoài xử lý xóa
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