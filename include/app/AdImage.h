#pragma once
#include <string>

// Trả về: Đường dẫn tuyệt đối của file vừa chọn (VD: "C:\Users\Dat\Desktop\anh.jpg")
std::string OpenFileDialog();

// Trả về: Đường dẫn tương đối để lưu vào DB (VD: "assets/product_images/CF01.jpg")
std::string SaveImageToSystem(const std::string& sourcePath, const std::string& productID);
