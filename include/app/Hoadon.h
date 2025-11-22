#ifndef HOADON_H
#define HOADON_H
#include "Item.h"
#include "Nhanvien.h"
#define MAX 1000
using namespace std;

class Item;

class Hoadon {
private:
    int day, month, year;
    string maHD;
    string maNV;
    Item *Menu;
    int *soluong;
    int price = 0;
    int total = 0;

public:
    Hoadon();
    ~Hoadon();

    // Trả về tổng tiền
    friend int Cashier(Hoadon &p, const string &maNV);

    // Trả về nội dung hóa đơn dạng string
    friend string display(const Hoadon &p);

    // Toán tử xuất không đổi
    friend ostream& operator<<(ostream& out, const Hoadon &p);
};

#endif
