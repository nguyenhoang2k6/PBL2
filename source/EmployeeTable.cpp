#include <app/EmployeeTable.h>
#include <app/Color.h>
#include <algorithm>

// Khởi tạo bảng nhân viên
EmployeeTable::EmployeeTable(float x, float y, float w, float h, TTF_Font* font, SDL_Renderer* renderer)
    : x(x), y(y), width(w), height(h), m_font(font), m_renderer(renderer),
      rowHeight(50.0f),
      headerHeight(60.0f),
      m_scrollOffset(0), m_maxScroll(0)
{
    m_headers = {"Ma NV", "Ten NV", "Chuc Vu", "Tuoi", "Dia Chi", "SDT", "Xoa"};
    
    m_colWidths = {
        200.0f,
        800.0f,
        400.0f,
        100.0f,
        800.0f,
        332.0f
    }; 
    m_deleteColWidth = 100.0f;
    m_colWidths.push_back(m_deleteColWidth);

    m_headerColor = COLOR_CREAM; 
    m_rowColor1   = COLOR_CREAM;   
    m_rowColor2   = COLOR_CREAM;

    m_textColor   = {0, 0, 0, 255};           

    m_borderColor = {100, 80, 60, 255};   

    reloadData();
}

// Tải lại dữ liệu nhân viên từ file
void EmployeeTable::reloadData() {
    m_data = NhanVien::getAll();
    
    // Tính toán lại vùng cuộn
    float totalH = m_data.size() * rowHeight;
    // maxScroll = Tổng chiều cao nội dung - Chiều cao vùng hiển thị (height - header)
    m_maxScroll = std::max(0.0f, totalH - (height - headerHeight));
}

// Thêm một nhân viên vào bảng
void EmployeeTable::addRow(const NhanVien& nv) {
    m_data.push_back(nv);
    
    // Cập nhật lại thanh cuộn
    float totalH = m_data.size() * rowHeight;
    m_maxScroll = std::max(0.0f, totalH - (height - headerHeight));
    
    if (m_maxScroll > 0) m_scrollOffset = m_maxScroll;
}

// Cập nhật bảng mỗi frame
void EmployeeTable::update() {
}

// Xử lý cuộn và click xóa nhân viên
void EmployeeTable::handleEvent(const SDL_Event& e) {
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
    if (e.type == SDL_EVENT_MOUSE_BUTTON_UP && e.button.button == SDL_BUTTON_LEFT) {
        float mx = (float)e.button.x;
        float my = (float)e.button.y;
        SDL_FRect tableRect = {x, y, width, height};
        SDL_FPoint pt{mx, my};
        if (SDL_PointInRectFloat(&pt, &tableRect)) {
            size_t idx = rowIndexAt(my);
            if (idx < m_data.size()) {
                if (hitDeleteButton(mx, my, idx)) {
                    // Xóa nhân viên theo mã
                    const std::string code = m_data[idx].getMaNV();
                    if (NhanVien::remove(code)) {
                        reloadData();
                    }
                }
            }
        }
    }
}

// Vẽ một ô trong bảng
void EmployeeTable::renderCell(float cellX, float cellY, float cellW, float cellH, const std::string& text, bool isHeader) {
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

// Vẽ bảng nhân viên
void EmployeeTable::render() {
    SDL_Rect contentClip = {(int)x, (int)(y + headerHeight), (int)width, (int)(height - headerHeight)};
    SDL_SetRenderClipRect(m_renderer, &contentClip);

    float currentY = y + headerHeight - m_scrollOffset;

    for (size_t i = 0; i < m_data.size(); ++i) {
        if (currentY + rowHeight < y + headerHeight) { 
            currentY += rowHeight; 
            continue; 
        }
        if (currentY > y + height) break;

        const NhanVien& nv = m_data[i];
        float currentX = x;

        SDL_Color rowBg = (i % 2 == 0) ? m_rowColor1 : m_rowColor2;
        SDL_SetRenderDrawColor(m_renderer, rowBg.r, rowBg.g, rowBg.b, rowBg.a);
        SDL_FRect rowRect = {x, currentY, width, rowHeight};
        SDL_RenderFillRect(m_renderer, &rowRect);

        renderCell(currentX, currentY, m_colWidths[0], rowHeight, nv.getMaNV(), false); 
        currentX += m_colWidths[0];
        
        renderCell(currentX, currentY, m_colWidths[1], rowHeight, nv.getTen(), false); 
        currentX += m_colWidths[1];
        
        renderCell(currentX, currentY, m_colWidths[2], rowHeight, nv.getChucVu(), false); 
        currentX += m_colWidths[2];
        
        renderCell(currentX, currentY, m_colWidths[3], rowHeight, std::to_string(nv.getTuoi()), false); 
        currentX += m_colWidths[3];
        
        renderCell(currentX, currentY, m_colWidths[4], rowHeight, nv.getDiaChi(), false); 
        currentX += m_colWidths[4];
        
        renderCell(currentX, currentY, m_colWidths[5], rowHeight, nv.getSDT(), false);
        currentX += m_colWidths[5];

        SDL_Color delBg = {200, 60, 60, 255};
        SDL_SetRenderDrawColor(m_renderer, delBg.r, delBg.g, delBg.b, delBg.a);
        SDL_FRect delRect = {currentX + 10.0f, currentY + 10.0f, m_deleteColWidth - 20.0f, rowHeight - 20.0f};
        SDL_RenderFillRect(m_renderer, &delRect);
        SDL_SetRenderDrawColor(m_renderer, 120, 20, 20, 255);
        SDL_RenderRect(m_renderer, &delRect);
        renderCell(currentX, currentY, m_deleteColWidth, rowHeight, "X", true);

        currentY += rowHeight;
    }
    
    SDL_SetRenderClipRect(m_renderer, nullptr);

    float headerX = x;
    
    SDL_SetRenderDrawColor(m_renderer, m_headerColor.r, m_headerColor.g, m_headerColor.b, m_headerColor.a);
    SDL_FRect headerRect = {x, y, width, headerHeight};
    SDL_RenderFillRect(m_renderer, &headerRect);

    for (size_t i = 0; i < m_headers.size(); ++i) {
        renderCell(headerX, y, m_colWidths[i], headerHeight, m_headers[i], true);
        headerX += m_colWidths[i];
    }

    SDL_SetRenderDrawColor(m_renderer, m_borderColor.r, m_borderColor.g, m_borderColor.b, m_borderColor.a);
    SDL_FRect outline = {x, y, width, height};
    SDL_RenderRect(m_renderer, &outline);
}

// Tính chỉ số hàng theo tọa độ Y
size_t EmployeeTable::rowIndexAt(float my) const {
    float top = y + headerHeight;
    float relY = my - top + m_scrollOffset;
    if (relY < 0) return (size_t)-1;
    return (size_t)(relY / rowHeight);
}

// Kiểm tra click vào nút xóa của hàng
bool EmployeeTable::hitDeleteButton(float mx, float my, size_t rowIndex) const {
    float rowY = y + headerHeight + rowIndex * rowHeight - m_scrollOffset;
    float currentX = x;
    for (size_t i = 0; i < m_colWidths.size() - 1; ++i) {
        currentX += m_colWidths[i];
    }
    SDL_FRect delRect = {currentX + 10.0f, rowY + 10.0f, m_deleteColWidth - 20.0f, rowHeight - 20.0f};
    SDL_FPoint pt{mx, my};
    return SDL_PointInRectFloat(&pt, &delRect);
}
