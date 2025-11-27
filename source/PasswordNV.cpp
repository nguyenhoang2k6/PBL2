#include "app/PasswordNV.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cstdio> 
#include <Windows.h> 

using namespace std;

void TextColor7(int x){
    HANDLE color = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute( color , x );
}

// Hàm này chỉ có trách nhiệm xác thực, KHÔNG hiển thị giao diện hay xử lý lỗi
bool Staff(const string& _Username, const string& _Pass){
    ifstream in;
    in.open("data/Password/staff.txt");
    
    if (!in.is_open()) {
        return false; 
    }
    
    string username, pass;
    while(in >> username >> pass){
        if (_Username == username && _Pass == pass) {
            in.close();
            return true; // Xác thực thành công
        }
    }
    
    in.close();
    return false; // Không tìm thấy tài khoản
    return false; // Không tìm thấy tài khoản
}

ResetStatusNV resetPassNV(
    const std::string& username, 
    const std::string& currentPass, 
    const std::string& newPass, 
    const std::string& retypePass
){
    string _Username, pass_from_file;
    bool userFound = false;
    
    // 1. Tìm kiếm và lấy mật khẩu hiện tại
    ifstream on;
    on.open("data/Password/staff.txt");
    if (!on.is_open()) return FILE_IO_ERROR_NV;

    while(on >> _Username >> pass_from_file){
        if (username == _Username) {
            userFound = true;
            break;
        }
    }
    on.close();
    
    if (!userFound) {
        return USER_NOT_FOUND_NV;
    }
    
    // 2. Kiểm tra mật khẩu hiện tại
    if (currentPass != pass_from_file){
        return INCORRECT_CURRENT_PASS_NV;
    }
    
    // 3. Kiểm tra mật khẩu mới
    if (newPass != retypePass){
        return NEW_PASSWORDS_MISMATCH_NV;
    }

    // 4. Thực hiện ghi mật khẩu mới vào file (dùng temp file)
    ofstream out;
    out.open("data/Password/temp.txt", ios::out);
    if (!out.is_open()) return FILE_IO_ERROR_NV;

    ifstream in;
    in.open("data/Password/staff.txt");
    if (!in.is_open()) { 
        out.close(); 
        return FILE_IO_ERROR_NV; 
    }
    
    string u, p;
    while(in >> u >> p){
        if (username == u){
            out << u << " " << newPass << endl; // Ghi mật khẩu mới
        }
        else {
            out << u << " " << p << endl; // Giữ lại các tài khoản khác
        }
    }
    
    in.close();
    out.close();
    
    // 5. Thay thế file
    if (remove("data/Password/staff.txt") != 0) {
        // Xử lý lỗi xóa file gốc nếu cần
        return FILE_IO_ERROR_NV; 
    }
    if (rename("data/Password/temp.txt", "data/Password/staff.txt") != 0) {
        // Xử lý lỗi đổi tên file nếu cần
        return FILE_IO_ERROR_NV;
    }

    return RESET_SUCCESS_NV; // Thành công
}
