#include <windows.h>
#include "app/Hoadon.h"
#include "app/Item.h"
#include "app/Nhanvien.h"
// #include "app/Thongke.h" // Bỏ bớt nếu chưa dùng
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <iomanip>
#include <filesystem>

using namespace std;

// --- CÁC HÀM TIỆN ÍCH GIỮ NGUYÊN ---
void TextColor5(int x) {
    HANDLE mau;
    mau = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(mau, x);
}

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

bool check_exist(const string &x) {
    ifstream in;
    in.open("data/Hoadon/" + x + ".txt");
    if (in) {
        in.close();
        return true;
    }
    return false;
}

// --- CONSTRUCTOR & DESTRUCTOR ---
Hoadon::Hoadon() {
    Menu = new Item[MAX];
    soluong = new int[MAX];
    total = 0; // Số lượng loại món ăn (Item count)
    price = 0; // Tổng tiền
}

Hoadon::~Hoadon() {
    if (Menu) delete[] Menu;
    if (soluong) delete[] soluong;
}

// --- NHÓM HÀM LOGIC QUAN TRỌNG CHO GIAO DIỆN ---

// 1. Hàm nhận thông tin từ các TextBox (Mã HĐ, Ngày,...)
void Hoadon::setThongTinChung(string mHD, string mNV, int d, int m, int y) {
    this->maHD = mHD;
    this->maNV = mNV;
    this->day = d;
    this->month = m;
    this->year = y;
}

// 2. Hàm xử lý khi nhấn vào một món ăn (hoặc nút Thêm)
void Hoadon::themVatPham(const Item &itemMoi, int sl_them) {
    // Kiểm tra xem món này đã có trong danh sách chưa
    int index = -1;
    for (int i = 0; i < total; i++) {
        if (Menu[i].getmaItem() == itemMoi.getmaItem()) {
            index = i;
            break;
        }
    }

    if (index != -1) {
        // Nếu đã có -> Cộng dồn số lượng
        soluong[index] += sl_them;
    } else {
        // Nếu chưa có -> Thêm dòng mới
        if (total < MAX) {
            Menu[total] = itemMoi; // Copy item vào mảng
            soluong[total] = sl_them;
            total++; // Tăng số đếm loại món
        } else {
            TextColor5(12);
            cout << "Hoa don da day, khong the them!" << endl;
            TextColor5(7);
        }
    }

    // Luôn tính lại tổng tiền sau khi thay đổi
    tinhTongTien();
}

// 3. Hàm xử lý khi nhấn nút Xóa một dòng
void Hoadon::xoaVatPham(const string &maItemCanXoa) {
    int index = -1;
    // Tìm vị trí món cần xóa
    for (int i = 0; i < total; i++) {
        if (Menu[i].getmaItem() == maItemCanXoa) {
            index = i;
            break;
        }
    }

    // Nếu tìm thấy -> Xóa và dồn mảng
    if (index != -1) {
        for (int i = index; i < total - 1; i++) {
            Menu[i] = Menu[i + 1];
            soluong[i] = soluong[i + 1];
        }
        total--; // Giảm số lượng loại món
        tinhTongTien();
    }
}

// 4. Hàm tính tổng tiền tự động
void Hoadon::tinhTongTien() {
    price = 0;
    for (int i = 0; i < total; i++) {
        price += (long long)Menu[i].getprice() * soluong[i];
    }
}

// 5. Hàm Lưu hóa đơn (Gọi khi nhấn nút Thanh toán/Lưu)
bool Hoadon::luuHoadonVaoFile() {
    // Kiểm tra trùng mã
    if (check_exist(maHD)) {
        return false; 
    }

    // Lưu chi tiết hóa đơn
    ofstream file("data/Hoadon/" + maHD + ".txt"); // Chú ý đường dẫn
    if (!file) return false;
    file << (*this); // Dùng lại operator<< đã viết
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

// --- GIỮ NGUYÊN OPERATOR<< ĐỂ IN RA FILE ---
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

// --- MÔ PHỎNG CÁCH GIAO DIỆN SỬ DỤNG CLASS NÀY (Hàm Cashier cũ) ---
int Cashier(Hoadon &p, const string &maNV) {
    // 1. Giả lập việc nhập từ TextBox ngày tháng, mã HĐ
    string tempMaHD;
    int d, m, y;
    
    cout << "--- MO PHONG GIAO DIEN ---" << endl;
    cout << "Nhap Ngay (d m y): "; cin >> d >> m >> y;
    cout << "Nhap Ma HD: "; cin >> tempMaHD;

    // Gọi hàm set thông tin (Logic UI -> Class)
    p.setThongTinChung(tempMaHD, maNV, d, m, y);

    bool running = true;
    while (running) {
        system("cls");
        // Hiển thị hóa đơn hiện tại (Giống như vẽ GridView/ListBox)
        cout << "=== HOA DON HIEN TAI (Update Realtime) ===\n";
        cout << p; 
        cout << "\n==========================================\n";

        cout << "1. Chon mon (Gia lap click vao hinh mon an)\n";
        cout << "2. Xoa mon (Gia lap click nut Xoa)\n";
        cout << "3. THANH TOAN & LUU\n";
        cout << "Lua chon cua ban: ";
        
        int choice; cin >> choice;

        if (choice == 1) {
            // Giả lập lấy item từ Database/List món ăn
            Item itemTest; 
            // Ở GUI thật, bạn sẽ lấy Item từ đối tượng nút bấm
            // Ở đây mình nhập tạm để test logic
            string id, name; int pr;
            cout << "Nhap ID mon: "; cin >> id;
            cout << "Nhap Ten mon: "; cin.ignore(); getline(cin, name);
            cout << "Nhap Gia: "; cin >> pr;
            
            // Item(ma, ten, gia, ...) -> Tùy constructor của class Item
            // Giả sử Item có method set hoặc constructor tương ứng
            // itemTest.setAll(id, name, pr...); <--- Bạn tự điền theo class Item của bạn
            
            // GỌI HÀM LOGIC:
            p.themVatPham(itemTest, 1); // Thêm 1 món

        } else if (choice == 2) {
            string idXoa;
            cout << "Nhap ID mon can xoa: "; cin >> idXoa;
            
            // GỌI HÀM LOGIC:
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

// Trả về số lượng loại món (biến total trong private)
int Hoadon::getSoLuongMatHang() const {
    return total;
}

// Trả về đối tượng Item tại vị trí index
Item Hoadon::getItemAt(int index) const {
    if (index >= 0 && index < total) {
        return Menu[index];
    }
    // Trả về item rỗng nếu index sai (đề phòng lỗi)
    return Item(); 
}

// Trả về số lượng tại vị trí index
int Hoadon::getSoLuongAt(int index) const {
    if (index >= 0 && index < total) {
        return soluong[index];
    }
    return 0;
}

// Trả về tổng tiền
long long Hoadon::getTongTien() const {
    return price;
}

// Giảm số lượng vật phẩm (nếu tồn tại). Nếu số lượng sau giảm <= 0 thì xóa dòng hoàn toàn.
void Hoadon::giamVatPham(const string &maItem, int sl_giam) {
    int index = -1;
    for (int i = 0; i < total; i++) {
        if (Menu[i].getmaItem() == maItem) {
            index = i;
            break;
        }
    }

    if (index == -1) return; // không có trong hóa đơn

    // giảm số lượng
    soluong[index] -= sl_giam;
    if (soluong[index] <= 0) {
        // xóa dòng và dồn mảng
        for (int i = index; i < total - 1; i++) {
            Menu[i] = Menu[i + 1];
            soluong[i] = soluong[i + 1];
        }
        total--;
    }

    // luôn cập nhật lại tổng tiền
    tinhTongTien();
}