#include "Hoadon.h"
#include "Item.h"
#include "Nhanvien.h"
#include "Thongke.h"
#include<windows.h>
#include<iostream>
#include<stdlib.h>
#include<iomanip>
#include<conio.h>
using namespace std;

void TextColor5(int x)
{
	HANDLE mau;
	mau = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(mau,x);
}

void read(const string &x){
    ifstream in;
    in.open(x);
    string line;
    while (getline(in,line)){
        cout<<line<<endl;
    }
    in.close();
}

Hoadon::Hoadon(){
    Menu=new Item[MAX];
    soluong=new int[MAX];
    total = 0;
    price = 0;
}

Hoadon::~Hoadon(){
    delete [] Menu;
    delete [] soluong;
}

bool check_exist(const string &x){
    ifstream in;
    in.open("Hoadon/"+x+".txt");
    if(in){
        in.close();
        return true;
    }
    return false;
}

ostream& operator<<(ostream& out,const Hoadon &p){
    out<<"Ngay thu ngan: "<<p.day<<"/"<<p.month<<"/"<<p.year<<endl;
    out<<"Nhan vien thu ngan: "<<p.maNV<<endl;
    out<<setw(30)<<left<<"Ten mon";
    out<<setw(10)<<left<<"So luong";
    out<<setw(10)<<left<<"Don gia";
    out<<setw(10)<<left<<"Thanh tien"<<endl;
    out<<"---------------------------------------------------------------"<<endl;
    for (int i=0;i<p.total;i++){
        out<<setw(30)<<left<<p.Menu[i].getTenItem();
        out<<setw(10)<<left<<p.soluong[i];
        out<<setw(10)<<left<<p.Menu[i].getprice();
        out<<setw(10)<<left<<p.soluong[i]*p.Menu[i].getprice()<<endl;
    }
    out<<"---------------------------------------------------------------"<<endl;
    out<<"Tong gia tien: "<<p.price<<endl;
    return out;
}

int Cashier(Hoadon &p,const string &maNV){
    p.total = 0;
    p.price = 0;

    // Nhập ngày
    cout << "Nhap ngay thu ngan (d m y): ";
    cin >> p.day >> p.month >> p.year;

    if (KiemTraNgay(p.day, p.month, p.year) == 0) {
        TextColor5(12);
        cout << "\nNgay thang nam khong hop le!\n";
        TextColor5(7);
        return -1;          // ❗ TRẢ VỀ LỖI
    }

    p.maNV = maNV;

    // Nhập mã hóa đơn
    cout << "Nhap ma hoa don: ";
    cin >> p.maHD;
    while (check_exist(p.maHD)) {
        cout << "Ma da ton tai, moi nhap lai: ";
        cin >> p.maHD;
    }

    bool running = true;

    while (running) {
        system("cls");

        Item *x = new Item[MAX];
        display(x);              // hiện menu
        int n = getInfo(x);      // số lượng item trong menu

        string maItem;
        cout << "Nhap ma mat hang: ";
        cin >> maItem;

        int pos = -1;
        for (int i = 0; i < n - 1; i++) {
            if (maItem == x[i].getmaItem()) {
                pos = i;
                break;
            }
        }

        if (pos == -1) {
            TextColor5(12);
            cout << "\nMa Item khong ton tai!\n";
            TextColor5(7);

            delete [] x;
            break;  // THOÁT
        }

        bool exists = false;
        for (int i = 0; i < p.total; i++) {
            if (p.Menu[i].getmaItem() == maItem) {
                cout << "Nhap so luong them: ";
                int sl;
                cin >> sl;
                p.soluong[i] += sl;
                exists = true;
                break;
            }
        }

        if (!exists) {
            cout << "Nhap so luong: ";
            cin >> p.soluong[p.total];
            p.Menu[p.total] = x[pos];
            p.total++;
        }

        delete [] x;

        // Cập nhật tổng tiền
        p.price = 0;
        for (int i = 0; i < p.total; i++)
            p.price += p.soluong[i] * p.Menu[i].getprice();

        // In tạm thời
        system("cls");
        cout << "=== HOA DON TAM THOI ===\n";
        for (int i = 0; i < p.total; i++) {
            cout << setw(30) << left << p.Menu[i].getTenItem()
                 << setw(10) << left << p.soluong[i]
                 << setw(10) << left << p.Menu[i].getprice()
                 << setw(10) << left << p.soluong[i] * p.Menu[i].getprice()
                 << endl;
        }
        cout << "\nTong tam tinh: " << p.price << endl;

        cout << "\n1. Them mon\n2. Thanh toan\nLua chon: ";
        int c; 
        cin >> c;
        if (c == 2) running = false;
    }

    // Lưu hóa đơn
    ofstream file("Hoadon/" + p.maHD + ".txt");
    file << p;
    file.close();

    // Lưu lịch sử
    ofstream log("History/log.txt", ios::app);
    log << p.day << " " << p.month << " " << p.year << " "
        << p.maNV << " " << p.maHD << " " << p.price << endl;
    log.close();

    // In hóa đơn cuối
    system("cls");
    cout << "=== HOA DON CHINH THUC ===\n";
    read("Hoadon/" + p.maHD + ".txt");

    return p.price;         //  TRẢ VỀ TỔNG TIỀN
}


bool display(Hoadon &p) {
    ifstream in("History/log.txt");
    if (!in) return false;

    int d, m, y, price;
    string maNV, maHD;

    cout << "\n+----------------------------------------------------------+\n";
    cout << "|  Ma Hoa Don  |     Thoi gian thuc hien giao dich        |\n";
    cout << "+----------------------------------------------------------+\n";

    cout << "+----------------------------------------------------------+\n";
    in.close();

    cout << "\nNhap ma hoa don can xem: ";
    string choose;
    cin >> choose;

    if (!check_exist(choose)) {
        TextColor5(12);
        cout << "Hoa don khong ton tai!\n";
        TextColor5(7);
        return false;
    }

    system("cls");
    read("Hoadon/" + choose + ".txt");

    return true;
}
