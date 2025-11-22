#include "app/PasswordQL.h"
#include <iostream>
#include <fstream>
#include <string>
#include <conio.h> 
#include <Windows.h> 

using namespace std;

// -------------------------------- UTILITY FUNCTIONS (Giao diện phụ trợ) --------------------------------

/**
 * @brief Hàm tiện ích để đổi màu chữ trong console.
 * @param x Mã màu.
 */
void TextColor3(int x){
    HANDLE color = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute( color , x );
}

/**
 * @brief Nhập mật khẩu và hiển thị ký tự '*' thay thế (dùng _getch()).
 * @param pass Tham chiếu đến chuỗi string để lưu trữ mật khẩu.
 */
void passInput(string &pass){
    char c;
    pass = "";
    while((c = _getch()) != 13){ // 13 là mã ASCII của Enter
        if (c == 8){ // 8 là mã ASCII của Backspace
            if(pass.length() > 0){
                cout << "\b \b"; // Xóa ký tự '*' trên màn hình
                pass.erase(pass.length() - 1); 
            }
        } else {
            pass += c;
            cout << "*";
        }
    }
}


/**
 * @brief Thực hiện kiểm tra đăng nhập quản lý.
 * @param inputUsername Tên đăng nhập người dùng nhập vào.
 * @param inputPass Mật khẩu người dùng nhập vào.
 * @return true nếu đăng nhập thành công, false nếu sai tên đăng nhập/mật khẩu.
 */
bool Manager(const std::string& inputUsername, const std::string& inputPass){
    string username, pass;
    ifstream in;
    in.open("Password/admin.txt");
    
    // Đảm bảo file tồn tại và đọc dữ liệu. Nếu không, coi như lỗi hoặc dùng default.
    if (!in.is_open()) {
        // Có thể coi là lỗi file, hoặc dùng giá trị mặc định cho an toàn
        // Tuy nhiên, theo yêu cầu backend, ta giả định file là nguồn duy nhất
        return false; // Lỗi file/không tìm thấy thông tin quản trị
    }
    
    in >> username >> pass;
    in.close();
    
    // Trả về true/false dựa trên kết quả so sánh
    return (inputUsername == username && inputPass == pass);
}

/**
 * @brief Thực hiện đặt lại mật khẩu quản lý.
 * @param currentPass Mật khẩu hiện tại người dùng nhập vào.
 * @param newPass Mật khẩu mới.
 * @param retypePass Nhập lại mật khẩu mới.
 * @return 0: Thành công.
 * 1: Sai mật khẩu hiện tại.
 * 2: Mật khẩu mới và nhập lại không khớp.
 * 3: Lỗi khi truy cập/ghi file.
 */
int resetPass(const std::string& currentPass, const std::string& newPass, const std::string& retypePass){
    string username, pass_from_file;
    ifstream in;
    in.open("Password/admin.txt");
    
    // 1. Đọc mật khẩu hiện tại từ file
    if (!in.is_open()) {
        return 3; // Lỗi file
    }
    in >> username >> pass_from_file;
    in.close();
    
    // 2. Kiểm tra mật khẩu hiện tại
    if (currentPass != pass_from_file) {
        return 1; // Sai mật khẩu hiện tại
    }
    
    // 3. Kiểm tra mật khẩu mới nhập lại
    if (newPass != retypePass) {
        return 2; // Mật khẩu mới không khớp
    }

    // 4. Thực hiện ghi mật khẩu mới vào file
    ofstream out;
    out.open("Password/admin.txt", ios::trunc);
    
    if (out.is_open()){
        out << username << " " << newPass;
        out.close();
        return 0; // Thành công
    } else {
        return 3; // Lỗi ghi file
    }
}