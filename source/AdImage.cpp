#include <app/AdImage.h>

// QUAN TRỌNG: Phải define cái này để Windows không định nghĩa lại min/max gây lỗi
#ifndef NOMINMAX
#define NOMINMAX 
#endif

#include <windows.h>   // Thư viện Windows
#include <commdlg.h>   // Thư viện hộp thoại Common Dialog
#include <filesystem>  // Thư viện xử lý file (C++17)
#include <iostream>    // Để in log lỗi

// Rút gọn tên namespace cho đỡ phải gõ dài
namespace fs = std::filesystem;

// Cấu hình thư mục chứa ảnh sản phẩm (Bạn có thể sửa đường dẫn này)
const std::string IMAGE_FOLDER = "data/Image";

std::string OpenFileDialog() {
    char filename[MAX_PATH] = ""; // Bộ đệm chứa tên file

    OPENFILENAMEA ofn; // Cấu trúc dữ liệu của Windows
    ZeroMemory(&ofn, sizeof(ofn)); // Xóa sạch bộ nhớ struct này trước khi dùng

    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL; // Handle cửa sổ cha (để NULL là dùng desktop)
    ofn.lpstrFile = filename;
    ofn.nMaxFile = MAX_PATH;
    
    // Bộ lọc chỉ cho chọn file ảnh
    ofn.lpstrFilter = "Image Files\0*.jpg;*.png;*.bmp;*.jpeg\0All Files\0*.*\0";
    ofn.nFilterIndex = 1;
    
    // Flags quan trọng:
    // OFN_PATHMUSTEXIST: Đường dẫn phải tồn tại
    // OFN_FILEMUSTEXIST: File phải tồn tại
    // OFN_NOCHANGEDIR: CỰC KỲ QUAN TRỌNG! Giữ nguyên thư mục làm việc để SDL không bị lỗi load ảnh/font
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

    // Mở hộp thoại
    if (GetOpenFileNameA(&ofn)) {
        return std::string(filename);
    }

    // Nếu người dùng ấn Cancel
    return "";
}

std::string SaveImageToSystem(const std::string& sourcePath, const std::string& productID) {
    if (sourcePath.empty()) return "";

    try {
        // 1. Kiểm tra và tạo thư mục đích nếu chưa có
        if (!fs::exists(IMAGE_FOLDER)) {
            fs::create_directories(IMAGE_FOLDER);
        }

        // 2. Lấy đuôi file gốc (ví dụ .jpg hay .png)
        fs::path src(sourcePath);
        std::string extension = src.extension().string();

        // 3. Tạo đường dẫn đích: Folder + Mã SP + Đuôi file
        // Ví dụ: assets/product_images/SP001.png
        fs::path targetPath = fs::path(IMAGE_FOLDER) / (productID + extension);

        // 4. Copy file (Ghi đè nếu đã tồn tại - overwrite_existing)
        fs::copy_file(src, targetPath, fs::copy_options::overwrite_existing);

        std::cout << "[System] Copy thanh cong: " << targetPath.string() << std::endl;

        // Trả về đường dẫn dạng string để lưu vào code
        return targetPath.string();
    }
    catch (const fs::filesystem_error& e) {
        std::cerr << "[Error] Loi copy file: " << e.what() << std::endl;
        return "";
    }
}