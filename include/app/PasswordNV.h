#ifndef PASSWORDNV_H
#define PASSWORDNV_H

#include <string>

// Định nghĩa mã lỗi/trạng thái cho hàm đặt lại mật khẩu
enum ResetStatusNV {
    RESET_SUCCESS_NV = 0,
    INCORRECT_CURRENT_PASS_NV = 1,
    NEW_PASSWORDS_MISMATCH_NV = 2,
    USER_NOT_FOUND_NV = 3,
    FILE_IO_ERROR_NV = 4
};

void TextColor7(int x);

// Giả định hàm này tồn tại:
void passInput(std::string &pass); 

bool Staff(const std::string& _Username, const std::string& _Pass);

ResetStatusNV resetPassNV(
    const std::string& username, 
    const std::string& currentPass, 
    const std::string& newPass, 
    const std::string& retypePass
);

#endif
