#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

class NhanVien {
private:
    std::string maNV;
    std::string ten;
    std::string chucVu;
    int tuoi;
    std::string diaChi;
    std::string sdt;

public:
    NhanVien();
    NhanVien(std::string ma, std::string t, std::string cv, int age, std::string dc, std::string phone);

    std::string getMaNV() const { return maNV; }
    std::string getTen() const { return ten; }
    std::string getChucVu() const { return chucVu; }
    int getTuoi() const { return tuoi; }
    std::string getDiaChi() const { return diaChi; }
    std::string getSDT() const { return sdt; }

    // 1. Lấy toàn bộ danh sách nhân viên từ file
    static std::vector<NhanVien> getAll();

    // 2. Thêm nhân viên mới (kèm mật khẩu) vào file
    static bool add(const NhanVien& nv, const std::string& password);

    // 3. Xóa nhân viên theo mã khỏi file
    static bool remove(const std::string& maNV);

    // 4. Kiểm tra mã nhân viên đã tồn tại chưa
    static bool exists(const std::string& maNV);
};
