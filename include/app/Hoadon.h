#ifndef HOADON_H
#define HOADON_H
#include "app/Item.h"
#include <string>
#include <iostream>
#include <fstream>
#define MAX 1000

using namespace std;

class Hoadon {
private:
    int day, month, year;
    string maHD;
    string maNV;
    Item *Menu;      // Mảng chứa các món
    int *soluong;    // Mảng chứa số lượng tương ứng
    
    int total = 0;   // BIẾN ĐẾM: Số dòng sản phẩm hiện có trong hóa đơn (Item Count)
    long long price = 0; // TỔNG TIỀN: Tổng giá trị hóa đơn

public:
    Hoadon();
    ~Hoadon();

    // Nhóm hàm Set thông tin cơ bản (Dùng cho TextBox)
    void setThongTinChung(string mHD, string mNV, int d, int m, int y);

    // Nhóm hàm Logic (Dùng cho nút bấm Thêm/Xóa)
    void themVatPham(const Item &item, int sl_them); // Logic nút Thêm
    void xoaVatPham(const string &maItem);           // Logic nút Xóa
    void tinhTongTien();                             // Hàm nội bộ tính lại tiền

    int getSoLuongMatHang() const; 

    // Lấy thông tin món ăn tại dòng thứ i
    Item getItemAt(int index) const;

    // Lấy số lượng của món ăn tại dòng thứ i
    int getSoLuongAt(int index) const;

    // Lấy tổng tiền hiện tại của hóa đơn
    long long getTongTien() const;

    // Hàm lưu file (Dùng cho nút Lưu/Thanh toán)
    bool luuHoadonVaoFile();

    // Các hàm cũ (giữ lại để tương thích nếu cần)
    friend ostream& operator<<(ostream& out, const Hoadon &p);
    friend int Cashier(Hoadon &p, const string &maNV); // Hàm này giờ chỉ để test console
    friend string display(const Hoadon &p);
};

#endif