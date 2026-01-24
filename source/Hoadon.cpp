#include <windows.h>
#include "app/Hoadon.h"
#include "app/Item.h"
#include "app/Nhanvien.h"
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <iomanip>
#include <filesystem>

using namespace std;

// Đổi màu chữ console (Windows)
void TextColor5(int x) {
    HANDLE mau;
    mau = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(mau, x);
}

// Đọc nội dung file và in ra console
void read(const string &x) {
    ifstream in;
    in.open(x);
    if (!in) return;
    string line;
    while (getline(in, line)) {
        cout << line << endl;
    }
    in.close();
}

// Kiểm tra file hóa đơn đã tồn tại
bool check_exist(const string &x) {
    ifstream in;
    in.open("data/Hoadon/" + x + ".txt");
    if (in) {
        in.close();
        return true;
    }
    return false;
}

// Khởi tạo hóa đơn
Hoadon::Hoadon() {
    Menu = new Item[MAX];
    soluong = new int[MAX];
    total = 0;
    price = 0;
}

// Giải phóng bộ nhớ động
Hoadon::~Hoadon() {
    if (Menu) delete[] Menu;
    if (soluong) delete[] soluong;
}

// Thiết lập thông tin chung hóa đơn
void Hoadon::setThongTinChung(string mHD, string mNV, int d, int m, int y) {
    this->maHD = mHD;
    this->maNV = mNV;
    this->day = d;
    this->month = m;
    this->year = y;
}

// Thêm vật phẩm vào hóa đơn
void Hoadon::themVatPham(const Item &itemMoi, int sl_them) {
    int index = -1;
    for (int i = 0; i < total; i++) {
        if (Menu[i].getmaItem() == itemMoi.getmaItem()) {
            index = i;
            break;
        }
    }

    if (index != -1) {
        soluong[index] += sl_them;
    } else {
        if (total < MAX) {
            Menu[total] = itemMoi;
            soluong[total] = sl_them;
            total++;
        } else {
            TextColor5(12);
            cout << "Hoa don da day, khong the them!" << endl;
            TextColor5(7);
        }
    }

    tinhTongTien();
}

// Xóa vật phẩm theo mã
void Hoadon::xoaVatPham(const string &maItemCanXoa) {
    int index = -1;
    for (int i = 0; i < total; i++) {
        if (Menu[i].getmaItem() == maItemCanXoa) {
            index = i;
            break;
        }
    }

    if (index != -1) {
        for (int i = index; i < total - 1; i++) {
            Menu[i] = Menu[i + 1];
            soluong[i] = soluong[i + 1];
        }
        total--;
        tinhTongTien();
    }
}

// Tính tổng tiền của hóa đơn
void Hoadon::tinhTongTien() {
    price = 0;
    for (int i = 0; i < total; i++) {
        price += (long long)Menu[i].getprice() * soluong[i];
    }
}

// Lưu hóa đơn vào file và ghi log
bool Hoadon::luuHoadonVaoFile() {
    if (check_exist(maHD)) {
        return false; 
    }

    ofstream file("data/Hoadon/" + maHD + ".txt");
    if (!file) return false;
    file << (*this);
    file.close();

    // Lưu lịch sử (Log)
    std::filesystem::create_directories("data/history");
    ofstream log("data/history/log.txt", ios::app);
    if (log) {
        log << day << " " << month << " " << year << " "
            << maNV << " " << maHD << " " << price << endl;
        log.close();
    }
    return true;
}

// Xuất hóa đơn ra luồng
ostream& operator<<(ostream& out, const Hoadon &p) {
    out << "Ngay thu ngan: " << p.day << "/" << p.month << "/" << p.year << endl;
    out << "Nhan vien thu ngan: " << p.maNV << endl;
    out << setw(30) << left << "Ten mon";
    out << setw(10) << left << "So luong";
    out << setw(10) << left << "Don gia";
    out << setw(10) << left << "Thanh tien" << endl;
    out << "---------------------------------------------------------------" << endl;
    for (int i = 0; i < p.total; i++) {
        out << setw(30) << left << p.Menu[i].getTenItem();
        out << setw(10) << left << p.soluong[i];
        out << setw(10) << left << p.Menu[i].getprice();
        out << setw(10) << left << (long long)p.soluong[i] * p.Menu[i].getprice() << endl;
    }
    out << "---------------------------------------------------------------" << endl;
    out << "Tong gia tien: " << p.price << endl;
    return out;
}

// Mô phỏng giao diện thu ngân (console)
int Cashier(Hoadon &p, const string &maNV) {
    string tempMaHD;
    int d, m, y;
    
    cout << "--- MO PHONG GIAO DIEN ---" << endl;
    cout << "Nhap Ngay (d m y): "; cin >> d >> m >> y;
    cout << "Nhap Ma HD: "; cin >> tempMaHD;

    p.setThongTinChung(tempMaHD, maNV, d, m, y);

    bool running = true;
    while (running) {
        system("cls");
        cout << "=== HOA DON HIEN TAI (Update Realtime) ===\n";
        cout << p; 
        cout << "\n==========================================\n";

        cout << "1. Chon mon (Gia lap click vao hinh mon an)\n";
        cout << "2. Xoa mon (Gia lap click nut Xoa)\n";
        cout << "3. THANH TOAN & LUU\n";
        cout << "Lua chon cua ban: ";
        
        int choice; cin >> choice;

        if (choice == 1) {
            Item itemTest; 
            string id, name; int pr;
            cout << "Nhap ID mon: "; cin >> id;
            cout << "Nhap Ten mon: "; cin.ignore(); getline(cin, name);
            cout << "Nhap Gia: "; cin >> pr;
            
            p.themVatPham(itemTest, 1);

        } else if (choice == 2) {
            string idXoa;
            cout << "Nhap ID mon can xoa: "; cin >> idXoa;
            
            p.xoaVatPham(idXoa);
            
        } else if (choice == 3) {
            if (p.luuHoadonVaoFile()) {
                cout << "Luu thanh cong!\n";
                running = false;
            } else {
                cout << "Loi: Ma hoa don da ton tai hoac loi file.\n";
                system("pause");
            }
        }
    }
    return p.price;
}

// Lấy số lượng mặt hàng trong hóa đơn
int Hoadon::getSoLuongMatHang() const {
    return total;
}

// Lấy Item ở vị trí index
Item Hoadon::getItemAt(int index) const {
    if (index >= 0 && index < total) {
        return Menu[index];
    }
    return Item(); 
}

// Lấy số lượng của mặt hàng ở vị trí index
int Hoadon::getSoLuongAt(int index) const {
    if (index >= 0 && index < total) {
        return soluong[index];
    }
    return 0;
}

long long Hoadon::getTongTien() const {
    return price;
}

void Hoadon::giamVatPham(const string &maItem, int sl_giam) {
    int index = -1;
    for (int i = 0; i < total; i++) {
        if (Menu[i].getmaItem() == maItem) {
            index = i;
            break;
        }
    }

    if (index == -1) return;

    soluong[index] -= sl_giam;
    if (soluong[index] <= 0) {
        for (int i = index; i < total - 1; i++) {
            Menu[i] = Menu[i + 1];
            soluong[i] = soluong[i + 1];
        }
        total--;
    }

    tinhTongTien();
}
