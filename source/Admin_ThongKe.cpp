#include <app/Admin_ThongKe.h>
#include <app/Button.h>
#include <app/Label.h>
#include <app/TextBox.h>
#include <app/Color.h>// Để dùng màu sắc chuẩn
#include <app/Nhanvien.h>
#include <filesystem>
#include <algorithm>
#include <cctype>

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
    m_lblTitle = new Label("THỐNG KÊ DOANH THU", COLOR_WARNING, 1000, 100, fontTitle, renderer);

    // 2. Các nút chọn chế độ (Tab) - Hàng ngang
    // Màu xám cho nút chưa chọn, màu xanh cho nút đang chọn (xử lý trong update)
    float startX = 100;
    float btnW = 275;
    float gap = 50;
    
    m_btnByDay   = new Button(startX, 180, btnW, 75, {200,200,200,255}, "Theo Ngày", renderer, font, {0,0,0,255});
    m_btnByMonth = new Button(startX + (btnW+gap)*1, 180, btnW, 75, {200,200,200,255}, "Theo Tháng", renderer, font, {0,0,0,255});
    m_btnByYear  = new Button(startX + (btnW+gap)*2, 180, btnW, 75, {200,200,200,255}, "Theo Năm", renderer, font, {0,0,0,255});
    m_btnByStaff = new Button(startX + (btnW+gap)*3, 180, btnW, 75, {200,200,200,255}, "Nhân Viên", renderer, font, {0,0,0,255});



    m_txtDay   = new TextBox(50, 305, 200, 40, COLOR_WHITE, COLOR_BLACK, app, font, "Ngày", COLOR_GRAY_SILVER);
    m_txtMonth = new TextBox(300, 305, 200, 40, COLOR_WHITE, COLOR_BLACK, app, font, "Tháng", COLOR_GRAY_SILVER);
    m_txtYear  = new TextBox(550, 305, 200, 40, COLOR_WHITE, COLOR_BLACK, app, font, "Năm", COLOR_GRAY_SILVER);
    m_txtMaNV  = new TextBox(800, 305, 200, 40, COLOR_WHITE, COLOR_BLACK, app, font, "Mã NV", COLOR_GRAY_SILVER);

    // 4. Nút Tìm kiếm & Quay lại
    m_btnSearch = new Button(1300, 305, 200, 60,COLOR_UI_GREEN, "Xem", renderer, font, COLOR_WHITE);
    m_btnBack   = new Button(2482, 0, 250, 75, COLOR_UI_RED, "Quay lại", renderer, font, COLOR_WHITE); // Góc trên phải

    // 5. Label Tổng kết
    m_lblTotalRevenue = new Label("Tổng: 0 VND",COLOR_GREEN, 50, 395, font, renderer);

    // 6. Bảng hiển thị (Table)
    // Định nghĩa cột cho bảng Doanh Thu
    std::vector<std::string> headers = {"Mã HĐ", "Ngày lập", "Doanh thu", "Người lập", "Ghi chú"};
    // Chia lại độ rộng: thu hẹp cột "Ghi chú" và tăng không gian cho các cột còn lại
    std::vector<float> widths = {500.0f, 500.0f, 520.0f, 420.0f, 792.0f};

    m_tableWidth = 2732;
    m_tableHeight = 1086;
    m_table = new Table(0, 450, m_tableWidth, m_tableHeight, font, renderer, headers, widths);

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
    m_lblTotalRevenue->setColor(COLOR_GREEN);
    m_lblTotalRevenue->setText("Tổng: 0 VND");
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
}

void Admin_ThongKe::performSearch() {
    auto trim = [](std::string s) {
        while (!s.empty() && std::isspace(static_cast<unsigned char>(s.front()))) s.erase(s.begin());
        while (!s.empty() && std::isspace(static_cast<unsigned char>(s.back()))) s.pop_back();
        return s;
    };
    auto isDigits = [](const std::string& s) {
        return !s.empty() && std::all_of(s.begin(), s.end(), [](unsigned char c){ return std::isdigit(c); });
    };
    auto formatCurrency = [](long long value) {
        std::string s = std::to_string(value);
        std::string out;
        int count = 0;
        for (int i = static_cast<int>(s.size()) - 1; i >= 0; --i) {
            out.push_back(s[i]);
            if (++count == 3 && i != 0) {
                out.push_back('.');
                count = 0;
            }
        }
        std::reverse(out.begin(), out.end());
        out += "đ";
        return out;
    };
    auto setError = [this](const std::string& msg) {
        m_lblTotalRevenue->setColor({255, 0, 0, 255});
        m_lblTotalRevenue->setText(msg);
        m_table->clear();
    };
    auto setSuccess = [this](const std::string& msg) {
        m_lblTotalRevenue->setColor(COLOR_GREEN);
        m_lblTotalRevenue->setText(msg);
    };

    std::vector<ThongKeItem> results;
    
    try {
        if (m_currentMode == VIEW_DAY) {
            std::string dStr = trim(m_txtDay->getText());
            std::string mStr = trim(m_txtMonth->getText());
            std::string yStr = trim(m_txtYear->getText());
            if(!isDigits(dStr) || !isDigits(mStr) || !isDigits(yStr)) {
                setError("Ngày/Tháng/Năm không hợp lệ");
                return;
            }

            int d = std::stoi(dStr);
            int m = std::stoi(mStr);
            int y = std::stoi(yStr);
            if (!ThongKe::isValidDate(d, m, y)) {
                setError("Ngày/Tháng/Năm không hợp lệ");
                return;
            }
            results = ThongKe::getByDay(d, m, y);
        } 
        else if (m_currentMode == VIEW_MONTH) {
            std::string mStr = trim(m_txtMonth->getText());
            std::string yStr = trim(m_txtYear->getText());
            if(!isDigits(mStr) || !isDigits(yStr)) {
                setError("Tháng/Năm không hợp lệ");
                return;
            }

            int m = std::stoi(mStr);
            int y = std::stoi(yStr);
            if (m < 1 || m > 12 || y < 0) {
                setError("Tháng/Năm không hợp lệ");
                return;
            }
            results = ThongKe::getByMonth(m, y);
        }
        else if (m_currentMode == VIEW_YEAR) {
            std::string yStr = trim(m_txtYear->getText());
            if(!isDigits(yStr)) {
                setError("Năm không hợp lệ");
                return;
            }

            int y = std::stoi(yStr);
            if (y < 0) {
                setError("Năm không hợp lệ");
                return;
            }
            results = ThongKe::getByYear(y);
        }
        else if (m_currentMode == VIEW_STAFF) {
            std::string ma = trim(m_txtMaNV->getText());
            if(ma.empty()) {
                setError("Mã NV không được để trống");
                return;
            }
            if(!NhanVien::exists(ma)) {
                setError("Mã NV không tồn tại");
                return;
            }
            results = ThongKe::getByStaff(ma);
        }
    } 
    catch (...) {
        setError("Không tìm thấy hóa đơn hợp lệ");
        return;
    }

    // --- Lọc chỉ lấy hóa đơn có file tồn tại ---
    std::vector<ThongKeItem> valid;
    for (const auto& item : results) {
        std::filesystem::path p = std::filesystem::path("data/Hoadon") / (item.maHD + ".txt");
        if (std::filesystem::exists(p)) {
            valid.push_back(item);
        }
    }

    // --- CẬP NHẬT GIAO DIỆN ---
    if (valid.empty()) {
        setError("Khong tim thay hoa don hop le");
        return;
    }

    // Trả về cấu hình bảng cho chế độ thống kê
    m_table->setColumns({"Mã HĐ", "Ngày lập", "Doanh thu", "Người lập", "Ghi chú"}, {500.0f, 500.0f, 520.0f, 420.0f, 792.0f});

    // 1. Tính tổng
    long long total = ThongKe::calculateTotal(valid);
    setSuccess("Tổng: " + formatCurrency(total));

    // 2. Đổ vào bảng
    m_table->clear();
    for (const auto& item : valid) {
        m_table->addRow({ item.maHD, item.ngay, formatCurrency(item.doanhThu), item.maNV, "" });
    }

    // Dòng tổng doanh thu
    m_table->addRow({ "Tổng", "", formatCurrency(total), "", "" });
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
    
    // Xử lý nút tìm kiếm
    if (m_btnSearch->isClicked()) {
        performSearch();
    }
    
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