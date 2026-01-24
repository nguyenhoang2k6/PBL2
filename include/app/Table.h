#pragma once
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <vector>
#include <string>
#include <iostream>

class Table {
private:
    std::vector<std::vector<std::string>> m_data;
    
    float x, y, width, height;
    float rowHeight, headerHeight;
    float m_scrollOffset;
    float m_maxScroll;
    
    TTF_Font* m_font;
    SDL_Renderer* m_renderer;

    std::vector<float> m_colWidths;
    std::vector<std::string> m_headers;

    SDL_Color m_headerColor;
    SDL_Color m_rowColor1;
    SDL_Color m_rowColor2;
    SDL_Color m_textColor;
    SDL_Color m_borderColor;

    // Helper: Vẽ một ô đơn lẻ
    void renderCell(float x, float y, float w, float h, const std::string& text, bool isHeader);

public:
    Table(float x, float y, float w, float h, 
          TTF_Font* font, SDL_Renderer* renderer,
          const std::vector<std::string>& headers, 
          const std::vector<float>& colWidths);

    // Xóa sạch dữ liệu cũ
    void clear();

    // Thêm một hàng dữ liệu mới (danh sách các chuỗi)
    void addRow(const std::vector<std::string>& rowData);

    void setColumns(const std::vector<std::string>& headers,
                    const std::vector<float>& colWidths);

    // --- HÀM HỆ THỐNG ---
    void handleEvent(const SDL_Event& e);
    void render();
    
    void setHeaderColor(SDL_Color c) { m_headerColor = c; }
    void setRowColor1(SDL_Color c) { m_rowColor1 = c; }
    void setRowColor2(SDL_Color c) { m_rowColor2 = c; }
};
