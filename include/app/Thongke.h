#ifndef THONGKE_H
#define THONGKE_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

// Struct đơn giản để chứa thông tin một dòng thống kê
struct ThongKeItem {
    std::string maHD;
    std::string ngay; // Định dạng "dd/mm/yyyy"
    int doanhThu;
    std::string maNV; // Người lập
};

class ThongKe {
public:
    // --- CÁC HÀM TIỆN ÍCH NGÀY THÁNG ---
    static bool isNamNhuan(int y);
    static bool isValidDate(int d, int m, int y);
    static int getDaysInMonth(int m, int y);

    // --- CÁC HÀM THỐNG KÊ (Trả về danh sách hóa đơn) ---
    
    // 1. Thống kê theo Ngày
    static std::vector<ThongKeItem> getByDay(int d, int m, int y);

    // 2. Thống kê theo Tháng
    static std::vector<ThongKeItem> getByMonth(int m, int y);

    // 3. Thống kê theo Năm
    static std::vector<ThongKeItem> getByYear(int y);

    // 4. Thống kê theo Nhân viên
    static std::vector<ThongKeItem> getByStaff(const std::string& maNV);

    // --- HÀM TÍNH TỔNG (Tiện ích) ---
    static long long calculateTotal(const std::vector<ThongKeItem>& list);
};

#endif