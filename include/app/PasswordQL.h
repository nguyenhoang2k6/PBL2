#ifndef PASSWORDQL_H
#define PASSWORDQL_H

#include <string>

// -------------------------------- UTILITY FUNCTIONS (Giao diện phụ trợ) --------------------------------

/**
 * @brief Hàm tiện ích để đổi màu chữ trong console (Windows-specific).
 * @param x Mã màu.
 */
void TextColor3(int x);

/**
 * @brief Nhập mật khẩu và hiển thị ký tự '*' thay thế (sử dụng _getch()).
 * @param pass Tham chiếu đến chuỗi std::string để lưu trữ mật khẩu.
 */
void passInput(std::string &pass);

// -------------------------------- MANAGEMENT FUNCTIONS (Logic quản lý) --------------------------------

/**
 * @brief Thực hiện kiểm tra đăng nhập quản lý.
 * * Đọc tên đăng nhập và mật khẩu từ file "Password/admin.txt" và so sánh.
 * * @param inputUsername Tên đăng nhập người dùng nhập vào.
 * @param inputPass Mật khẩu người dùng nhập vào.
 * @return true nếu đăng nhập thành công, false nếu sai tên đăng nhập/mật khẩu hoặc lỗi file.
 */
bool Manager(const std::string& inputUsername, const std::string& inputPass);

/**
 * @brief Thực hiện đặt lại mật khẩu quản lý.
 * * Đọc tên đăng nhập và mật khẩu hiện tại từ file, kiểm tra, và sau đó ghi mật khẩu mới vào file.
 * * @param currentPass Mật khẩu hiện tại người dùng nhập vào.
 * @param newPass Mật khẩu mới.
 * @param retypePass Nhập lại mật khẩu mới.
 * @return 0: Thành công.
 * 1: Sai mật khẩu hiện tại.
 * 2: Mật khẩu mới và nhập lại không khớp.
 * 3: Lỗi khi truy cập/ghi file.
 */
int resetPass(const std::string& currentPass, const std::string& newPass, const std::string& retypePass);

#endif // PASSWORDQL_H