#ifndef NHANVIEN_H
#define NHANVIEN_H

#include<iostream>
#include<fstream>
#include<string>
#include<sstream>
#include<iomanip>
#include <cstdio> // Cần thiết cho hàm remove/rename trong Delete()
#include <stdlib.h> // Cần thiết cho std::exit

// Lớp Nhanvien
class Nhanvien{
private:
    // Thêm std:: cho kiểu dữ liệu để tăng tính rõ ràng
    std::string maNV;
    std::string chucvu;
    std::string name;
    int age;
    std::string address;
    std::string sdt;

public:
    // Constructor
    Nhanvien(){
        this->age = 0;
    }
    
    // ==========================================================
    // KHAI BÁO FRIEND CHO CÁC HÀM LOGIC (ĐƯỢC ĐỊNH NGHĨA Ở FILE .CPP)
    // ==========================================================
    
    // Overload Operator (Output)
    friend std::ostream& operator <<(std::ostream & ,const Nhanvien &);
    // friend std::istream& operator >>(std::istream &,Nhanvien &); // Bỏ vì không có định nghĩa trong .cpp
    
    // Utility functions (getInfo, check_exist, check_maNV)
    friend int getInfo(Nhanvien*);
    friend bool check_exist(const Nhanvien &x);
    friend int check_maNV(Nhanvien *p, std::string ma);

    // CRUD functions (Add, Delete) - Đã sửa chữ ký để khớp với .cpp
    friend bool Add(const Nhanvien& x, const std::string& pass);
    friend bool Delete(const std::string& del_maNV);


    // ==========================================================
    // KHAI BÁO MEMBER FUNCTIONS (ĐƯỢC ĐỊNH NGHĨA Ở FILE .CPP)
    // ==========================================================
    
    // Getters and Setters
    void setmaNV(std::string maNV);
    std::string getmaNV();
    void setChucvu(std::string chucvu);
    std::string getChucvu();
    void setName(std::string name);
    std::string getName();
    void setAge(int age);
    int getAge();
    void setAddress(std::string address);
    std::string getAddress();
    void setSDT(std::string sdt);
    std::string getSDT();

    // Hàm hiển thị
    void display();
};

#endif // NHANVIEN_H