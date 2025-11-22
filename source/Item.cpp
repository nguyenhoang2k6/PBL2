#include "app/Item.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cstdio> // Cho remove và rename
using namespace std;

// Khai báo sớm (Forward declaration) cho hàm loadAllItems vì nó được dùng trong check_exist
bool loadAllItems(vector<Item> &items);

/**
 * @brief Overload operator << để xuất thông tin Item (Đã giả định là friend trong Item.h)
 * @param out Output stream
 * @param p Item cần xuất
 * @return Output stream
 */
std::ostream& operator<<(std::ostream &out, const Item &p) {
    // Truy cập trực tiếp được chấp nhận nếu là friend function
    out << p.maItem << endl; 
    out << p.tenItem << endl; 
    out << p.price << endl; 
    return out;
}

/**
 * @brief Kiểm tra item đã tồn tại chưa
 * @param x Item cần kiểm tra
 * @return true nếu tồn tại, false nếu không
 */
bool check_exist(const Item &x) {
    vector<Item> items;
    if (!loadAllItems(items)) {
        return false; // Không thể đọc file
    }
    
    // Duyệt qua tất cả items để kiểm tra trùng mã, sử dụng getter
    for (const auto &item : items) {
        if (x.getmaItem() == item.getmaItem()) { // Đã sửa: dùng getmaItem()
            return true; // Mã đã tồn tại
        }
    }
    return false; // Mã chưa tồn tại
}

/**
 * @brief Overload operator >> để nhập thông tin Item (Đã giả định là friend trong Item.h)
 * @param in Input stream 
 * @param p Item cần nhập
 * @return Input stream
 */
std::istream& operator>>(std::istream &in, Item &p) {
    string tempMa;
    cout << "Nhap ma mat hang: ";
    getline(in, tempMa); 
    p.setmaItem(tempMa); // Đã sửa: dùng setter để gán

    // Kiểm tra và yêu cầu nhập lại nếu mã đã tồn tại
    while(check_exist(p)) {
        cout << "Ma mat hang da duoc su dung, nhap lai: ";
        getline(in, tempMa);
        p.setmaItem(tempMa); // Đã sửa: dùng setter để gán
    }
    
    string tempTen;
    cout << "Nhap ten mat hang: ";
    getline(in, tempTen); 
    p.setTenItem(tempTen); // Đã sửa: dùng setter để gán
    
    int tempPrice;
    cout << "Nhap gia: ";
    in >> tempPrice; 
    p.setPrice(tempPrice); // Đã sửa: dùng setter để gán

    in.ignore(); // Xóa bộ đệm sau khi nhập số
    return in;
}

// Các hàm getter/setter (Định nghĩa lại để đảm bảo đúng cú pháp Item::)

void Item::setmaItem(string maIteam) {
    this->maItem = maIteam;
}

string Item::getmaItem() const { // Thêm const cho getter nếu maItem không đổi
    return maItem;
}

void Item::setTenItem(string tenIteam) {
    this->tenItem = tenIteam;
}

string Item::getTenItem() const { // Thêm const cho getter
    return tenItem;
}

void Item::setPrice(int price) {
    this->price = price;
}

int Item::getprice() const { // Thêm const cho getter
    return price;
}

/**
 * @brief Đọc thông tin items từ file vào mảng
 * @param p Mảng items đầu ra
 * @return Số lượng items đọc được, -1 nếu lỗi
 */
int getInfo(Item* p) {
    ifstream myFile("Item/Item.txt");
    if (!myFile.is_open()) {
        return -1; // Lỗi mở file
    }
    
    int total = 0;
    string ma, ten, line;
    int price;

    // Đọc từng dòng: mã, tên, giá và sử dụng setters để gán giá trị
    while (getline(myFile, ma) && 
           getline(myFile, ten) && 
           getline(myFile, line)) {
        stringstream geek(line);
        geek >> price; // Chuyển string sang int
        
        // Sử dụng Setters
        p[total].setmaItem(ma);
        p[total].setTenItem(ten);
        p[total].setPrice(price);

        total++;
        if (total >= 1000) break; // Bảo vệ khỏi tràn mảng
    }
    myFile.close();
    return total;
}

/**
 * @brief Đọc items vào vector (an toàn hơn)
 * @param items Vector kết quả
 * @return true nếu thành công, false nếu thất bại
 */
bool loadAllItems(vector<Item> &items) {
    ifstream myFile("Item/Item.txt");
    if (!myFile.is_open()) {
        return false; // Lỗi mở file
    }
    
    items.clear(); // Xóa vector cũ
    string ma, ten, line;
    int price;
    
    // Đọc từng bộ 3 dòng: mã, tên, giá
    while (getline(myFile, ma) && 
           getline(myFile, ten) && 
           getline(myFile, line)) {
        Item temp;
        stringstream geek(line);
        geek >> price;
        
        // Sử dụng Setters
        temp.setmaItem(ma);
        temp.setTenItem(ten);
        temp.setPrice(price);

        items.push_back(temp); // Thêm vào vector
    }
    myFile.close();
    return true;
}

/**
 * @brief Hiển thị item theo mã
 * @param p Mảng items
 * @param x Mã cần tìm
 * @return true nếu tìm thấy, false nếu không
 */
bool displayItemByCode(Item* p, string x) {
    int total = getInfo(p);
    if (total <= 0) return false; // Không có items hoặc lỗi
    
    for (int i = 0; i < total; i++) {
        if (x == p[i].getmaItem()) { // Đã sửa: dùng getmaItem()
            // Hiển thị thông tin item, dùng getters
            cout << "MaMH: " << p[i].getmaItem() << endl;
            cout << "Ten mon: " << p[i].getTenItem() << endl;
            cout << "Gia: " << p[i].getprice() << endl;
            return true; // Đã tìm thấy và hiển thị
        }
    }
    return false; // Không tìm thấy
}

/**
 * @brief Hiển thị tất cả items
 * @param p Mảng items
 * @return true nếu thành công, false nếu thất bại
 */
bool displayAllItems(Item* p) {
    int total = getInfo(p);
    if (total <= 0) return false; // Không có items hoặc lỗi
    
    cout << "DANH SACH TAT CA MAT HANG:" << endl;
    cout << "--------------------------" << endl;
    for (int i = 0; i < total; i++) {
        // Hiển thị từng item, dùng getters
        cout << "STT " << (i+1) << ": " 
             << p[i].getmaItem() << " | " // Đã sửa: dùng getmaItem()
             << p[i].getTenItem() << " | " // Đã sửa: dùng getTenItem()
             << p[i].getprice() << " VND" << endl; // Đã sửa: dùng getprice()
    }
    cout << "--------------------------" << endl;
    cout << "Tong cong: " << total << " mat hang" << endl;
    return true;
}

/**
 * @brief Thêm item mới
 * @param p Mảng items
 * @return true nếu thành công, false nếu thất bại
 */
bool addItem(Item* p) {
    Item x;
    cin >> x; // Nhập thông tin item mới
    
    ofstream file2("Item/Item.txt", ios::app);
    if (!file2.is_open()) {
        return false; // Lỗi mở file
    }
    
    file2 << x; // Ghi item mới vào file (sử dụng operator<< đã được friend)
    file2.close();
    return true;
}

/**
 * @brief Kiểm tra mã item có tồn tại không
 * @param p Mảng items
 * @param ma Mã cần kiểm tra
 * @return true nếu tồn tại, false nếu không
 */
bool check_maItem(Item *p, string ma) {
    int total = getInfo(p);
    if (total <= 0) return false; // Không có items
    
    for (int i = 0; i < total; i++) {
        if (ma == p[i].getmaItem()) { // Đã sửa: dùng getmaItem()
            return true; // Tìm thấy mã
        }
    }
    return false; // Không tìm thấy mã
}

/**
 * @brief Xóa item khỏi hệ thống
 * @param p Mảng items
 * @return true nếu thành công, false nếu thất bại
 */
bool deleteItem(Item *p) {
    string x;
    cout << "Nhap ma mat hang muon xoa: ";
    cin >> x;
    cin.ignore(); // Xóa bộ đệm
    
    // Kiểm tra mã có tồn tại không
    if (!check_maItem(p, x)) {
        cout << "Ma mat hang khong ton tai!" << endl;
        return false;
    }
    
    // Mở file gốc để đọc
    ifstream is("Item/Item.txt");
    if (!is.is_open()) {
        cout << "Khong the mo file!" << endl;
        return false;
    }
    
    // Tạo file tạm để ghi
    ofstream file2("temp.txt");
    if (!file2.is_open()) {
        is.close();
        cout << "Khong the tao file tam!" << endl;
        return false;
    }
    
    int n = getInfo(p);
    bool found = false;
    
    // Ghi tất cả items trừ item cần xóa
    for (int j = 0; j < n; j++) {
        if (p[j].getmaItem() != x) { // Đã sửa: dùng getmaItem()
            file2 << p[j]; // Ghi item không bị xóa
        } else {
            found = true; // Đánh dấu đã tìm thấy item cần xóa
        }
    }
    
    file2.close();
    is.close();
    
    if (found) {
        // Thay thế file cũ bằng file tạm
        remove("Item/Item.txt");
        rename("temp.txt", "Item/Item.txt");
        cout << "Xoa mat hang thanh cong!" << endl;
        return true;
    } else {
        remove("temp.txt"); // Xóa file tạm nếu không tìm thấy
        cout << "Khong tim thay mat hang de xoa!" << endl;
        return false;
    }
}

/**
 * @brief Cập nhật thông tin item
 * @param p Mảng items
 * @param ma Mã item cần cập nhật
 * @param newItem Thông tin mới
 * @return true nếu thành công, false nếu thất bại
 */
bool updateItem(Item *p, string ma, const Item &newItem) {
    // Kiểm tra mã có tồn tại không
    if (!check_maItem(p, ma)) {
        cout << "Ma mat hang khong ton tai!" << endl;
        return false;
    }
    
    // Mở file gốc để đọc
    ifstream is("Item/Item.txt");
    if (!is.is_open()) {
        cout << "Khong the mo file!" << endl;
        return false;
    }
    
    // Tạo file tạm để ghi
    ofstream file2("temp.txt");
    if (!file2.is_open()) {
        is.close();
        cout << "Khong the tao file tam!" << endl;
        return false;
    }
    
    int n = getInfo(p);
    bool updated = false;
    
    // Ghi tất cả items, thay thế item cần cập nhật
    for (int j = 0; j < n; j++) {
        if (p[j].getmaItem() == ma) { // Đã sửa: dùng getmaItem()
            file2 << newItem; // Ghi thông tin mới
            updated = true;
        } else {
            file2 << p[j]; // Giữ nguyên item khác
        }
    }
    
    file2.close();
    is.close();
    
    if (updated) {
        // Thay thế file cũ bằng file tạm
        remove("Item/Item.txt");
        rename("temp.txt", "Item/Item.txt");
        cout << "Cap nhat mat hang thanh cong!" << endl;
        return true;
    } else {
        remove("temp.txt"); // Xóa file tạm nếu không cập nhật
        cout << "Khong the cap nhat mat hang!" << endl;
        return false;
    }
}

/**
 * @brief Tìm kiếm items theo tên
 * @param p Mảng items
 * @param name Tên cần tìm
 * @param results Mảng kết quả
 * @return Số lượng items tìm thấy
 */
int searchItemsByName(Item* p, string name, Item* results) {
    int total = getInfo(p);
    int count = 0;
    
    // Duyệt và tìm kiếm theo tên
    for (int i = 0; i < total && count < 1000; i++) {
        // Đã sửa: dùng getTenItem()
        if (p[i].getTenItem().find(name) != string::npos) { 
            results[count++] = p[i]; // Thêm vào kết quả
        }
    }
    return count; // Trả về số lượng tìm thấy
}

/**
 * @brief Đếm tổng số items
 * @return Số lượng items, -1 nếu lỗi
 */
int countItems() {
    vector<Item> items;
    if (loadAllItems(items)) {
        return items.size(); // Trả về kích thước vector
    }
    return -1; // Lỗi đọc file
}