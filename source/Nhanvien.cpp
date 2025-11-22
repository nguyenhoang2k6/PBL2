#include <fstream>
#include <sstream>
#include <string>
#include <iomanip> // Dùng cho setw trong operator << (nếu cần)
#include <stdlib.h>
#include "app/Nhanvien.h" // Giả định Nhanvien.h chứa định nghĩa class Nhanvien

// Không cần các thư viện và hàm liên quan đến UI/Windows nữa
// using namespace std; // Tốt hơn là dùng std::

// Hằng số N không còn cần thiết nếu sử dụng cấp phát động

// --------------------------- HÀM TIỆN ÍCH DỮ LIỆU ----------------------------

/**
 * @brief Đọc thông tin tất cả nhân viên từ file "Nhanvien/Nhanvien.txt" 
 * và lưu vào mảng con trỏ p.
 * @param p Con trỏ tới mảng Nhanvien để lưu dữ liệu.
 * @return Số lượng nhân viên đã đọc (bao gồm cả dòng trống cuối file nếu có).
 */
int getInfo(Nhanvien* p){
    std::ifstream myFile;
    // Mở file ở chế độ nhị phân (ios::binary) thường không cần thiết 
    // trừ khi xử lý binary data, ta dùng mặc định (text mode)
    myFile.open("Nhanvien/Nhanvien.txt");
    if (!myFile.is_open()){
        // Thay vì cout và exit, chỉ nên trả về 0 hoặc ném exception trong môi trường logic
        return 0; 
    }
    int total = 0;
    
    // Đảm bảo không đọc quá giới hạn của mảng p nếu p được cấp phát tĩnh
    // Giả định mảng p đủ lớn (ví dụ: 1000 phần tử)
    while (!myFile.eof()){
        std::string line;
        // Đọc từng trường dữ liệu
        std::getline(myFile, p[total].maNV);
        if (p[total].maNV.empty() && myFile.eof()) break; // Thoát nếu gặp dòng trống cuối file

        std::getline(myFile, p[total].name);
        std::getline(myFile, p[total].chucvu);
        
        std::getline(myFile, line);
        std::stringstream geek(line);
        geek >> p[total].age;
        
        std::getline(myFile, p[total].address);
        std::getline(myFile, p[total].sdt);
        
        total++;
    }
    myFile.close();
    return total;
}

/**
 * @brief Kiểm tra xem mã nhân viên (maNV) của đối tượng x đã tồn tại trong file chưa.
 * @param x Đối tượng Nhanvien cần kiểm tra mã.
 * @return true nếu mã đã tồn tại, false nếu ngược lại.
 */
bool check_exist(const Nhanvien &x){
    // Cấp phát động
    Nhanvien* p = new Nhanvien[1000]; 
    int n = getInfo(p);
    bool exists = false;
    
    // Vòng lặp chỉ nên đến n (tổng số phần tử hợp lệ)
    for (int i = 0; i < n; i++){ 
        if (x.maNV == p[i].maNV) {
            exists = true;
            break; 
        }
    }
    
    delete [] p; // Giải phóng bộ nhớ trong mọi trường hợp (khắc phục rò rỉ bộ nhớ)
    return exists;
}

/**
 * @brief Kiểm tra mã nhân viên có tồn tại trong danh sách đã tải (mảng p) không.
 * Lưu ý: Hàm này tải lại dữ liệu từ file, giống hệt check_exist.
 * @param p Con trỏ tới mảng Nhanvien (chỉ dùng để tải dữ liệu, không dùng dữ liệu cũ)
 * @param ma Mã nhân viên cần kiểm tra.
 * @return 1 nếu mã tồn tại, 0 nếu ngược lại. (Giữ nguyên int để khớp với code cũ)
 */
int check_maNV(Nhanvien *p, std::string ma){
    int total = getInfo(p);
    int check = 0;
    for (int i = 0; i < total; i++){ // Lặp đến total
        if(ma == p[i].maNV){
           check = 1;
           break;
        }
    }
    return check;
}

// ----------------------- OVERLOAD OPERATORS (Không UI) ----------------------

/**
 * @brief Ghi thông tin Nhanvien vào output stream (dùng cho file)
 */
std::ostream& operator <<(std::ostream &out ,const Nhanvien &p){
    out << p.maNV << std::endl;
    out << p.name << std::endl;
    out << p.chucvu << std::endl;
    out << p.age << std::endl;
    out << p.address << std::endl;
    out << p.sdt << std::endl;
    return out;
}

// operator >> đã bị loại bỏ vì nó hoàn toàn phụ thuộc vào giao diện người dùng (cout/cin)

// ------------------------------ GETTERS & SETTERS ---------------------------

void Nhanvien:: setmaNV(std::string maNV){ 
    this->maNV = maNV;
}
std::string Nhanvien:: getmaNV(){
    return maNV;
}
void Nhanvien:: setChucvu(std::string chucvu){
    this->chucvu = chucvu;
}
std::string Nhanvien:: getChucvu(){
    return chucvu;
}
void Nhanvien:: setName(std::string name){
    this->name = name;
}
std::string Nhanvien:: getName(){
    return name;
}
void Nhanvien:: setAge(int age){
    this->age = age;
}
int Nhanvien:: getAge(){
    return age;
}
void Nhanvien:: setAddress(std::string address){
    this->address = address;
}
std::string Nhanvien:: getAddress(){
    return address;
}
void Nhanvien:: setSDT(std::string sdt){
    this->sdt = sdt;
}
std::string Nhanvien:: getSDT(){
    return sdt;
}

// ------------------------------ HÀM CHỨC NĂNG ------------------------------

/**
 * @brief Thêm một nhân viên mới và mật khẩu vào các file.
 * @param x Đối tượng Nhanvien cần thêm.
 * @param pass Mật khẩu của nhân viên.
 * @return true nếu thêm thành công, false nếu thất bại (ví dụ: không mở được file).
 */
bool Add(const Nhanvien& x, const std::string& pass){
    // Kiểm tra tồn tại trước khi thêm (Logic này nên được giữ)
    if (check_exist(x)) {
        return false; // Mã nhân viên đã tồn tại
    }

    // 1. Thêm mật khẩu
    std::ofstream out_pass;
    out_pass.open("Password/staff.txt", std::ios::app);
    if(!out_pass.is_open()) return false;
    out_pass << x.maNV << " " << pass << std::endl;
    out_pass.close();

    // 2. Thêm thông tin nhân viên
    std::ofstream out_nv;
    out_nv.open("Nhanvien/Nhanvien.txt", std::ios::app);
    if(!out_nv.is_open()) return false;
    out_nv << x;
    out_nv.close();
    
    return true; // Thêm thành công
}

/**
 * @brief Xóa một nhân viên khỏi hệ thống bằng mã nhân viên.
 * @param del_maNV Mã nhân viên muốn xóa.
 * @return true nếu xóa thành công, false nếu không tìm thấy mã hoặc thất bại trong thao tác file.
 */
bool Delete(const std::string& del_maNV){
    // Tạo mảng tạm để kiểm tra và lấy dữ liệu
    Nhanvien* p = new Nhanvien[1000];
    int n = getInfo(p);
    
    // 1. Kiểm tra mã nhân viên có tồn tại không
    int index_to_delete = -1;
    for (int i = 0; i < n; i++){ 
        if (del_maNV == p[i].maNV) {
            index_to_delete = i;
            break;
        }
    }

    if (index_to_delete == -1) {
        delete [] p;
        return false; // Mã nhân viên không tồn tại
    }
    
    // 2. Xóa mật khẩu (Password/staff.txt)
    std::ifstream in_pass("Password/staff.txt");
    std::ofstream out_pass("Password/temp.txt");
    if (!in_pass.is_open() || !out_pass.is_open()) {
        delete [] p;
        return false;
    }
    
    std::string username, pass;
    while(in_pass >> username >> pass){
        if(username != del_maNV) {
            out_pass << username << " " << pass << std::endl;
        }
    }
    in_pass.close();
    out_pass.close();
    
    // 3. Xóa thông tin nhân viên (Nhanvien/Nhanvien.txt)
    std::ofstream file2("Nhanvien/temp.txt");
    if (!file2.is_open()) {
        delete [] p;
        return false;
    }
    
    // Ghi lại danh sách nhân viên ngoại trừ người cần xóa
    for(int j = 0; j < n; j++){
        if(j != index_to_delete){
            file2 << p[j];
        }
    }
    file2.close();
    
    // 4. Đổi tên file
    // Xóa file cũ và rename file temp
    
    // Xử lý file Password
    if (std::remove("Password/staff.txt") != 0) { 
        // Xảy ra lỗi khi xóa
        delete [] p;
        return false; 
    }
    if (std::rename("Password/temp.txt", "Password/staff.txt") != 0) {
        // Xảy ra lỗi khi đổi tên
        delete [] p;
        return false;
    }

    // Xử lý file Nhanvien
    if (std::remove("Nhanvien/Nhanvien.txt") != 0) {
        delete [] p;
        return false;
    }
    if (std::rename("Nhanvien/temp.txt", "Nhanvien/Nhanvien.txt") != 0) {
        delete [] p;
        return false;
    }

    delete [] p;
    return true; // Xóa thành công
}