#include <app/Nhanvien.h>
#include <cstdio>

// Khởi tạo nhân viên mặc định
NhanVien::NhanVien() : tuoi(0) {}

// Khởi tạo nhân viên với đầy đủ thông tin
NhanVien::NhanVien(std::string ma, std::string t, std::string cv, int age, std::string dc, std::string phone)
    : maNV(ma), ten(t), chucVu(cv), tuoi(age), diaChi(dc), sdt(phone) {}

// Lấy danh sách toàn bộ nhân viên từ file
std::vector<NhanVien> NhanVien::getAll() {
    std::vector<NhanVien> list;
    std::ifstream file("data/Nhanvien/Nhanvien.txt"); 
    
    if (!file.is_open()) return list;

    std::string ma, t, cv, dc, phone, ageStr;
    
    while (std::getline(file, ma)) {
        if (ma.empty()) continue; 

        std::getline(file, t);
        std::getline(file, cv);
        std::getline(file, ageStr);
        std::getline(file, dc);
        std::getline(file, phone);

        int age = 0;
        try { age = std::stoi(ageStr); } catch (...) {}

        list.push_back(NhanVien(ma, t, cv, age, dc, phone));
    }
    
    file.close();
    return list;
}

// Kiểm tra mã nhân viên đã tồn tại
bool NhanVien::exists(const std::string& maCheck) {
    std::vector<NhanVien> list = getAll();
    for (const auto& nv : list) {
        if (nv.getMaNV() == maCheck) return true;
    }
    return false;
}

// Thêm nhân viên và ghi mật khẩu
bool NhanVien::add(const NhanVien& nv, const std::string& password) {
    if (exists(nv.getMaNV())) return false;

    std::ofstream fileNV("data/Nhanvien/Nhanvien.txt", std::ios::app);
    if (!fileNV.is_open()) return false;

    fileNV << nv.getMaNV() << "\n"
           << nv.getTen() << "\n"
           << nv.getChucVu() << "\n"
           << nv.getTuoi() << "\n"
           << nv.getDiaChi() << "\n"
           << nv.getSDT() << "\n";
    fileNV.close();

    std::ofstream filePass("data/Password/staff.txt", std::ios::app);
    if (filePass.is_open()) {
        filePass << nv.getMaNV() << " " << password << "\n";
        filePass.close();
    }

    return true;
}

// Xóa nhân viên theo mã
bool NhanVien::remove(const std::string& maDel) {
    std::vector<NhanVien> list = getAll();
    bool found = false;

    std::ofstream file("data/Nhanvien/temp.txt");
    if (!file.is_open()) return false;

    for (const auto& nv : list) {
        if (nv.getMaNV() == maDel) {
            found = true; 
            continue;
        }
        file << nv.getMaNV() << "\n"
             << nv.getTen() << "\n"
             << nv.getChucVu() << "\n"
             << nv.getTuoi() << "\n"
             << nv.getDiaChi() << "\n"
             << nv.getSDT() << "\n";
    }
    file.close();

    std::remove("data/Nhanvien/Nhanvien.txt");
    std::rename("data/Nhanvien/temp.txt", "data/Nhanvien/Nhanvien.txt");

    return found;
}
