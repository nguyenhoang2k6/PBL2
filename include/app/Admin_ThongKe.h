#pragma once
#include "Screen.h"
#include "App.h"
#include "Table.h"     // Class Table tổng quát
#include "Thongke.h"   // Class Logic Backend
#include <vector>
#include <string>

// Khai báo trước
class Button;
class Label;
class TextBox;

class Admin_ThongKe : public Screen {
private:
    // --- UI COMPONENTS ---
    Label* m_lblTitle;
    Label* m_lblTotalRevenue; // Label hiển thị tổng tiền
    
    // Nhóm nhập liệu
    TextBox* m_txtDay;
    TextBox* m_txtMonth;
    TextBox* m_txtYear;
    TextBox* m_txtMaNV;
    
    // Nút chọn chế độ
    Button* m_btnByDay;
    Button* m_btnByMonth;
    Button* m_btnByYear;
    Button* m_btnByStaff;
    
    // Nút chức năng
    Button* m_btnSearch; // Nút "Xem"
    Button* m_btnBack;   // Nút "Quay lại"

    // Bảng hiển thị kết quả
    Table* m_table; 
    float m_tableWidth;
    float m_tableHeight;

    // --- TRẠNG THÁI ---
    enum Mode { VIEW_DAY, VIEW_MONTH, VIEW_YEAR, VIEW_STAFF };
    Mode m_currentMode; // Chế độ hiện tại đang chọn

    // Hàm phụ trợ để reset màu các nút chọn chế độ
    void resetButtonColors();

    // Hàm phụ trợ để thực hiện tìm kiếm và đổ dữ liệu vào bảng
    void performSearch();

public:
    Admin_ThongKe(App* app);
    ~Admin_ThongKe();

    bool Init() override;
    void handleEvent(const SDL_Event& e) override;
    void update() override;
    void render(SDL_Renderer* renderer) override;
    
    void onEnter() override; // Reset dữ liệu khi vào màn hình
    void onExit() override;
};