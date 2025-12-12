#pragma once
#include <vector>
#include <SDL3/SDL.h>
#include <app/Itemrow.h> // Class chứa từng dòng sản phẩm (đã có ảnh)
#include <app/App.h>

class ProductListView {
private:
    App* app;
    std::string dataFilePath;
    std::vector<Itemrow*> rows; // Danh sách các dòng hiển thị
    float scrollOffsetY = 0.0f; // Độ lệch khi cuộn trang (thường là số âm)
    // Tăng mạnh chiều cao hàng để dễ thao tác (2.5x so với 200 -> 500)
    float rowHeight = 180.0f;   // Giảm xuống 180px để dễ thấy hiệu quả cuộn và partial visibility
    float startX = 0.0f;       // Lề trái (0 để hàng trải sát mép)
    float startY = 150.0f;       // Vị trí Y bắt đầu của danh sách
    float viewHeight = 1450.0f; // Tăng chiều cao vùng nhìn thấy
    float headerHeight = 150.0f; // Chiều cao vùng header label (150px)
    // Header labels
    Label* hdrCode = nullptr;
    Label* hdrName = nullptr;
    Label* hdrPrice = nullptr;
    bool showDelete = true; // hiển thị nút xóa trên mỗi dòng

public:
    ProductListView() : app(nullptr), dataFilePath("") {}
    // Construct with App* to access fonts / renderer resources
    ProductListView(App* app) : app(app), dataFilePath("") {}

    // Destructor: Dọn dẹp bộ nhớ các dòng đã tạo
    ~ProductListView() {
        for (auto row : rows) delete row;
        rows.clear();
        if (hdrCode) delete hdrCode;
        if (hdrName) delete hdrName;
        if (hdrPrice) delete hdrPrice;
    }

    /**
     * @brief Đồng bộ dữ liệu logic (vector<Item>) sang giao diện (vector<Itemrow>)
     * @param renderer Cần thiết để load texture ảnh trong Itemrow
     * @param dataItems Danh sách dữ liệu gốc
     */
    void syncWithData(SDL_Renderer* renderer, const std::vector<Item>& dataItems) {
        // 1. Xóa giao diện cũ
        for (auto row : rows) delete row;
        rows.clear();
        
        // Reset lại vị trí cuộn về đầu trang khi load dữ liệu mới
        scrollOffsetY = 0.0f;
        
        // Update viewHeight dynamically: window height minus header
        if (app && app->getWindow()) {
            int winW = 0, winH = 0;
            SDL_GetWindowSize(app->getWindow(), &winW, &winH);
            viewHeight = static_cast<float>(winH) - headerHeight;
        } 

        // 2. Tạo giao diện mới
        // Determine row width dynamically from window size if App* is available
        float rowW = 700.0f; // fallback
        if (app && app->getWindow()) {
            int winW = 0, winH = 0;
            SDL_GetWindowSize(app->getWindow(), &winW, &winH);
            // Use full window width (startX may be 0 to stretch to edges)
            rowW = static_cast<float>(winW);
            // if you want a small right margin, subtract a constant here (e.g., -10)
        }

        for (int i = 0; i < (int)dataItems.size(); ++i) {
            // Tính toán vị trí Y ban đầu (chưa tính scroll)
            float currentY = startY + (i * rowHeight);

            // Tạo dòng mới: Truyền renderer vào để load ảnh
            // Kích thước dòng: Rộng = rowW, Cao = rowHeight
            Itemrow* newRow = new Itemrow(renderer, dataItems[i], startX, currentY, rowW, rowHeight, app, showDelete);
            rows.push_back(newRow);
        }
    }

    /**
     * @brief Xử lý sự kiện lăn chuột
     * @param wheelY Giá trị lăn chuột (thường là 1 hoặc -1)
     */
    void handleScroll(float wheelY) {
        float scrollSpeed = 30.0f; // Tốc độ cuộn
        scrollOffsetY += wheelY * scrollSpeed;
        
        // --- Logic giới hạn cuộn (Clamping) ---
        
        // 1. Không cho cuộn quá lên trên (kéo xuống quá đà)
        if (scrollOffsetY > 0) {
            scrollOffsetY = 0;
        }
        
        // 2. Không cho cuộn quá xuống dưới (hết danh sách)
        // Tính tổng chiều cao nội dung
        float totalContentHeight = rows.size() * rowHeight;
        
        // Nếu nội dung dài hơn vùng hiển thị thì mới chặn dưới
        if (totalContentHeight > viewHeight) {
            float minScroll = -(totalContentHeight - viewHeight);
            if (scrollOffsetY < minScroll) {
                scrollOffsetY = minScroll;
            }
        } else {
            // Nếu nội dung ngắn hơn màn hình thì không cho cuộn xuống (giữ nguyên ở 0)
            scrollOffsetY = 0;
        }
    }

    /**
     * @brief Kiểm tra xem có click vào nút xóa nào không
     * @return Mã sản phẩm cần xóa (nếu có), hoặc chuỗi rỗng
     */
    std::string checkClick(float mouseX, float mouseY) {
        if (!showDelete) return "";
        for (auto row : rows) {
            // Itemrow sẽ tự kiểm tra xem chuột có nằm trong nút xóa của nó không
            // Lưu ý: Itemrow đã được cập nhật vị trí Y mới nhất trong hàm render()
            if (row->checkDeleteClick(mouseX, mouseY)) {
                return row->getCode(); // Trả về mã để xóa trong Database
            }
        }
        return ""; // Không click vào gì cả
    }

    /**
     * @brief Check if a click is on an item row (not on delete). Returns item code if clicked, else empty.
     */
    std::string checkItemClick(float mouseX, float mouseY) {
        for (auto row : rows) {
            if (row->contains(mouseX, mouseY)) {
                // If click falls inside the row, but not on delete button, return the code
                // Note: btnDelete click is handled by checkClick(), here we treat row click as 'select item'
                return row->getCode();
            }
        }
        return "";
    }

    // Return Item by code (search rows)
    Item getItemByCode(const std::string& code) {
        for (auto row : rows) {
            if (row->getCode() == code) return row->getItem();
        }
        return Item();
    }

    /**
     * @brief Vẽ toàn bộ danh sách
     */
    void render(SDL_Renderer* renderer) {
        // Render header (STICKY at fixed position, not affected by scroll)
        if (hdrCode && hdrName && hdrPrice) {
            // header background at FIXED top position - compute width from window
            float rowW = 700.0f;
            if (app && app->getWindow()) {
                int winW = 0, winH = 0;
                SDL_GetWindowSize(app->getWindow(), &winW, &winH);
                rowW = static_cast<float>(winW);
            }
            float fixedHeaderY = 0.0f; // fixed Y at top (not scrolled)
            SDL_FRect hbg = { startX, fixedHeaderY, rowW, headerHeight };
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderFillRect(renderer, &hbg);
            
            // Update header label Y to fixed position (centered vertically in header)
            hdrCode->setPosition(startX + 10.0f, fixedHeaderY + 50.0f);
            hdrName->setPosition(startX + 200.0f, fixedHeaderY + 50.0f);
            hdrPrice->setPosition(startX + 600.0f, fixedHeaderY + 50.0f);
            
            // draw labels
            hdrCode->render(renderer);
            hdrName->render(renderer);
            hdrPrice->render(renderer);
            
            // Draw separator line under header
            SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
            SDL_RenderLine(renderer, (int)startX, (int)(fixedHeaderY + headerHeight), (int)(startX + rowW), (int)(fixedHeaderY + headerHeight));
        }
        // Compute list view dimensions for clipping
        float rowW = 700.0f;
        if (app && app->getWindow()) {
            int winW = 0, winH = 0;
            SDL_GetWindowSize(app->getWindow(), &winW, &winH);
            rowW = static_cast<float>(winW);
        }
        
        // Set clip rect to limit rendering to list view area (partial visibility ok)
        SDL_Rect clipRect = { (int)startX, (int)startY, (int)rowW, (int)viewHeight };
        SDL_SetRenderClipRect(renderer, &clipRect);

        for (int i = 0; i < (int)rows.size(); ++i) {
            // Tính vị trí vẽ thực tế: Vị trí gốc + Offset cuộn
            float drawY = (startY + (i * rowHeight)) + scrollOffsetY;
            
            // Cập nhật vị trí Y mới cho dòng (để vẽ và check click đúng chỗ)
            rows[i]->setY(drawY); 
            
            // Culling: Vẽ hàng khi nó nằm gần vùng nhìn thấy (kể cả partial)
            // Chỉ bỏ qua khi hàng nằm hoàn toàn ngoài vùng view
            if (drawY + rowHeight > startY && drawY < startY + viewHeight) {
                rows[i]->render(renderer);
            }
        }

        // Hủy clip rect sau khi vẽ xong
        SDL_SetRenderClipRect(renderer, nullptr);
    }

    // Update interactive elements (hover states) for each row
    void update() {
        for (auto row : rows) {
            if (row) row->update();
        }
    }

    // Forward events to child rows so their buttons can process clicks
    void handleEvent(const SDL_Event& e) {
        if (e.type == SDL_EVENT_MOUSE_MOTION || e.type == SDL_EVENT_MOUSE_BUTTON_DOWN || e.type == SDL_EVENT_MOUSE_BUTTON_UP) {
            for (auto row : rows) {
                if (row) {
                    // ensure hovered state is up-to-date before handling event
                    row->update();
                    row->handleEvent(e);
                }
            }
        }
    }

    // Load items from a simple text file: each item is 3 lines: code, name, price
    void loadFromFile(SDL_Renderer* renderer, const std::string& filepath);

    // Remove an item by code: makes a backup of the original file, rewrites file
    // without the deleted item, and reloads the view. Returns true on success.
    bool removeItem(const std::string& code, SDL_Renderer* renderer);

    // Setter: cấu hình hiển thị nút xóa (Admin: true, NV: false)
    void setShowDelete(bool enabled) { showDelete = enabled; }
};