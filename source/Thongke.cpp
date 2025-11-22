#include "app/Thongke.h"
#include "app/Hoadon.h"   // Cần khai báo này cho các class liên quan đến Hoadon
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <conio.h>
#include <windows.h>
#include <limits> // Dùng cho numeric_limits

using namespace std;


void InLine4(int n){
    for (int i = 0; i < n; i++) 
        cout << "=";
}

void TextColor4(int x)
{
    HANDLE mau;
    mau = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(mau,x);
}

void read1(const string &x){
    ifstream in;
    in.open(x);
    if (!in.is_open()) {
        cout << "Loi: Khong the mo file chi tiet hoa don: " << x << endl;
        return;
    }
    string line;
    while (getline(in,line)){
        cout << line << endl;
    }
    in.close();
}

/**
 * @brief Kiểm tra năm nhuận (0: Nhuận, 1: Không nhuận)
 * @param y Năm
 * @return 0 nếu nhuận, 1 nếu không nhuận
 */
int KiemTraNam(int y){
    if (y % 400 == 0 || (y % 4 == 0 && y % 100 != 0)){
        return 0; // Nhuận
    }
    return 1; // Không nhuận
}

/**
 * @brief Kiểm tra tháng và năm có hợp lệ không
 * @param m Tháng
 * @param y Năm
 * @return 1 nếu hợp lệ, 0 nếu không hợp lệ
 */
int KiemTraThang(int m, int y){
    if (m >= 1 && m <= 12 && y > 0){
        return 1;
    }
    return 0;
}

/**
 * @brief Tính số ngày trong từng tháng của năm y
 * @param y Năm
 * @param m Tháng (Không dùng trong logic tính toán của hàm này, nhưng cần để khớp với header)
 * @param a Mảng 13 phần tử (từ 1 đến 12) chứa số ngày
 * @return true (Luôn true nếu chỉ tính toán, nhưng ta giữ bool để khớp header)
 */
bool SoNgayTrongThang(int y, int m, int a[13]){ // Thêm tham số m để khớp với header
    // Mảng lưu trữ số ngày trong tháng (bỏ qua a[0])
    a[1] = 31; a[2] = 28; a[3] = 31; a[4] = 30; a[5] = 31; a[6] = 30;
    a[7] = 31; a[8] = 31; a[9] = 30; a[10] = 31; a[11] = 30; a[12] = 31;
    
    // Xử lý năm nhuận
    if (KiemTraNam(y) == 0) {
        a[2] = 29;
    }

    return true; // Giữ lại bool để khớp với khai báo trong ThongKe.h
}

/**
 * @brief Kiểm tra ngày, tháng, năm có hợp lệ không
 * @param d Ngày
 * @param m Tháng
 * @param y Năm
 * @return 1 nếu hợp lệ, 0 nếu không hợp lệ
 */
int KiemTraNgay(int d, int m, int y){
    if (KiemTraThang(m, y) == 0) return 0; // Tháng/năm không hợp lệ
    
    int a[13];
    // Giả định ta không cần dùng tham số m trong hàm này, nhưng vẫn truyền vào
    SoNgayTrongThang(y, m, a); 
    
    if (d <= a[m] && d > 0)
        return 1;
    return 0;
}

void xemChiTietHoaDon(const string* line, int tot, int d, int m, int y, const string& type) {
    string maHD1;
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Xóa bộ đệm trước khi nhập string
    
    do {
        cout << "\nChon hoa don can xem: ";
        getline(cin, maHD1); 
        
        int dem = 0;
        for(int i = 0; i < tot; i++){
            if(maHD1 == line[i]){
                dem++;    
                read1("Hoadon/" + maHD1 + ".txt");
                break;
            }
        }
        if(dem == 0){ 
            cout << "\nMa hoa don khong ton tai";
            if(type == "day") {
                cout << " hoac ngay " << d << " thang " << m << " nam " << y;
            } else if(type == "month") {
                cout << " hoac thang " << m << " nam " << y;
            } else if(type == "year") {
                cout << " hoac nam " << y;
            } else if(type == "nhanvien") {
                // Không in ra ngày tháng năm cụ thể khi là thống kê theo nhân viên
                // Mã nhân viên được in trong hàm gọi
            }
            cout << " khong thu ngan hoa don nay";
        }
        
        cout << "\nBan co muon tiep tuc xem hoa don (y/n) ? : ";
        char p; 
        cin >> p;
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Xóa bộ đệm sau khi nhập char
        
        if(p != 'y' && p != 'Y') break;
    } while(true);
}

/**
 * @brief Thống kê doanh thu theo ngày
 * @return true nếu thống kê thành công, false nếu người dùng chọn thoát
 */
bool statsDay(){ // Đã đổi sang bool
    string *line = new string[1000];
    bool continueOperation = true;
    bool result = false; // Mặc định là thất bại (người dùng thoát)
    
    while(continueOperation) {
        system("cls");
        cout << endl; InLine4(19);
        cout << "Thong ke theo ngay";
        InLine4(19); cout << endl;
        
        int d, m, y;
        cout << "Nhap ngay muon xem (d m y): "; 
        if (!(cin >> d >> m >> y)) {
            // Xử lý lỗi nhập
            cout << "\nLoi nhap lieu. Thoat thong ke ngay." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continueOperation = false;
            break;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Xóa bộ đệm sau khi nhập số
        
        if(KiemTraNgay(d, m, y) == 0){
            cout << "\nNgay, thang, nam khong hop le\n";
            cout << "\nBan co muon nhap lai ngay muon xem (y/n) : ";
            char t; cin >> t;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            if(t != 'y' && t != 'Y') {
                continueOperation = false; 
            }
            continue;
        }
        
        int day, month, year;
        string maNV, maHD;
        int price;
        unsigned int sum = 0;
        ifstream infile;
        int tot = 0;
        infile.open("history/log.txt");
        
        if (!infile.is_open()) {
             cout << "Loi: Khong the mo file log.txt." << endl;
             continueOperation = false;
             break;
        }

        while(infile >> day >> month >> year >> maNV >> maHD >> price){
            if(day == d && month == m && year == y){
                line[tot++] = maHD;
                sum += price;
            }
        }
        infile.close();
        
        if(tot == 0){
            cout << "Ngay " << d << " thang " << m << " nam " << y << " khong co hoa don nao\n";
        } else {
            cout << "Hoa don trong ngay " << d << " thang " << m << " nam " << y << " la:\n";
            for(int i = 0; i < tot; i++){
                cout << line[i] << endl;
            }
            cout << "Doanh thu ngay nay la: " << sum << endl;
            result = true; // Đánh dấu là đã thực hiện thống kê thành công
            
            cout << "\nBan co muon xem chi tiet hoa don ngay " << d << " thang " << m << " nam " << y << " khong (y/n) ? : ";
            char v; cin >> v;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            if(v == 'y' || v == 'Y'){
                xemChiTietHoaDon(line, tot, d, m, y, "day");
            }
        }
        
        cout << "\nBan co muon tiep tuc thao tac thong ke doanh thu theo ngay ? (y/n) : ";
        char t; cin >> t;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        continueOperation = (t == 'y' || t == 'Y');
    }
    delete [] line;
    return result; // Trả về trạng thái thống kê thành công
}

/**
 * @brief Thống kê doanh thu theo tháng
 * @return true nếu thống kê thành công, false nếu người dùng chọn thoát
 */
bool statsMonth(){ // Đã đổi sang bool
    string *line = new string[1000];
    bool continueOperation = true;
    bool result = false; // Mặc định là thất bại (người dùng thoát)
    
    while(continueOperation) {
        system("cls");
        cout << endl; InLine4(19);
        cout << "Thong ke theo thang";
        InLine4(19); cout << endl;
        
        int m, y;
        cout << "Nhap thang muon xem (m y): "; 
        if (!(cin >> m >> y)) {
            cout << "\nLoi nhap lieu. Thoat thong ke thang." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continueOperation = false;
            break;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        if(KiemTraThang(m, y) == 0) {
            cout << "\nThang hoac nam khong hop le\n";
            cout << "\nBan co muon nhap lai thang muon xem (y/n) : ";
            char t; cin >> t;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            if(t != 'y' && t != 'Y') {
                 continueOperation = false;
            }
            continue;
        }
        
        int day, month, year;
        string maNV, maHD;
        int price;
        unsigned int sum = 0;
        ifstream infile;
        int tot = 0;
        infile.open("history/log.txt");
        
        if (!infile.is_open()) {
             cout << "Loi: Khong the mo file log.txt." << endl;
             continueOperation = false;
             break;
        }
        
        while(infile >> day >> month >> year >> maNV >> maHD >> price){
            if(month == m && year == y){
                line[tot++] = maHD;
                sum += price;
            }
        }
        infile.close();
        
        if(tot == 0){
            cout << "Thang " << m << " nam " << y << " khong co hoa don nao\n";
        } else {
            cout << "Hoa don trong thang " << m << " nam " << y << " la:\n";
            for(int i = 0; i < tot; i++){
                cout << line[i] << endl;
            }
            cout << "Tong doanh thu thang nay la: " << sum << endl;
            result = true;
            
            cout << "\nBan co muon xem chi tiet hoa don thang " << m << " nam " << y << " khong (y/n) ? : ";
            char v; cin >> v;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            if(v == 'y' || v == 'Y'){
                xemChiTietHoaDon(line, tot, 0, m, y, "month");
            }
        }
        
        cout << "\nBan co muon tiep tuc thao tac thong ke doanh thu theo thang ? (y/n) : ";
        char t; cin >> t;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        continueOperation = (t == 'y' || t == 'Y');
    }
    delete [] line;
    return result;
}

/**
 * @brief Thống kê doanh thu theo năm
 * @return true nếu thống kê thành công, false nếu người dùng chọn thoát
 */
bool statsYear(){ // Đã đổi sang bool
    string *line = new string[10000];
    bool continueOperation = true;
    bool result = false; // Mặc định là thất bại (người dùng thoát)
    
    while(continueOperation) {
        system("cls");
        cout << endl; InLine4(19);
        cout << "Thong ke theo nam";
        InLine4(19); cout << endl;
        
        int y;
        cout << "Nhap nam muon xem: "; 
        if (!(cin >> y)) {
            cout << "\nLoi nhap lieu. Thoat thong ke nam." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continueOperation = false;
            break;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        if(y <= 0){ // Đổi từ y < 0 thành y <= 0
            cout << "\nNam khong hop le\n";
            cout << "\nBan co muon nhap lai nam muon xem (y/n) : ";
            char t; cin >> t;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            if(t != 'y' && t != 'Y') {
                 continueOperation = false;
            }
            continue;
        }
        
        int day, month, year;
        string maNV, maHD;
        int price;
        unsigned int sum = 0;
        ifstream infile;
        int tot = 0;
        infile.open("history/log.txt");
        
        if (!infile.is_open()) {
             cout << "Loi: Khong the mo file log.txt." << endl;
             continueOperation = false;
             break;
        }
        
        while(infile >> day >> month >> year >> maNV >> maHD >> price){
            if(year == y){
                line[tot++] = maHD;
                sum += price;
            }
        }
        infile.close();
        
        if(tot == 0){
            cout << "Nam " << y << " khong co hoa don nao\n";
        } else {
            cout << "Hoa don trong nam " << y << " la:\n";
            for(int i = 0; i < tot; i++){
                cout << line[i] << endl;
            }
            cout << "Doanh thu nam nay la: " << sum << endl;
            result = true;
            
            cout << "\nBan co muon xem chi tiet hoa don nam " << y << " khong (y/n) ? : ";
            char v; cin >> v;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            if(v == 'y' || v == 'Y'){
                xemChiTietHoaDon(line, tot, 0, 0, y, "year");
            }
        }
        
        cout << "\nBan co muon tiep tuc thao tac thong ke doanh thu theo nam ? (y/n) : ";
        char t; cin >> t;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        continueOperation = (t == 'y' || t == 'Y');
    }
    delete [] line;
    return result;
}

/**
 * @brief Thống kê doanh thu theo nhân viên
 * @return true nếu thống kê thành công, false nếu người dùng chọn thoát
 */
bool statsNhanvien(){ // Đã đổi sang bool
    string *line = new string[1000];
    bool continueOperation = true;
    bool result = false; // Mặc định là thất bại (người dùng thoát)
    
    while(continueOperation) {
        system("cls");
        cout << endl; InLine4(19);
        cout << "Thong ke theo nhan vien";
        InLine4(19); cout << endl;
        
        string ma;
        // Cần phải định nghĩa Nhanvien* NV cho check_maNV hoạt động
        // Tuy nhiên, việc tạo mảng Nhanvien lớn (100) trên stack/heap không cần thiết
        // nếu chỉ dùng để kiểm tra mã. Giả định check_maNV không cần tham số này
        // hoặc Nhanvien* được cấp phát bộ nhớ.

        // Giả sử Nhanvien cần được cấp phát
        Nhanvien *NV = new Nhanvien[100]; 
        cout << "Nhap ma nhan vien muon xem: "; 
        // Dùng getline() để tránh lỗi bộ đệm, hoặc dùng cin >> ma rồi cin.ignore()
        cin >> ma;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        if(!check_maNV(NV, ma)){ // Thay check_maNV(NV, ma) == 0 thành !check_maNV(NV, ma)
            cout << "\nMa nhan vien khong hop le\n";
            cout << "\nBan co muon nhap lai ma nhan vien muon xem ?(y/n) : ";
            char t; cin >> t;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            if(t != 'y' && t != 'Y') {
                continueOperation = false;
            }
            delete [] NV;
            continue;
        }
        delete [] NV;
        
        int day, month, year;
        string maNV, maHD;
        int price;
        unsigned int sum = 0;
        ifstream infile;
        int tot = 0;
        infile.open("history/log.txt");
        
        if (!infile.is_open()) {
             cout << "Loi: Khong the mo file log.txt." << endl;
             continueOperation = false;
             break;
        }
        
        while(infile >> day >> month >> year >> maNV >> maHD >> price){
            if(ma == maNV){
                line[tot++] = maHD;
                sum += price;
            }
        }
        infile.close();
        
        if(tot == 0){
            cout << "Nhan vien nay khong co hoa don nao\n";
        } else {
            cout << "Hoa don do nhan vien nay thuc hien la:\n";
            for(int i = 0; i < tot; i++){
                cout << line[i] << endl;
            }
            cout << "Doanh thu cua nhan vien nay la: " << sum << endl;
            result = true;
            
            cout << "\nBan co muon xem chi tiet hoa don cua nhan vien co ma la " << ma << " khong (y/n) ? : ";
            char v; cin >> v;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            if(v == 'y' || v == 'Y'){
                xemChiTietHoaDon(line, tot, 0, 0, 0, "nhanvien"); // Sử dụng hàm xemChiTietHoaDon chung
            }
        }
        
        cout << "\nBan co muon tiep tuc thao tac thong ke doanh thu theo nhan vien ? (y/n) : ";
        char t; cin >> t;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        continueOperation = (t == 'y' || t == 'Y');
    }
    delete [] line;
    return result;
}