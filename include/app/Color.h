#ifndef COLOR_H
#define COLOR_H

// --- Màu Cơ Bản ---
#define COLOR_WHITE         { 255, 255, 255, 255 }
#define COLOR_BLACK         { 0, 0, 0, 255 }
#define COLOR_RED           { 255, 0, 0, 255 }
#define COLOR_GREEN         { 0, 255, 0, 255 }
#define COLOR_BLUE          { 0, 0, 255, 255 }
#define COLOR_YELLOW        { 255, 255, 0, 255 }
#define COLOR_CYAN          { 0, 255, 255, 255 }
#define COLOR_MAGENTA       { 255, 0, 255, 255 }
#define COLOR_TRANSPARENT   { 0, 0, 0, 0 }

// --- Các Sắc Thái Xám ---
#define COLOR_GRAY_LIGHTEST { 248, 248, 248, 255 } // Gần như trắng
#define COLOR_GRAY_LIGHT    { 220, 220, 220, 255 } // Cho nền, viền
#define COLOR_GRAY_MEDIUM   { 128, 128, 128, 255 } // Cho văn bản phụ, vô hiệu hóa
#define COLOR_GRAY_DARK     { 80, 80, 80, 255 }
#define COLOR_GRAY_DARKEST  { 30, 30, 30, 255 }   // Gần như đen

// --- Màu Trạng Thái UI (Giao diện người dùng) ---
#define COLOR_PRIMARY       { 33, 150, 243, 255 }  // Xanh dương cho nút chính
#define COLOR_SECONDARY     { 158, 158, 158, 255 } // Xám cho nút phụ
#define COLOR_SUCCESS       { 76, 175, 80, 255 }   // Xanh lá (Hoàn thành)
#define COLOR_WARNING       { 255, 152, 0, 255 }   // Cam (Cảnh báo)
#define COLOR_DANGER        { 211, 47, 47, 255 }   // Đỏ (Lỗi, Xóa)
#define COLOR_INFO          { 0, 188, 212, 255 }    // Xanh lơ (Thông tin)

// --- Chủ Đề Quán Cà Phê ---

// Màu nền chính
#define COLOR_CREAM         { 245, 245, 220, 255 } // Màu kem (Beige)
#define COLOR_CREAM_LIGHT   { 253, 253, 240, 255 } // Màu trắng ngà

// Màu chữ chính (Tương phản tốt với nền kem)
#define COLOR_COFFEE_DARK   { 70, 48, 38, 255 }    // Nâu đậm (Espresso)

// Màu nhấn (Accent) và các thành phần
#define COLOR_COFFEE_MEDIUM { 139, 102, 82, 255 }   // Nâu vừa (Latte)
#define COLOR_COFFEE_LIGHT  { 188, 150, 131, 255 }  // Nâu sáng
#define COLOR_ACCENT_GREEN  { 46, 125, 50, 255 }    // Xanh lá (tươi, cho trạng thái "Online", "Sẵn sàng")
#define COLOR_ACCENT_GOLD   { 255, 200, 0, 255 }    // Vàng (cho khuyến mãi, đặc biệt)

// Màu cho các thành phần cụ thể
#define COLOR_TABLE_EMPTY   COLOR_SUCCESS          // Bàn trống
#define COLOR_TABLE_OCCUPIED COLOR_WARNING         // Bàn có khách
#define COLOR_TABLE_SELECTED COLOR_PRIMARY         // Bàn đang chọn

// Màu trong suốt (Overlay)
#define COLOR_MODAL_BACKDROP { 0, 0, 0, 150 }      // Nền mờ khi mở popup

#endif