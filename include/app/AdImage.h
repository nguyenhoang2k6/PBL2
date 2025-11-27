#pragma once // Chỉ include file này 1 lần để tránh lỗi
#include <string>

// Hàm mở cửa sổ chọn file của Windows
// Trả về: Đường dẫn tuyệt đối của file vừa chọn (VD: "C:\Users\Dat\Desktop\anh.jpg")
std::string OpenFileDialog();

// Hàm copy ảnh vào thư mục dự án và đổi tên
// Input: sourcePath (đường dẫn gốc), productID (mã sản phẩm để đặt tên)
// Trả về: Đường dẫn tương đối để lưu vào DB (VD: "assets/product_images/CF01.jpg")
std::string SaveImageToSystem(const std::string& sourcePath, const std::string& productID);