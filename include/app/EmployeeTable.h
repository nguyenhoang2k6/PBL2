#pragma once
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <vector>
#include <string>
#include "app/Nhanvien.h" // Class mới của chúng ta

class EmployeeTable {
private:
    std::vector<NhanVien> m_data; // Dùng class NhanVien chuẩn
    
    // Cấu hình hiển thị
    float x, y, width, height;
    float rowHeight, headerHeight;
    float m_scrollOffset;
    float m_maxScroll;
    
    // Tài nguyên
    TTF_Font* m_font;
    SDL_Renderer* m_renderer;

    // Cột
    std::vector<float> m_colWidths;
    std::vector<std::string> m_headers;
    float m_deleteColWidth;

    // Màu sắc (Thêm vào để khớp với constructor trong .cpp)
    SDL_Color m_headerColor;
    SDL_Color m_rowColor1;
    SDL_Color m_rowColor2;
    SDL_Color m_textColor;
    SDL_Color m_borderColor;

    // Helper
    void renderCell(float x, float y, float w, float h, const std::string& text, bool isHeader);
    bool hitDeleteButton(float mx, float my, size_t rowIndex) const;
    size_t rowIndexAt(float my) const;

public:
    EmployeeTable(float x, float y, float w, float h, TTF_Font* font, SDL_Renderer* renderer);

    // --- HÀM MỚI QUAN TRỌNG ---
    void update(); // Xử lý logic mỗi frame (nếu cần)
    void reloadData(); // Tải lại dữ liệu từ file (Real-time update)
    
    // Thêm trực tiếp vào bảng (hiển thị ngay mà không cần đọc lại file)
    void addRow(const NhanVien& nv);

    void handleEvent(const SDL_Event& e);
    void render();
};