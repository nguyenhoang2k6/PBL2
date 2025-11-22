#ifndef PASSWORDNV_H
#define PASSWORDNV_H

#include <string>

// Định nghĩa mã lỗi/trạng thái cho hàm đặt lại mật khẩu
enum ResetStatusNV {
    RESET_SUCCESS_NV = 0,
    INCORRECT_CURRENT_PASS_NV = 1,
    NEW_PASSWORDS_MISMATCH_NV = 2,
    USER_NOT_FOUND_NV = 3,
    FILE_IO_ERROR_NV = 4 // Lỗi không mở được file staff.txt hoặc temp.txt
};

// -------------------------------- UTILITY FUNCTIONS --------------------------------

/**
 * @brief Hàm tiện ích để đổi màu chữ trong console (Windows-specific).
 * @param x Mã màu.
 */
void TextColor7(int x);

// Hàm nhập mật khẩu (cần được định nghĩa trong file .cpp hoặc file utility chung)
// Giả định hàm này tồn tại:
void passInput(std::string &pass); 

// -------------------------------- STAFF FUNCTIONS --------------------------------

/**
 * @brief Xác thực tên đăng nhập và mật khẩu của nhân viên.
 * @param _Username Tên đăng nhập.
 * @param _Pass Mật khẩu.
 * @return true nếu xác thực thành công, false nếu không thành công hoặc lỗi file.
 */
bool Staff(const std::string& _Username, const std::string& _Pass);

/**
 * @brief Thực hiện logic ĐẶT LẠI MẬT KHẨU NHÂN VIÊN (tách biệt khỏi giao diện).
 * * Hàm này thực hiện các bước:
 * 1. Kiểm tra sự tồn tại của người dùng.
 * 2. Xác thực mật khẩu hiện tại.
 * 3. Kiểm tra mật khẩu mới và nhập lại.
 * 4. Ghi mật khẩu mới vào file (dùng cơ chế temp file).
 * * @param username Tên đăng nhập của nhân viên cần đổi mật khẩu.
 * @param currentPass Mật khẩu hiện tại do người dùng nhập.
 * @param newPass Mật khẩu mới.
 * @param retypePass Nhập lại mật khẩu mới.
 * @return Mã trạng thái (ResetStatusNV) của quá trình.
 */
ResetStatusNV resetPassNV(
    const std::string& username, 
    const std::string& currentPass, 
    const std::string& newPass, 
    const std::string& retypePass
);

#endif // PASSWORDNV_H