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
    Item *Menu;
    int *soluong;
    
    int total = 0;
    long long price = 0;

public:
    Hoadon();
    ~Hoadon();

    // Nhóm hàm Set thông tin cơ bản (Dùng cho TextBox)
    void setThongTinChung(string mHD, string mNV, int d, int m, int y);

    // Nhóm hàm Logic (Dùng cho nút bấm Thêm/Xóa)
    void themVatPham(const Item &item, int sl_them);
    void xoaVatPham(const string &maItem);
    void tinhTongTien();
    // Giảm số lượng một mặt hàng trong hóa đơn. Nếu số lượng <= 0 thì xóa dòng.
    void giamVatPham(const string &maItem, int sl_giam);

    int getSoLuongMatHang() const; 

    // Lấy thông tin món ăn tại dòng thứ i
    Item getItemAt(int index) const;

    // Lấy số lượng của món ăn tại dòng thứ i
    int getSoLuongAt(int index) const;

    long long getTongTien() const;

    // Hàm lưu file (Dùng cho nút Lưu/Thanh toán)
    bool luuHoadonVaoFile();

    // Các hàm cũ (giữ lại để tương thích nếu cần)
    friend ostream& operator<<(ostream& out, const Hoadon &p);
    friend int Cashier(Hoadon &p, const string &maNV);
    friend string display(const Hoadon &p);
};

#endif
