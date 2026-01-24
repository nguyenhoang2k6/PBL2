#include <app/Admin_ThongKe.h>
#include <app/Button.h>
#include <app/Label.h>
#include <app/TextBox.h>
#include <app/Color.h>
#include <app/Nhanvien.h>
#include <filesystem>
#include <algorithm>
#include <cctype>

// Khởi tạo màn hình thống kê
Admin_ThongKe::Admin_ThongKe(App* app) : Screen(app) {
    m_lblTitle = nullptr; m_lblTotalRevenue = nullptr;
    m_txtDay = nullptr; m_txtMonth = nullptr; m_txtYear = nullptr; m_txtMaNV = nullptr;
    m_btnByDay = nullptr; m_btnByMonth = nullptr; m_btnByYear = nullptr; m_btnByStaff = nullptr;
    m_btnSearch = nullptr; m_btnBack = nullptr;
    m_table = nullptr;
    
    m_currentMode = VIEW_DAY;
}

// Giải phóng tài nguyên màn hình thống kê
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

// Khởi tạo UI và bảng thống kê
bool Admin_ThongKe::Init() {
    TTF_Font* font = app->getFont1();
    TTF_Font* fontTitle = app->getFont2();
    if (!fontTitle) fontTitle = font;
    
    SDL_Renderer* renderer = app->getRenderer();
    
    m_lblTitle = new Label("THỐNG KÊ DOANH THU", COLOR_WARNING, 1000, 100, fontTitle, renderer);

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

    m_btnSearch = new Button(1300, 305, 200, 60,COLOR_UI_GREEN, "Xem", renderer, font, COLOR_WHITE);
    m_btnBack   = new Button(2482, 0, 250, 75, COLOR_UI_RED, "Quay lại", renderer, font, COLOR_WHITE);

    m_lblTotalRevenue = new Label("Tổng: 0 VND",COLOR_GREEN, 50, 395, font, renderer);

    std::vector<std::string> headers = {"Mã HĐ", "Ngày lập", "Doanh thu", "Người lập", "Ghi chú"};
    std::vector<float> widths = {500.0f, 500.0f, 520.0f, 420.0f, 792.0f};

    m_tableWidth = 2732;
    m_tableHeight = 1086;
    m_table = new Table(0, 450, m_tableWidth, m_tableHeight, font, renderer, headers, widths);

    resetButtonColors();

    return true;
}

// Đồng bộ màu nút theo chế độ xem
void Admin_ThongKe::resetButtonColors() {
    SDL_Color normalColor = {220, 220, 220, 255};
    SDL_Color activeColor = {100, 200, 255, 255};

    m_btnByDay->setColor(m_currentMode == VIEW_DAY ? activeColor : normalColor);
    m_btnByMonth->setColor(m_currentMode == VIEW_MONTH ? activeColor : normalColor);
    m_btnByYear->setColor(m_currentMode == VIEW_YEAR ? activeColor : normalColor);
    m_btnByStaff->setColor(m_currentMode == VIEW_STAFF ? activeColor : normalColor);
}

// Reset trạng thái khi vào màn hình
void Admin_ThongKe::onEnter() {
    m_txtDay->setText("");
    m_txtMonth->setText("");
    m_txtYear->setText("");
    m_txtMaNV->setText("");
    m_lblTotalRevenue->setColor(COLOR_GREEN);
    m_lblTotalRevenue->setText("Tổng: 0 VND");
    m_table->clear();
    
    m_currentMode = VIEW_DAY;
    resetButtonColors();
}

// Xử lý khi rời màn hình
void Admin_ThongKe::onExit() {
    SDL_StopTextInput(app->getWindow());
}

// Cập nhật UI mỗi frame
void Admin_ThongKe::update() {
    m_btnByDay->update();
    m_btnByMonth->update();
    m_btnByYear->update();
    m_btnByStaff->update();
    m_btnSearch->update();
    m_btnBack->update();

    if (m_btnByDay->isClicked()) { m_currentMode = VIEW_DAY; resetButtonColors(); }
    if (m_btnByMonth->isClicked()) { m_currentMode = VIEW_MONTH; resetButtonColors(); }
    if (m_btnByYear->isClicked()) { m_currentMode = VIEW_YEAR; resetButtonColors(); }
    if (m_btnByStaff->isClicked()) { m_currentMode = VIEW_STAFF; resetButtonColors(); }

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

// Thực hiện tìm kiếm và tính tổng doanh thu
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

    std::vector<ThongKeItem> valid;
    for (const auto& item : results) {
        std::filesystem::path p = std::filesystem::path("data/Hoadon") / (item.maHD + ".txt");
        if (std::filesystem::exists(p)) {
            valid.push_back(item);
        }
    }

    if (valid.empty()) {
        setError("Khong tim thay hoa don hop le");
        return;
    }

    m_table->setColumns({"Mã HĐ", "Ngày lập", "Doanh thu", "Người lập", "Ghi chú"}, {500.0f, 500.0f, 520.0f, 420.0f, 792.0f});

    long long total = ThongKe::calculateTotal(valid);
    setSuccess("Tổng: " + formatCurrency(total));

    m_table->clear();
    for (const auto& item : valid) {
        m_table->addRow({ item.maHD, item.ngay, formatCurrency(item.doanhThu), item.maNV, "" });
    }

    m_table->addRow({ "Tổng", "", formatCurrency(total), "", "" });
}

// Xử lý sự kiện người dùng
void Admin_ThongKe::handleEvent(const SDL_Event& e) {
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
    
    if (m_btnSearch->isClicked()) {
        performSearch();
    }
    
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

    m_table->handleEvent(e);
    if(m_btnBack->isClicked()) {
        app->changeScreen("AdminDashBoard");
    }
}

// Vẽ giao diện thống kê
void Admin_ThongKe::render(SDL_Renderer* renderer) {
    m_btnByDay->render(renderer);
    m_btnByMonth->render(renderer);
    m_btnByYear->render(renderer);
    m_btnByStaff->render(renderer);

    switch (m_currentMode) {
        case VIEW_DAY:
            m_txtDay->render(renderer);
            m_txtMonth->render(renderer);
            m_txtYear->render(renderer);
            break;
        case VIEW_MONTH:
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

    m_lblTitle->render(renderer);
    m_btnSearch->render(renderer);
    m_btnBack->render(renderer);
    m_lblTotalRevenue->render(renderer);
    
    m_table->render();
}
