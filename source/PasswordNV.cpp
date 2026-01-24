#include "app/PasswordNV.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cstdio> 
#include <Windows.h> 

using namespace std;

// Đổi màu chữ console (Windows)
void TextColor7(int x){
    HANDLE color = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute( color , x );
}

// Kiểm tra đăng nhập nhân viên
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
            return true;
        }
    }
    
    in.close();
    return false;
    return false;
}

// Đặt lại mật khẩu nhân viên
ResetStatusNV resetPassNV(
    const std::string& username, 
    const std::string& currentPass, 
    const std::string& newPass, 
    const std::string& retypePass
){
    string _Username, pass_from_file;
    bool userFound = false;
    
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
    
    if (currentPass != pass_from_file){
        return INCORRECT_CURRENT_PASS_NV;
    }
    
    if (newPass != retypePass){
        return NEW_PASSWORDS_MISMATCH_NV;
    }

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
            out << u << " " << newPass << endl;
        }
        else {
            out << u << " " << p << endl;
        }
    }
    
    in.close();
    out.close();
    
    if (remove("data/Password/staff.txt") != 0) {
        return FILE_IO_ERROR_NV; 
    }
    if (rename("data/Password/temp.txt", "data/Password/staff.txt") != 0) {
        return FILE_IO_ERROR_NV;
    }

    return RESET_SUCCESS_NV;
}
