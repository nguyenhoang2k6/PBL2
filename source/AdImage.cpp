#include <app/AdImage.h>

#ifndef NOMINMAX
#define NOMINMAX 
#endif
#include <windows.h>
#include <commdlg.h>
#include <filesystem>
#include <iostream>
using namespace std;
namespace fs = std::filesystem;
const std::string IMAGE_FOLDER = "data/Image";

// Mở hộp thoại chọn ảnh và trả về đường dẫn
std::string OpenFileDialog() {
    char filename[MAX_PATH] = "";
    OPENFILENAMEA ofn;
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFile = filename;
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrFilter = "Image Files\0*.jpg;*.png;*.bmp;*.jpeg\0All Files\0*.*\0";
    ofn.nFilterIndex = 1;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
    if (GetOpenFileNameA(&ofn)) {
        return std::string(filename);
    }
    return "";
}

// Sao chép ảnh vào thư mục hệ thống theo mã sản phẩm
std::string SaveImageToSystem(const std::string& sourcePath, const std::string& productID) {
    if (sourcePath.empty()) return "";
    try {
        if (!fs::exists(IMAGE_FOLDER)) {
            fs::create_directories(IMAGE_FOLDER);
        }
        fs::path src(sourcePath);
        string extension = src.extension().string();
        fs::path targetPath = fs::path(IMAGE_FOLDER) / (productID + extension);
        fs::copy_file(src, targetPath, fs::copy_options::overwrite_existing);
        cout << "[System] Copy thanh cong: " << targetPath.string() << endl;
        return targetPath.string();
    } catch (const fs::filesystem_error& e) {
        cerr << "[Error] Loi copy file: " << e.what() << endl;
        return "";
    }
}
