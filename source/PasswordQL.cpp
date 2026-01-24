#include "app/PasswordQL.h"
#include <iostream>
#include <fstream>
#include <string>
#include <conio.h> 
#include <Windows.h> 

using namespace std;

// Đổi màu chữ console (Windows)
void TextColor3(int x){
    HANDLE color = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute( color , x );
}

// Nhập mật khẩu ẩn ký tự bằng dấu *
void passInput(string &pass){
    char c;
    pass = "";
    while((c = _getch()) != 13){
        if (c == 8){
            if(pass.length() > 0){
                cout << "\b \b";
                pass.erase(pass.length() - 1); 
            }
        } else {
            pass += c;
            cout << "*";
        }
    }
}

// Kiểm tra đăng nhập quản trị
bool Manager(const std::string& inputUsername, const std::string& inputPass){
    string username, pass;
    ifstream in;
    in.open("data/Password/admin.txt");
    
    if (!in.is_open()) {
        return false;
    }
    
    in >> username >> pass;
    in.close();
    
    return (inputUsername == username && inputPass == pass);
}

// Đặt lại mật khẩu quản trị
int resetPass(const std::string& currentPass, const std::string& newPass, const std::string& retypePass){
    string username, pass_from_file;
    ifstream in;
    in.open("data/Password/admin.txt");
    
    if (!in.is_open()) {
        return 3;
    }
    in >> username >> pass_from_file;
    in.close();
    
    if (currentPass != pass_from_file) {
        return 1;
    }
    
    if (newPass != retypePass) {
        return 2;
    }

    ofstream out;
    out.open("data/Password/admin.txt", ios::trunc);
    
    if (out.is_open()){
        out << username << " " << newPass;
        out.close();
        return 0;
    } else {
        return 3;
    }
}
