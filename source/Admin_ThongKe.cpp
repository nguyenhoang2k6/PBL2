#include <app/Admin_ThongKe.h>
#include <app/Button.h>
#include <app/Label.h>
#include <app/TextBox.h>
#include <app/Color.h>// Để dùng màu sắc chuẩn

Admin_ThongKe::Admin_ThongKe(App* app) : Screen(app) {
    // Khởi tạo nullptr
    m_lblTitle = nullptr; m_lblTotalRevenue = nullptr;
    m_txtDay = nullptr; m_txtMonth = nullptr; m_txtYear = nullptr; m_txtMaNV = nullptr;
    m_btnByDay = nullptr; m_btnByMonth = nullptr; m_btnByYear = nullptr; m_btnByStaff = nullptr;
    m_btnSearch = nullptr; m_btnBack = nullptr;
    m_table = nullptr;
    
    m_currentMode = VIEW_DAY; // Mặc định vào là xem theo ngày
}

Admin_ThongKe::~Admin_ThongKe() {
    delete m_lblTitle;
    delete m_lblTotalRevenue;
    delete m_txtDay;
    delete m_txtMonth;
    delete m_txtYear;
    delete m_txtMaNV;
    delete m_btnByDay;
    delete m_btnByMonth;
    delete m_btnByYear;
    delete m_btnByStaff;
    delete m_btnSearch;
    delete m_btnBack;
    delete m_table;
}

bool Admin_ThongKe::Init() {
    TTF_Font* font = app->getFont1();     // Font thường
    TTF_Font* fontTitle = app->getFont2(); // Font to hơn cho tiêu đề (nếu có)
    if (!fontTitle) fontTitle = font;       // Fallback nếu không có font2
    
    SDL_Renderer* renderer = app->getRenderer();
    
    // 1. Tiêu đề
    m_lblTitle = new Label("THONG KE DOANH THU", COLOR_WARNING, 1000, 100, fontTitle, renderer);

    // 2. Các nút chọn chế độ (Tab) - Hàng ngang
    // Màu xám cho nút chưa chọn, màu xanh cho nút đang chọn (xử lý trong update)
    float startX = 100;
    float btnW = 275;
    float gap = 50;
    
    m_btnByDay   = new Button(startX, 180, btnW, 75, {200,200,200,255}, "Theo Ngay", renderer, font, {0,0,0,255});
    m_btnByMonth = new Button(startX + (btnW+gap)*1, 180, btnW, 75, {200,200,200,255}, "Theo Thang", renderer, font, {0,0,0,255});
    m_btnByYear  = new Button(startX + (btnW+gap)*2, 180, btnW, 75, {200,200,200,255}, "Theo Nam", renderer, font, {0,0,0,255});
    m_btnByStaff = new Button(startX + (btnW+gap)*3, 180, btnW, 75, {200,200,200,255}, "Nhan Vien", renderer, font, {0,0,0,255});



    m_txtDay   = new TextBox(50, 305, 200, 40, COLOR_WHITE, COLOR_BLACK, app, font, "Ngay", COLOR_GRAY_SILVER);
    m_txtMonth = new TextBox(300, 305, 200, 40, COLOR_WHITE, COLOR_BLACK, app, font, "Thang", COLOR_GRAY_SILVER);
    m_txtYear  = new TextBox(550, 305, 200, 40, COLOR_WHITE, COLOR_BLACK, app, font, "Nam", COLOR_GRAY_SILVER);
    m_txtMaNV  = new TextBox(800, 305, 200, 40, COLOR_WHITE, COLOR_BLACK, app, font, "Ma NV", COLOR_GRAY_SILVER);

    // 4. Nút Tìm kiếm & Quay lại
    m_btnSearch = new Button(1050, 305, 250, 75,COLOR_UI_GREEN, "Xem", renderer, font, COLOR_WHITE);
    m_btnBack   = new Button(2482, 0, 250, 75, COLOR_UI_RED, "Back", renderer, font, COLOR_WHITE); // Góc trên phải

    // 5. Label Tổng kết
    m_lblTotalRevenue = new Label("Tong: 0 VND",COLOR_GREEN, 50, 395, font, renderer);

    // 6. Bảng hiển thị (Table)
    // Định nghĩa cột cho bảng Doanh Thu
    std::vector<std::string> headers = {"Ma HD", "Ngay Lap", "Doanh Thu", "Nguoi Lap"};
    std::vector<float> widths = {400.0f, 500.0f, 600.0f, 1232.0f}; // Tổng 1000px

    m_table = new Table(0, 450,2732,1086 , font, renderer, headers, widths);

    // Cài đặt màu ban đầu cho nút chế độ
    resetButtonColors();

    return true;
}

void Admin_ThongKe::resetButtonColors() {
    SDL_Color normalColor = {220, 220, 220, 255};
    SDL_Color activeColor = {100, 200, 255, 255}; // Màu xanh nhạt khi chọn

    m_btnByDay->setColor(m_currentMode == VIEW_DAY ? activeColor : normalColor);
    m_btnByMonth->setColor(m_currentMode == VIEW_MONTH ? activeColor : normalColor);
    m_btnByYear->setColor(m_currentMode == VIEW_YEAR ? activeColor : normalColor);
    m_btnByStaff->setColor(m_currentMode == VIEW_STAFF ? activeColor : normalColor);
}

void Admin_ThongKe::onEnter() {
    // Reset dữ liệu mỗi khi vào màn hình
    m_txtDay->setText("");
    m_txtMonth->setText("");
    m_txtYear->setText("");
    m_txtMaNV->setText("");
    m_lblTotalRevenue->setText("Tong: 0 VND");
    m_table->clear();
    
    // Reset chế độ về Ngày
    m_currentMode = VIEW_DAY;
    resetButtonColors();
}

void Admin_ThongKe::onExit() {
    // Dừng nhập liệu cho an toàn
    SDL_StopTextInput(app->getWindow());
}

void Admin_ThongKe::update() {
    // 1. Cập nhật các nút
    m_btnByDay->update();
    m_btnByMonth->update();
    m_btnByYear->update();
    m_btnByStaff->update();
    m_btnSearch->update();
    m_btnBack->update();

    // 2. Xử lý chuyển tab chế độ
    if (m_btnByDay->isClicked()) { m_currentMode = VIEW_DAY; resetButtonColors(); }
    if (m_btnByMonth->isClicked()) { m_currentMode = VIEW_MONTH; resetButtonColors(); }
    if (m_btnByYear->isClicked()) { m_currentMode = VIEW_YEAR; resetButtonColors(); }
    if (m_btnByStaff->isClicked()) { m_currentMode = VIEW_STAFF; resetButtonColors(); }

    // 3. Cập nhật TextBox (CHỈ update những ô đang hiển thị để tránh lỗi focus ảo)
    switch (m_currentMode) {
        case VIEW_DAY:
            m_txtDay->update(); m_txtMonth->update(); m_txtYear->update();
            break;
        case VIEW_MONTH:
            m_txtMonth->update(); m_txtYear->update();
            break;
        case VIEW_YEAR:
            m_txtYear->update();
            break;
        case VIEW_STAFF:
            m_txtMaNV->update();
            break;
    }

    // 4. Xử lý tìm kiếm
    if (m_btnSearch->isClicked()) {
        performSearch();
    }

    // 5. Xử lý nút Back (Quay về Dashboard hoặc Menu)
    if (m_btnBack->isClicked()) {
        app->changeScreen("AdminDasBoard"); // Hoặc tên màn hình bạn muốn
    }
}

void Admin_ThongKe::performSearch() {
    std::vector<ThongKeItem> results;
    
    try {
        if (m_currentMode == VIEW_DAY) {
            std::string dStr = m_txtDay->getText();
            std::string mStr = m_txtMonth->getText();
            std::string yStr = m_txtYear->getText();
            if(dStr.empty() || mStr.empty() || yStr.empty()) return; // Bỏ qua nếu rỗng

            int d = std::stoi(dStr);
            int m = std::stoi(mStr);
            int y = std::stoi(yStr);
            results = ThongKe::getByDay(d, m, y);
        } 
        else if (m_currentMode == VIEW_MONTH) {
            std::string mStr = m_txtMonth->getText();
            std::string yStr = m_txtYear->getText();
            if(mStr.empty() || yStr.empty()) return;

            int m = std::stoi(mStr);
            int y = std::stoi(yStr);
            results = ThongKe::getByMonth(m, y);
        }
        else if (m_currentMode == VIEW_YEAR) {
            std::string yStr = m_txtYear->getText();
            if(yStr.empty()) return;

            int y = std::stoi(yStr);
            results = ThongKe::getByYear(y);
        }
        else if (m_currentMode == VIEW_STAFF) {
            std::string ma = m_txtMaNV->getText();
            if(ma.empty()) return;
            results = ThongKe::getByStaff(ma);
        }
    } 
    catch (...) {
        std::cerr << "Loi nhap lieu: Khong phai so!" << std::endl;
        return;
    }

    // --- CẬP NHẬT GIAO DIỆN ---

    // 1. Tính tổng
    long long total = ThongKe::calculateTotal(results);
    m_lblTotalRevenue->setText("Tong: " + std::to_string(total) + " VND");

    // 2. Đổ vào bảng
    m_table->clear();
    for (const auto& item : results) {
        // Chuyển đổi dữ liệu sang vector<string> để Table hiển thị
        m_table->addRow({
            item.maHD, 
            item.ngay, 
            std::to_string(item.doanhThu), 
            item.maNV
        });
    }
}

void Admin_ThongKe::handleEvent(const SDL_Event& e) {
    // Nút luôn nhận sự kiện
    m_btnByDay->handleEvent(e);
    m_btnByMonth->handleEvent(e);
    m_btnByYear->handleEvent(e);
    m_btnByStaff->handleEvent(e);
    m_btnSearch->handleEvent(e);
    m_btnBack->handleEvent(e);

    if(m_btnByDay->isClicked()) {
        m_currentMode = VIEW_DAY;
        resetButtonColors();
    }
    if (m_btnByMonth->isClicked()){
        m_currentMode = VIEW_MONTH;
        resetButtonColors();
    } 
    if (m_btnByYear->isClicked()) {
        m_currentMode = VIEW_YEAR;
        resetButtonColors();
    }
    if (m_btnByStaff->isClicked()) {
        m_currentMode = VIEW_STAFF;
        resetButtonColors();
    }
        // Nút Back không thay đổi chế độ
    // TextBox chỉ nhận sự kiện nếu đang ở chế độ tương ứng
    // Điều này ngăn người dùng click vào ô "Ngày" khi đang ở chế độ "Theo Năm" (vốn ô Ngày bị ẩn)
    switch (m_currentMode) {
        case VIEW_DAY:
            m_txtDay->handleEvent(e);
            m_txtMonth->handleEvent(e);
            m_txtYear->handleEvent(e);
            break;
        case VIEW_MONTH:
            m_txtMonth->handleEvent(e);
            m_txtYear->handleEvent(e);
            break;
        case VIEW_YEAR:
            m_txtYear->handleEvent(e);
            break;
        case VIEW_STAFF:
            m_txtMaNV->handleEvent(e);
            break;
    }

    // Bảng nhận sự kiện (để cuộn)
    m_table->handleEvent(e);
    if(m_btnBack->isClicked()) {
        app->changeScreen("AdminDashBoard");
    }
}

void Admin_ThongKe::render(SDL_Renderer* renderer) {
    // 1. Vẽ các nút chọn chế độ
    m_btnByDay->render(renderer);
    m_btnByMonth->render(renderer);
    m_btnByYear->render(renderer);
    m_btnByStaff->render(renderer);

    // 2. Vẽ TextBox theo chế độ (LOGIC ẨN/HIỆN)
    switch (m_currentMode) {
        case VIEW_DAY:
            m_txtDay->render(renderer);
            m_txtMonth->render(renderer);
            m_txtYear->render(renderer);
            break;
        case VIEW_MONTH:
            // Có thể cần điều chỉnh vị trí TextBox một chút cho đẹp nếu muốn,
            // nhưng ở đây ta vẽ đè lên vị trí cũ cũng được
            m_txtMonth->render(renderer);
            m_txtYear->render(renderer);
            break;
        case VIEW_YEAR:
            m_txtYear->render(renderer);
            break;
        case VIEW_STAFF:
            m_txtMaNV->render(renderer);
            break;
    }

    // 3. Vẽ các thành phần chung
    m_lblTitle->render(renderer);
    m_btnSearch->render(renderer);
    m_btnBack->render(renderer);
    m_lblTotalRevenue->render(renderer);
    
    // 4. Vẽ bảng
    m_table->render();
}