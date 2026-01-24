#include <app/Thongke.h>
#include <iomanip>
#include <array>

// Kiểm tra năm nhuận
bool ThongKe::isNamNhuan(int y) {
    return (y % 400 == 0 || (y % 4 == 0 && y % 100 != 0));
}

// Lấy số ngày trong tháng
int ThongKe::getDaysInMonth(int m, int y) {
    int days[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (isNamNhuan(y)) days[2] = 29;
    return days[m];
}

// Kiểm tra ngày/tháng/năm hợp lệ
bool ThongKe::isValidDate(int d, int m, int y) {
    if (y < 0 || m < 1 || m > 12) return false;
    if (d < 1 || d > getDaysInMonth(m, y)) return false;
    return true;
}

// Mở stream log tại các đường dẫn khả dụng
static std::ifstream openLogStream() {
    const std::array<const char*, 4> paths = {
        "data/history/log.txt",
        "data/History/log.txt",
        "History/log.txt",
        "history/log.txt"
    };
    for (auto p : paths) {
        std::ifstream f(p);
        if (f.is_open()) return f;
    }
    return std::ifstream();
}

// Đọc toàn bộ log thành danh sách ThongKeItem
static std::vector<ThongKeItem> readAllLogs() {
    std::vector<ThongKeItem> list;
    std::ifstream file = openLogStream();
    if (!file.is_open()) return list;

    int d, m, y, price;
    std::string maNV, maHD;

    while (file >> d >> m >> y >> maNV >> maHD >> price) {
        ThongKeItem item;
        item.maHD = maHD;
        item.maNV = maNV;
        item.doanhThu = price;
        
        std::stringstream ss;
        ss << std::setfill('0') << std::setw(2) << d << "/"
           << std::setw(2) << m << "/" << y;
        item.ngay = ss.str();

        list.push_back(item);
    }
    
    file.close();
    return list;
}

// Lấy thống kê theo ngày
std::vector<ThongKeItem> ThongKe::getByDay(int d, int m, int y) {
    std::vector<ThongKeItem> result;
    if (!isValidDate(d, m, y)) return result;

    std::ifstream file = openLogStream();
    if (!file.is_open()) return result;

    int day, month, year, price;
    std::string maNV, maHD;

    while (file >> day >> month >> year >> maNV >> maHD >> price) {
        if (day == d && month == m && year == y) {
            ThongKeItem item;
            item.maHD = maHD;
            item.maNV = maNV;
            item.doanhThu = price;
            item.ngay = std::to_string(d) + "/" + std::to_string(m) + "/" + std::to_string(y);
            result.push_back(item);
        }
    }
    file.close();
    return result;
}

// Lấy thống kê theo tháng
std::vector<ThongKeItem> ThongKe::getByMonth(int m, int y) {
    std::vector<ThongKeItem> result;
    if (m < 1 || m > 12 || y < 0) return result;

    std::ifstream file = openLogStream();
    if (!file.is_open()) return result;

    int day, month, year, price;
    std::string maNV, maHD;

    while (file >> day >> month >> year >> maNV >> maHD >> price) {
        if (month == m && year == y) {
            ThongKeItem item;
            item.maHD = maHD;
            item.maNV = maNV;
            item.doanhThu = price;
            item.ngay = std::to_string(day) + "/" + std::to_string(m) + "/" + std::to_string(y);
            result.push_back(item);
        }
    }
    file.close();
    return result;
}

// Lấy thống kê theo năm
std::vector<ThongKeItem> ThongKe::getByYear(int y) {
    std::vector<ThongKeItem> result;
    if (y < 0) return result;

    std::ifstream file = openLogStream();
    if (!file.is_open()) return result;

    int day, month, year, price;
    std::string maNV, maHD;

    while (file >> day >> month >> year >> maNV >> maHD >> price) {
        if (year == y) {
            ThongKeItem item;
            item.maHD = maHD;
            item.maNV = maNV;
            item.doanhThu = price;
            item.ngay = std::to_string(day) + "/" + std::to_string(month) + "/" + std::to_string(y);
            result.push_back(item);
        }
    }
    file.close();
    return result;
}

// Lấy thống kê theo mã nhân viên
std::vector<ThongKeItem> ThongKe::getByStaff(const std::string& targetMaNV) {
    std::vector<ThongKeItem> result;
    
    std::ifstream file = openLogStream();
    if (!file.is_open()) return result;

    int day, month, year, price;
    std::string maNV, maHD;

    while (file >> day >> month >> year >> maNV >> maHD >> price) {
        if (maNV == targetMaNV) {
            ThongKeItem item;
            item.maHD = maHD;
            item.maNV = maNV;
            item.doanhThu = price;
            item.ngay = std::to_string(day) + "/" + std::to_string(month) + "/" + std::to_string(year);
            result.push_back(item);
        }
    }
    file.close();
    return result;
}

// Tính tổng doanh thu từ danh sách
long long ThongKe::calculateTotal(const std::vector<ThongKeItem>& list) {
    long long total = 0;
    for (const auto& item : list) {
        total += item.doanhThu;
    }
    return total;
}
