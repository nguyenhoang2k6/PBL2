#include <app/Nhanvien.h>
#include <cstdio> // Cho hàm remove, rename

// Constructor mặc định
NhanVien::NhanVien() : tuoi(0) {}

// Constructor đầy đủ
NhanVien::NhanVien(std::string ma, std::string t, std::string cv, int age, std::string dc, std::string phone)
    : maNV(ma), ten(t), chucVu(cv), tuoi(age), diaChi(dc), sdt(phone) {}

// --- HÀM 1: LẤY TOÀN BỘ DỮ LIỆU ---
std::vector<NhanVien> NhanVien::getAll() {
    std::vector<NhanVien> list;
    // Đảm bảo đường dẫn file đúng (ví dụ: data/nhanvien.txt)
    std::ifstream file("data/Nhanvien/Nhanvien.txt"); 
    
    if (!file.is_open()) return list; // Trả về rỗng nếu lỗi

    std::string ma, t, cv, dc, phone, ageStr;
    
    // Giả sử định dạng file: Mỗi thuộc tính 1 dòng
    while (std::getline(file, ma)) {
        if (ma.empty()) continue; 

        std::getline(file, t);
        std::getline(file, cv);
        std::getline(file, ageStr);
        std::getline(file, dc);
        std::getline(file, phone);

        int age = 0;
        try { age = std::stoi(ageStr); } catch (...) {}

        // Thêm vào danh sách
        list.push_back(NhanVien(ma, t, cv, age, dc, phone));
    }
    
    file.close();
    return list;
}

// --- HÀM 2: KIỂM TRA TỒN TẠI ---
bool NhanVien::exists(const std::string& maCheck) {
    std::vector<NhanVien> list = getAll();
    for (const auto& nv : list) {
        if (nv.getMaNV() == maCheck) return true;
    }
    return false;
}

// --- HÀM 3: THÊM NHÂN VIÊN ---
bool NhanVien::add(const NhanVien& nv, const std::string& password) {
    if (exists(nv.getMaNV())) return false; // Mã đã tồn tại, không thêm

    // 1. Ghi vào file Nhanvien.txt (Append - Ghi nối tiếp)
    std::ofstream fileNV("data/Nhanvien/Nhanvien.txt", std::ios::app);
    if (!fileNV.is_open()) return false;

    fileNV << nv.getMaNV() << "\n"
           << nv.getTen() << "\n"
           << nv.getChucVu() << "\n"
           << nv.getTuoi() << "\n"
           << nv.getDiaChi() << "\n"
           << nv.getSDT() << "\n";
    fileNV.close();

    // 2. Ghi vào file mật khẩu (nếu cần)
    std::ofstream filePass("data/Password/staff.txt", std::ios::app);
    if (filePass.is_open()) {
        filePass << nv.getMaNV() << " " << password << "\n";
        filePass.close();
    }

    return true;
}

// --- HÀM 4: XÓA NHÂN VIÊN ---
bool NhanVien::remove(const std::string& maDel) {
    std::vector<NhanVien> list = getAll(); // Lấy tất cả ra RAM
    bool found = false;

    std::ofstream file("data/Nhanvien/temp.txt"); // Ghi vào file tạm
    if (!file.is_open()) return false;

    for (const auto& nv : list) {
        if (nv.getMaNV() == maDel) {
            found = true; 
            continue; // Bỏ qua người này (không ghi lại -> coi như xóa)
        }
        // Ghi lại người khác
        file << nv.getMaNV() << "\n"
             << nv.getTen() << "\n"
             << nv.getChucVu() << "\n"
             << nv.getTuoi() << "\n"
             << nv.getDiaChi() << "\n"
             << nv.getSDT() << "\n";
    }
    file.close();

    // Xóa file cũ, đổi tên file tạm thành file chính
    std::remove("data/Nhanvien/Nhanvien.txt");
    std::rename("data/Nhanvien/temp.txt", "data/Nhanvien/Nhanvien.txt");

    // (Bạn nên làm tương tự cho file Password nếu muốn xóa sạch sẽ)

    return found;
}