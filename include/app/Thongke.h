#ifndef THONGKE_H
#define THONGKE_H

// Các hàm kiểm tra ngày tháng (đã trả về int)
int KiemTraNgay(int d,int m,int y);
int KiemTraThang(int m,int y);
int KiemTraNam(int y,int m);

// Hàm tính số ngày trong tháng:
// Đổi từ void sang bool để báo cáo nếu tháng/năm hợp lệ (true/false)
bool SoNgayTrongThang(int y,int m,int a[13]);

// Các hàm thống kê:
// Đổi từ void sang bool để báo cáo trạng thái thành công của việc thống kê
bool statsMonth();
bool statsDay();
bool statsYear();
bool statsNhanvien();

#endif