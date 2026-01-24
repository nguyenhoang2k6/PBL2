#include <app/Table.h>
#include <algorithm>

// Khởi tạo bảng hiển thị dữ liệu
Table::Table(float x, float y, float w, float h, 
             TTF_Font* font, SDL_Renderer* renderer,
             const std::vector<std::string>& headers, 
             const std::vector<float>& colWidths)
    : x(x), y(y), width(w), height(h), m_font(font), m_renderer(renderer),
      m_headers(headers), m_colWidths(colWidths),
      rowHeight(50.0f), headerHeight(60.0f), 
      m_scrollOffset(0), m_maxScroll(0)
{
    m_headerColor = {245, 245, 220, 255}; 
    m_rowColor1   = {255, 255, 255, 255}; 
    m_rowColor2   = {245, 245, 220, 255};   
    m_textColor   = {0, 0, 0, 255};       
    m_borderColor = {100, 80, 60, 255};   
}

// Xóa toàn bộ dữ liệu trong bảng
void Table::clear() {
    m_data.clear();
    m_scrollOffset = 0;
    m_maxScroll = 0;
}

// Thêm một dòng dữ liệu vào bảng
void Table::addRow(const std::vector<std::string>& rowData) {
    if (rowData.size() != m_headers.size()) {
        std::cerr << "Lỗi Table: Số lượng cột không khớp! (Can: " 
                  << m_headers.size() << ", Nhan: " << rowData.size() << ")" << std::endl;
        return;
    }

    m_data.push_back(rowData);
    
    // Tính lại thanh cuộn
    float totalH = m_data.size() * rowHeight;
    m_maxScroll = std::max(0.0f, totalH - (height - headerHeight));
    
    if (m_maxScroll > 0) m_scrollOffset = m_maxScroll;
}

// Thiết lập lại tiêu đề và độ rộng cột
void Table::setColumns(const std::vector<std::string>& headers,
                       const std::vector<float>& colWidths) {
    if (headers.size() != colWidths.size()) {
        std::cerr << "Loi Table: headers va colWidths khong khop" << std::endl;
        return;
    }
    m_headers = headers;
    m_colWidths = colWidths;
    clear();
}

// Xử lý sự kiện cuộn chuột trong vùng bảng
void Table::handleEvent(const SDL_Event& e) {
    if (e.type == SDL_EVENT_MOUSE_WHEEL) {
        float mx, my;
        SDL_GetMouseState(&mx, &my);
        SDL_FPoint mousePoint = {mx, my};
        SDL_FRect tableRect = {x, y, width, height};

        if (SDL_PointInRectFloat(&mousePoint, &tableRect)) {
            m_scrollOffset -= e.wheel.y * 20.0f;
            if(m_scrollOffset < 0) m_scrollOffset = 0;
            if(m_scrollOffset > m_maxScroll) m_scrollOffset = m_maxScroll;
        }
    }
}

// Vẽ một ô trong bảng (tiêu đề hoặc dữ liệu)
void Table::renderCell(float cellX, float cellY, float cellW, float cellH, const std::string& text, bool isHeader) {
    SDL_Color txtColor = isHeader ? SDL_Color{50, 30, 20, 255} : m_textColor;

    if (m_font && !text.empty()) {
        SDL_Rect clipRect = { (int)cellX, (int)cellY, (int)cellW, (int)cellH };
        SDL_SetRenderClipRect(m_renderer, &clipRect);

        SDL_Surface* surf = TTF_RenderText_Blended(m_font, text.c_str(), 0, txtColor);
        if (surf) {
            SDL_Texture* texture = SDL_CreateTextureFromSurface(m_renderer, surf);
            if (texture) {
                float textX = cellX + 10.0f;
                float textY = cellY + (cellH - surf->h) / 2.0f;
                SDL_FRect dst = { textX, textY, (float)surf->w, (float)surf->h };
                SDL_RenderTexture(m_renderer, texture, nullptr, &dst);
                SDL_DestroyTexture(texture);
            }
            SDL_DestroySurface(surf);
        }
        SDL_SetRenderClipRect(m_renderer, nullptr);
    }

    SDL_SetRenderDrawColor(m_renderer, m_borderColor.r, m_borderColor.g, m_borderColor.b, m_borderColor.a);
    SDL_FRect borderRect = { cellX, cellY, cellW, cellH };
    SDL_RenderRect(m_renderer, &borderRect);
}

// Vẽ bảng, tiêu đề và các hàng dữ liệu
void Table::render() {
    SDL_Rect contentClip = {(int)x, (int)(y + headerHeight), (int)width, (int)(height - headerHeight)};
    SDL_SetRenderClipRect(m_renderer, &contentClip);

    float currentY = y + headerHeight - m_scrollOffset;

    for (size_t i = 0; i < m_data.size(); ++i) {
        if (currentY + rowHeight < y + headerHeight) { currentY += rowHeight; continue; }
        if (currentY > y + height) break;

        float currentX = x;
        
        SDL_Color rowBg = (i % 2 == 0) ? m_rowColor1 : m_rowColor2;
        SDL_SetRenderDrawColor(m_renderer, rowBg.r, rowBg.g, rowBg.b, rowBg.a);
        SDL_FRect rowRect = {x, currentY, width, rowHeight};
        SDL_RenderFillRect(m_renderer, &rowRect);

        const std::vector<std::string>& row = m_data[i];
        for (size_t j = 0; j < row.size(); ++j) {
            if (j < m_colWidths.size()) {
                renderCell(currentX, currentY, m_colWidths[j], rowHeight, row[j], false);
                currentX += m_colWidths[j];
            }
        }

        currentY += rowHeight;
    }
    
    SDL_SetRenderClipRect(m_renderer, nullptr);

    float headerX = x;
    SDL_SetRenderDrawColor(m_renderer, m_headerColor.r, m_headerColor.g, m_headerColor.b, m_headerColor.a);
    SDL_FRect headerRect = {x, y, width, headerHeight};
    SDL_RenderFillRect(m_renderer, &headerRect);

    for (size_t i = 0; i < m_headers.size(); ++i) {
        if (i < m_colWidths.size()) {
            renderCell(headerX, y, m_colWidths[i], headerHeight, m_headers[i], true);
            headerX += m_colWidths[i];
        }
    }
    
    SDL_SetRenderDrawColor(m_renderer, m_borderColor.r, m_borderColor.g, m_borderColor.b, m_borderColor.a);
    SDL_FRect outline = {x, y, width, height};
    SDL_RenderRect(m_renderer, &outline);
}
