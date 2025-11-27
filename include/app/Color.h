#ifndef COLOR_H
#define COLOR_H

// ========================================================
// 1. Màu cơ bản (BASIC)
// ========================================================
#define COLOR_WHITE         { 255, 255, 255, 255 }
#define COLOR_BLACK         { 0, 0, 0, 255 }
#define COLOR_TRANSPARENT   { 0, 0, 0, 0 }

// Các màu cơ bản rực rỡ
#define COLOR_RED           { 255, 0, 0, 255 }
#define COLOR_GREEN         { 0, 255, 0, 255 }
#define COLOR_BLUE          { 0, 0, 255, 255 }
#define COLOR_YELLOW        { 255, 255, 0, 255 }
#define COLOR_CYAN          { 0, 255, 255, 255 }
#define COLOR_MAGENTA       { 255, 0, 255, 255 }

// Mới thêm: Các màu cơ bản mở rộng (cần thiết)
#define COLOR_ORANGE        { 255, 165, 0, 255 }   // Cam chuẩn
#define COLOR_PURPLE        { 128, 0, 128, 255 }   // Tím chuẩn
#define COLOR_PINK          { 255, 192, 203, 255 } // Hồng nhạt
#define COLOR_TEAL          { 0, 128, 128, 255 }   // Xanh cổ vịt (đậm hơn Cyan)
#define COLOR_NAVY          { 0, 0, 128, 255 }     // Xanh dương đậm (hải quân)
#define COLOR_LIME          { 50, 205, 50, 255 }   // Xanh chanh

// ========================================================
// 2. SẮC THÁI XÁM (GRAYSCALE) - Cho nền và viền
// ========================================================
#define COLOR_GRAY_LIGHTEST { 248, 248, 248, 255 } // Gần như trắng (nền app)
#define COLOR_GRAY_LIGHT    { 220, 220, 220, 255 } // Viền (Border)
#define COLOR_GRAY_SILVER   { 192, 192, 192, 255 } // Bạc
#define COLOR_GRAY_MEDIUM   { 128, 128, 128, 255 } // Text phụ (Disabled text)
#define COLOR_GRAY_DARK     { 80, 80, 80, 255 }    // Text chính trên nền sáng
#define COLOR_GRAY_CHARCOAL { 54, 69, 79, 255 }    // Màu than chì (Đẹp hơn đen tuyền)
#define COLOR_GRAY_DARKEST  { 30, 30, 30, 255 }    // Gần như đen (Nền Dark mode)

// ========================================================
// 3. MÀU UI HIỆN ĐẠI (MODERN UI / FLAT COLORS)
// Dùng cho các nút bấm để trông chuyên nghiệp hơn màu cơ bản
// ========================================================
#define COLOR_UI_BLUE       { 52, 152, 219, 255 }  // Xanh dương dịu (Peter River)
#define COLOR_UI_GREEN      { 46, 204, 113, 255 }  // Xanh lá ngọc (Emerald)
#define COLOR_UI_RED        { 231, 76, 60, 255 }   // Đỏ cam (Alizarin) - Đỡ chói hơn Red
#define COLOR_UI_ORANGE     { 230, 126, 34, 255 }  // Cam cà rốt
#define COLOR_UI_PURPLE     { 155, 89, 182, 255 }  // Tím mộng mơ (Amethyst)
#define COLOR_UI_TURQUOISE  { 26, 188, 156, 255 }  // Xanh ngọc lam
#define COLOR_UI_MIDNIGHT   { 44, 62, 80, 255 }    // Xanh đen thẫm (Cho Header/Footer)

// ========================================================
// 4. MÀU PASTEL (NHẸ NHÀNG)
// Dùng cho nền thông báo (Notification) hoặc các thẻ (Tags)
// ========================================================
#define COLOR_PASTEL_RED    { 255, 179, 186, 255 }
#define COLOR_PASTEL_ORANGE { 255, 223, 186, 255 }
#define COLOR_PASTEL_YELLOW { 255, 255, 186, 255 }
#define COLOR_PASTEL_GREEN  { 186, 255, 201, 255 }
#define COLOR_PASTEL_BLUE   { 186, 225, 255, 255 }
#define COLOR_PASTEL_PURPLE { 225, 198, 255, 255 }

// ========================================================
// 5. CHỦ ĐỀ QUÁN CÀ PHÊ (COFFEE SHOP THEME)
// Mở rộng các tông màu gỗ, đất, và menu bảng phấn
// ========================================================

// Nền và Giấy
#define COLOR_CREAM         { 245, 245, 220, 255 } // Kem (Beige)
#define COLOR_CREAM_LIGHT   { 253, 253, 240, 255 } // Trắng ngà
#define COLOR_PAPER         { 240, 230, 210, 255 } // Màu giấy cũ

// Tông Nâu (Cà phê & Gỗ)
#define COLOR_COFFEE_BLACK  { 59, 47, 47, 255 }    // Cà phê đen
#define COLOR_COFFEE_DARK   { 70, 48, 38, 255 }    // Espresso
#define COLOR_COFFEE_MEDIUM { 139, 102, 82, 255 }  // Latte
#define COLOR_COFFEE_LIGHT  { 188, 150, 131, 255 } // Bạc xỉu
#define COLOR_WOOD_OAK      { 128, 101, 23, 255 }  // Gỗ sồi (Vàng nâu)
#define COLOR_WOOD_DARK     { 92, 64, 51, 255 }    // Gỗ mun
#define COLOR_CHOCOLATE     { 210, 105, 30, 255 }  // Socola

// Tông màu Bảng Phấn (Chalkboard) - Rất hợp làm menu
#define COLOR_SLATE_GREEN   { 47, 79, 79, 255 }    // Xanh đá phiến (Bảng xanh đen)
#define COLOR_CHALK_WHITE   { 240, 240, 240, 220 } // Màu phấn trắng (hơi mờ)

// Điểm nhấn (Highlights)
#define COLOR_ACCENT_GREEN  { 46, 125, 50, 255 }   // Xanh lá đậm (Online)
#define COLOR_ACCENT_GOLD   { 255, 215, 0, 255 }   // Vàng kim loại (VIP/Star)
#define COLOR_ACCENT_BRICK  { 178, 34, 34, 255 }   // Đỏ gạch (Firebrick) - Cho giá tiền

// ========================================================
// 6. TRẠNG THÁI UI (Semantic Colors)
// ========================================================
#define COLOR_PRIMARY       { 33, 150, 243, 255 }  // Nút chính
#define COLOR_SECONDARY     { 158, 158, 158, 255 } // Nút phụ
#define COLOR_SUCCESS       { 76, 175, 80, 255 }   // Thành công
#define COLOR_WARNING       { 255, 152, 0, 255 }   // Cảnh báo
#define COLOR_DANGER        { 211, 47, 47, 255 }   // Lỗi/Xóa
#define COLOR_INFO          { 0, 188, 212, 255 }   // Thông tin

// ========================================================
// 7. MÀU ĐẶC BIỆT CHO BÀN (Table Status)
// ========================================================
#define COLOR_TABLE_EMPTY   COLOR_SUCCESS          // Bàn trống
#define COLOR_TABLE_OCCUPIED COLOR_WARNING         // Có khách
#define COLOR_TABLE_RESERVED { 156, 39, 176, 255 } // Đã đặt trước (Tím)
#define COLOR_TABLE_DIRTY   { 121, 85, 72, 255 }   // Cần dọn dẹp (Nâu đất)
#define COLOR_TABLE_SELECTED COLOR_UI_BLUE         // Đang chọn

// Lớp phủ mờ (Overlay)
#define COLOR_MODAL_BACKDROP { 0, 0, 0, 180 }      // Đen mờ (đậm hơn cũ một chút)

#endif