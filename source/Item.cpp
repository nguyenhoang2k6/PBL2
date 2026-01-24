#include "app/Item.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cstdio>
using namespace std;

bool loadAllItems(vector<Item> &items);

// Ghi thông tin Item ra stream
std::ostream& operator<<(std::ostream &out, const Item &p) {
    out << p.maItem << endl; 
    out << p.tenItem << endl; 
    out << p.price << endl; 
    return out;
}

// Kiểm tra mã Item đã tồn tại
bool check_exist(const Item &x) {
    vector<Item> items;
    if (!loadAllItems(items)) {
        return false;
    }
    
    for (const auto &item : items) {
        if (x.getmaItem() == item.getmaItem()) {
            return true;
        }
    }
    return false;
}

// Đọc thông tin Item từ stream (console)
std::istream& operator>>(std::istream &in, Item &p) {
    string tempMa;
    cout << "Nhap ma mat hang: ";
    getline(in, tempMa); 
    p.setmaItem(tempMa);

    while(check_exist(p)) {
        cout << "Ma mat hang da duoc su dung, nhap lai: ";
        getline(in, tempMa);
        p.setmaItem(tempMa);
    }
    
    string tempTen;
    cout << "Nhap ten mat hang: ";
    getline(in, tempTen); 
    p.setTenItem(tempTen);
    
    int tempPrice;
    cout << "Nhap gia: ";
    in >> tempPrice; 
    p.setPrice(tempPrice);

    in.ignore();
    return in;
}

// Đặt mã mặt hàng
void Item::setmaItem(string maIteam) {
    this->maItem = maIteam;
}

// Lấy mã mặt hàng
string Item::getmaItem() const {
    return maItem;
}

// Đặt tên mặt hàng
void Item::setTenItem(string tenIteam) {
    this->tenItem = tenIteam;
}

// Lấy tên mặt hàng
string Item::getTenItem() const {
    return tenItem;
}

// Đặt giá
void Item::setPrice(int price) {
    this->price = price;
}

// Lấy giá
int Item::getprice() const {
    return price;
}

// Đọc danh sách mặt hàng từ file vào mảng
int getInfo(Item* p) {
    ifstream myFile("data/Item/Item.txt");
    if (!myFile.is_open()) {
        return -1;
    }
    
    int total = 0;
    string ma, ten, line;
    int price;

    while (getline(myFile, ma) && 
           getline(myFile, ten) && 
           getline(myFile, line)) {
        stringstream geek(line);
        geek >> price;
        
        p[total].setmaItem(ma);
        p[total].setTenItem(ten);
        p[total].setPrice(price);

        total++;
        if (total >= 1000) break;
    }
    myFile.close();
    return total;
}

// Tải toàn bộ mặt hàng vào vector
bool loadAllItems(vector<Item> &items) {
    ifstream myFile("data/Item/Item.txt");
    if (!myFile.is_open()) {
        return false;
    }
    
    items.clear();
    string ma, ten, line;
    int price;
    
    while (getline(myFile, ma) && 
           getline(myFile, ten) && 
           getline(myFile, line)) {
        Item temp;
        stringstream geek(line);
        geek >> price;
        
        temp.setmaItem(ma);
        temp.setTenItem(ten);
        temp.setPrice(price);

        items.push_back(temp);
    }
    myFile.close();
    return true;
}

// Hiển thị mặt hàng theo mã
bool displayItemByCode(Item* p, string x) {
    int total = getInfo(p);
    if (total <= 0) return false;
    
    for (int i = 0; i < total; i++) {
        if (x == p[i].getmaItem()) {
            cout << "MaMH: " << p[i].getmaItem() << endl;
            cout << "Ten mon: " << p[i].getTenItem() << endl;
            cout << "Gia: " << p[i].getprice() << endl;
            return true;
        }
    }
    return false;
}

// Hiển thị toàn bộ mặt hàng ra console
bool displayAllItems(Item* p) {
    int total = getInfo(p);
    if (total <= 0) return false;
    
    cout << "DANH SACH TAT CA MAT HANG:" << endl;
    cout << "--------------------------" << endl;
    for (int i = 0; i < total; i++) {
        cout << "STT " << (i+1) << ": " 
             << p[i].getmaItem() << " | "
             << p[i].getTenItem() << " | "
             << p[i].getprice() << " VND" << endl;
    }
    cout << "--------------------------" << endl;
    cout << "Tong cong: " << total << " mat hang" << endl;
    return true;
}

// Thêm mặt hàng mới (ghi file)
bool addItem(Item* p) {
    Item x;
    cin >> x;
    
    ofstream file2("data/Item/Item.txt", ios::app);
    if (!file2.is_open()) {
        return false;
    }
    
    file2 << x;
    file2.close();
    return true;
}

// Kiểm tra mã mặt hàng có tồn tại
bool check_maItem(Item *p, string ma) {
    int total = getInfo(p);
    if (total <= 0) return false;
    
    for (int i = 0; i < total; i++) {
        if (ma == p[i].getmaItem()) {
            return true;
        }
    }
    return false;
}

// Xóa mặt hàng theo mã từ file
bool deleteItem(Item *p) {
    string x;
    cout << "Nhap ma mat hang muon xoa: ";
    cin >> x;
    cin.ignore();
    
    if (!check_maItem(p, x)) {
        cout << "Ma mat hang khong ton tai!" << endl;
        return false;
    }
    
    ifstream is("data/Item/Item.txt");
    if (!is.is_open()) {
        cout << "Khong the mo file!" << endl;
        return false;
    }
    
    ofstream file2("data/temp.txt");
    if (!file2.is_open()) {
        is.close();
        cout << "Khong the tao file tam!" << endl;
        return false;
    }
    
    int n = getInfo(p);
    bool found = false;
    
    for (int j = 0; j < n; j++) {
        if (p[j].getmaItem() != x) {
            file2 << p[j];
        } else {
            found = true;
        }
    }
    
    file2.close();
    is.close();
    
    if (found) {
        remove("data/Item/Item.txt");
        rename("data/temp.txt", "data/Item/Item.txt");
        cout << "Xoa mat hang thanh cong!" << endl;
        return true;
    } else {
        remove("data/temp.txt");
        cout << "Khong tim thay mat hang de xoa!" << endl;
        return false;
    }
}

// Cập nhật mặt hàng theo mã
bool updateItem(Item *p, string ma, const Item &newItem) {
    if (!check_maItem(p, ma)) {
        cout << "Ma mat hang khong ton tai!" << endl;
        return false;
    }
    
    ifstream is("data/Item/Item.txt");
    if (!is.is_open()) {
        cout << "Khong the mo file!" << endl;
        return false;
    }
    
    ofstream file2("temp.txt");
    if (!file2.is_open()) {
        is.close();
        cout << "Khong the tao file tam!" << endl;
        return false;
    }
    
    int n = getInfo(p);
    bool updated = false;
    
    for (int j = 0; j < n; j++) {
        if (p[j].getmaItem() == ma) {
            file2 << newItem;
            updated = true;
        } else {
            file2 << p[j];
        }
    }
    
    file2.close();
    is.close();
    
    if (updated) {
        remove("data/Item/Item.txt");
        rename("data/temp.txt", "data/Item/Item.txt");
        cout << "Cap nhat mat hang thanh cong!" << endl;
        return true;
    } else {
        remove("data/temp.txt");
        cout << "Khong the cap nhat mat hang!" << endl;
        return false;
    }
}

// Tìm kiếm mặt hàng theo tên, trả về số lượng kết quả
int searchItemsByName(Item* p, string name, Item* results) {
    int total = getInfo(p);
    int count = 0;
    
    for (int i = 0; i < total && count < 1000; i++) {
        if (p[i].getTenItem().find(name) != string::npos) { 
            results[count++] = p[i];
        }
    }
    return count;
}

// Đếm tổng số mặt hàng
int countItems() {
    vector<Item> items;
    if (loadAllItems(items)) {
        return items.size();
    }
    return -1;
}
