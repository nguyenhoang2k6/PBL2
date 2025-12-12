#include <app/EmployeeTable.h>
#include <app/Color.h> // Bắt buộc phải include để dùng COLOR_CREAM
#include <algorithm> // Dùng cho std::max, std::min

// --- HÀM KHỞI TẠO (CONSTRUCTOR) ---
EmployeeTable::EmployeeTable(float x, float y, float w, float h, TTF_Font* font, SDL_Renderer* renderer)
    : x(x), y(y), width(w), height(h), m_font(font), m_renderer(renderer),
      // 1. CHỈNH ĐỘ CAO HÀNG Ở ĐÂY:
      rowHeight(50.0f),      // Hàng dữ liệu cao 50px (cho thoáng)
      headerHeight(60.0f),   // Tiêu đề cao 60px
      m_scrollOffset(0), m_maxScroll(0)
{
    // --- 2. CẤU HÌNH CỘT ---
    m_headers = {"Ma NV", "Ten NV", "Chuc Vu", "Tuoi", "Dia Chi", "SDT", "Xoa"};
    
    // Tổng chiều rộng: 100+220+150+60+320+150 = 1000 (Khớp với width bảng)
    m_colWidths = {
        200.0f,  // Mã NV
        800.0f,  // Tên
        400.0f,  // Chức vụ
        100.0f,   // Tuổi
        800.0f,  // Địa chỉ
        332.0f   // SĐT
    }; 
    m_deleteColWidth = 100.0f;
    // Tổng width cần đủ
    m_colWidths.push_back(m_deleteColWidth);

    // --- 3. CHỈNH MÀU SẮC (Quan trọng) ---
    // Để bảng "hòa nhập" với nền, ta dùng màu COLOR_CREAM (Màu kem)
    
    // Màu nền Header & Hàng: Trùng màu nền App
    m_headerColor = COLOR_CREAM; 
    m_rowColor1   = COLOR_CREAM;   
    m_rowColor2   = COLOR_CREAM;   // (Nếu muốn sọc, đổi cái này thành màu khác xíu)

    // Màu chữ: Đen (để nổi trên nền kem)
    m_textColor   = {0, 0, 0, 255};           

    // Màu viền: Nâu đậm (Rất quan trọng để phân biệt các ô khi nền trùng nhau)
    m_borderColor = {100, 80, 60, 255};   

    // Tự động tải dữ liệu ngay khi tạo bảng
    reloadData();
}

// --- HÀM TẢI LẠI DỮ LIỆU ---
void EmployeeTable::reloadData() {
    // Gọi hàm static từ class NhanVien để lấy list mới nhất
    m_data = NhanVien::getAll();
    
    // Tính toán lại vùng cuộn
    float totalH = m_data.size() * rowHeight;
    // maxScroll = Tổng chiều cao nội dung - Chiều cao vùng hiển thị (height - header)
    m_maxScroll = std::max(0.0f, totalH - (height - headerHeight));
}

// --- HÀM THÊM HÀNG MỚI (Real-time) ---
void EmployeeTable::addRow(const NhanVien& nv) {
    m_data.push_back(nv);
    
    // Cập nhật lại thanh cuộn
    float totalH = m_data.size() * rowHeight;
    m_maxScroll = std::max(0.0f, totalH - (height - headerHeight));
    
    // Tự động cuộn xuống cuối cùng để thấy nhân viên mới
    if (m_maxScroll > 0) m_scrollOffset = m_maxScroll;
}

// --- HÀM UPDATE (Logic mỗi frame) ---
void EmployeeTable::update() {
    // (Hiện tại để trống, có thể thêm hiệu ứng cuộn mượt sau này)
}

// --- HÀM XỬ LÝ SỰ KIỆN (Cuộn chuột) ---
void EmployeeTable::handleEvent(const SDL_Event& e) {
    if (e.type == SDL_EVENT_MOUSE_WHEEL) {
        // Kiểm tra chuột có nằm trong bảng không
        float mx, my;
        SDL_GetMouseState(&mx, &my);
        SDL_FPoint mousePoint = {mx, my};
        SDL_FRect tableRect = {x, y, width, height};

        if (SDL_PointInRectFloat(&mousePoint, &tableRect)) {
            // Cuộn: e.wheel.y > 0 là lăn lên, < 0 là lăn xuống
            // Nhân với 20 để cuộn nhanh hơn
            m_scrollOffset -= e.wheel.y * 20.0f;
            
            // Kẹp giá trị (Clamp) để không cuộn quá lố
            if(m_scrollOffset < 0) m_scrollOffset = 0;
            if(m_scrollOffset > m_maxScroll) m_scrollOffset = m_maxScroll;
        }
    }
    if (e.type == SDL_EVENT_MOUSE_BUTTON_UP && e.button.button == SDL_BUTTON_LEFT) {
        float mx = (float)e.button.x;
        float my = (float)e.button.y;
        // Chỉ xử lý nếu click trong vùng bảng
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

// --- HÀM VẼ MỘT Ô (CELL) ---
void EmployeeTable::renderCell(float cellX, float cellY, float cellW, float cellH, const std::string& text, bool isHeader) {
    // 1. Chọn màu chữ
    // Header có thể in đậm hoặc màu khác nếu muốn, ở đây ta dùng chung m_textColor cho đơn giản
    // Hoặc dùng màu riêng nếu bạn thích: 
    SDL_Color txtColor = isHeader ? SDL_Color{50, 30, 20, 255} : m_textColor; // Header màu nâu đậm hơn chút

    // 2. Vẽ Chữ (Có cắt lề - Clipping)
    if (m_font && !text.empty()) {
        // Thiết lập vùng cắt để chữ không tràn sang ô bên cạnh
        SDL_Rect clipRect = { (int)cellX, (int)cellY, (int)cellW, (int)cellH };
        SDL_SetRenderClipRect(m_renderer, &clipRect);

        // Render chữ
        SDL_Surface* surf = TTF_RenderText_Blended(m_font, text.c_str(), 0, txtColor);
        
        if (surf) {
            SDL_Texture* texture = SDL_CreateTextureFromSurface(m_renderer, surf);
            if (texture) {
                // Căn giữa theo chiều dọc, cách lề trái 10px
                float textX = cellX + 10.0f; 
                float textY = cellY + (cellH - surf->h) / 2.0f;

                SDL_FRect dst = { textX, textY, (float)surf->w, (float)surf->h };
                SDL_RenderTexture(m_renderer, texture, nullptr, &dst);
                SDL_DestroyTexture(texture);
            }
            SDL_DestroySurface(surf);
        }
        
        // Hủy vùng cắt
        SDL_SetRenderClipRect(m_renderer, nullptr);
    }

    // 3. Vẽ Viền Ô (Rất quan trọng khi nền bảng trùng nền App)
    SDL_SetRenderDrawColor(m_renderer, m_borderColor.r, m_borderColor.g, m_borderColor.b, m_borderColor.a);
    SDL_FRect borderRect = { cellX, cellY, cellW, cellH };
    SDL_RenderRect(m_renderer, &borderRect);
}

// --- HÀM VẼ TOÀN BỘ BẢNG (RENDER) ---
void EmployeeTable::render() {
    // --- 1. VẼ PHẦN NỘI DUNG (DATA) ---
    // Thiết lập vùng cắt cho nội dung (để khi cuộn, nó không đè lên header hay tràn ra ngoài dưới đáy)
    SDL_Rect contentClip = {(int)x, (int)(y + headerHeight), (int)width, (int)(height - headerHeight)};
    SDL_SetRenderClipRect(m_renderer, &contentClip);

    float currentY = y + headerHeight - m_scrollOffset;

    for (size_t i = 0; i < m_data.size(); ++i) {
        // Tối ưu: Không vẽ những hàng nằm ngoài vùng nhìn thấy
        if (currentY + rowHeight < y + headerHeight) { 
            currentY += rowHeight; 
            continue; 
        }
        if (currentY > y + height) break;

        const NhanVien& nv = m_data[i];
        float currentX = x;

        // Vẽ nền hàng (Sọc ngựa vằn nếu muốn, ở đây ta để giống nhau theo ý bạn)
        SDL_Color rowBg = (i % 2 == 0) ? m_rowColor1 : m_rowColor2;
        SDL_SetRenderDrawColor(m_renderer, rowBg.r, rowBg.g, rowBg.b, rowBg.a);
        SDL_FRect rowRect = {x, currentY, width, rowHeight};
        SDL_RenderFillRect(m_renderer, &rowRect);

        // Vẽ từng cột dữ liệu
        // Cột 0: Mã
        renderCell(currentX, currentY, m_colWidths[0], rowHeight, nv.getMaNV(), false); 
        currentX += m_colWidths[0];
        
        // Cột 1: Tên
        renderCell(currentX, currentY, m_colWidths[1], rowHeight, nv.getTen(), false); 
        currentX += m_colWidths[1];
        
        // Cột 2: Chức vụ
        renderCell(currentX, currentY, m_colWidths[2], rowHeight, nv.getChucVu(), false); 
        currentX += m_colWidths[2];
        
        // Cột 3: Tuổi (chuyển int sang string)
        renderCell(currentX, currentY, m_colWidths[3], rowHeight, std::to_string(nv.getTuoi()), false); 
        currentX += m_colWidths[3];
        
        // Cột 4: Địa chỉ
        renderCell(currentX, currentY, m_colWidths[4], rowHeight, nv.getDiaChi(), false); 
        currentX += m_colWidths[4];
        
        // Cột 5: SĐT
        renderCell(currentX, currentY, m_colWidths[5], rowHeight, nv.getSDT(), false);
        currentX += m_colWidths[5];

        // Cột 6: Nút Xóa
        // Vẽ nền nút
        SDL_Color delBg = {200, 60, 60, 255};
        SDL_SetRenderDrawColor(m_renderer, delBg.r, delBg.g, delBg.b, delBg.a);
        SDL_FRect delRect = {currentX + 10.0f, currentY + 10.0f, m_deleteColWidth - 20.0f, rowHeight - 20.0f};
        SDL_RenderFillRect(m_renderer, &delRect);
        // Viền
        SDL_SetRenderDrawColor(m_renderer, 120, 20, 20, 255);
        SDL_RenderRect(m_renderer, &delRect);
        // Chữ "X"
        renderCell(currentX, currentY, m_deleteColWidth, rowHeight, "X", true);

        currentY += rowHeight;
    }
    
    // Hủy vùng cắt nội dung
    SDL_SetRenderClipRect(m_renderer, nullptr);

    // --- 2. VẼ HEADER (Vẽ đè lên trên cùng) ---
    float headerX = x;
    
    // Vẽ nền Header
    SDL_SetRenderDrawColor(m_renderer, m_headerColor.r, m_headerColor.g, m_headerColor.b, m_headerColor.a);
    SDL_FRect headerRect = {x, y, width, headerHeight};
    SDL_RenderFillRect(m_renderer, &headerRect);

    // Vẽ từng ô Header
    for (size_t i = 0; i < m_headers.size(); ++i) {
        renderCell(headerX, y, m_colWidths[i], headerHeight, m_headers[i], true);
        headerX += m_colWidths[i];
    }

    // Vẽ khung viền bao quanh toàn bộ bảng
    SDL_SetRenderDrawColor(m_renderer, m_borderColor.r, m_borderColor.g, m_borderColor.b, m_borderColor.a);
    SDL_FRect outline = {x, y, width, height};
    SDL_RenderRect(m_renderer, &outline);
}

// --- Helpers ---
size_t EmployeeTable::rowIndexAt(float my) const {
    // Tính vị trí hàng từ tọa độ y chuột
    float top = y + headerHeight;
    float relY = my - top + m_scrollOffset;
    if (relY < 0) return (size_t)-1;
    return (size_t)(relY / rowHeight);
}

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