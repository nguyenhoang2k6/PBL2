#pragma once
#include "Screen.h"
#include "App.h"
#include "Table.h"
#include "Thongke.h"
#include <vector>
#include <string>

// Khai báo trước
class Button;
class Label;
class TextBox;

class Admin_ThongKe : public Screen {
private:
    Label* m_lblTitle;
    Label* m_lblTotalRevenue;
    
    TextBox* m_txtDay;
    TextBox* m_txtMonth;
    TextBox* m_txtYear;
    TextBox* m_txtMaNV;
    
    Button* m_btnByDay;
    Button* m_btnByMonth;
    Button* m_btnByYear;
    Button* m_btnByStaff;
    
    Button* m_btnSearch;
    Button* m_btnBack;

    Table* m_table; 
    float m_tableWidth;
    float m_tableHeight;

    // --- TRẠNG THÁI ---
    enum Mode { VIEW_DAY, VIEW_MONTH, VIEW_YEAR, VIEW_STAFF };
    Mode m_currentMode;

    // Hàm phụ trợ để reset màu các nút chọn chế độ
    void resetButtonColors();

    void performSearch();

public:
    Admin_ThongKe(App* app);
    ~Admin_ThongKe();

    bool Init() override;
    void handleEvent(const SDL_Event& e) override;
    void update() override;
    void render(SDL_Renderer* renderer) override;
    
    void onEnter() override;
    void onExit() override;
};
