#pragma once
#include <iostream>
#include <string>

// Khai báo sớm
class Item;

/**
 * @brief Class đại diện cho một Mặt hàng (Item)
 */
class Item {
private:
    std::string maItem;
    std::string tenItem;
    int price;

public:
    // Constructor (Bạn nên có một constructor)
    Item() : price(0) {}

    // Destructor
    ~Item() {}

    // --- GETTERS (CẦN CÓ CONST) ---
    // Khai báo hàm getmaItem() LÀ const
    std::string getmaItem() const; 

    // Khai báo hàm getTenItem() LÀ const
    std::string getTenItem() const; 
    
    // Khai báo hàm getprice() LÀ const
    int getprice() const; 

    // --- SETTERS ---
    void setmaItem(std::string maIteam);
    void setTenItem(std::string tenIteam);
    void setPrice(int price);

    // --- FRIEND FUNCTIONS (Cho phép truy cập private) ---
    friend std::ostream& operator<<(std::ostream &out, const Item &p);
    friend std::istream& operator>>(std::istream &in, Item &p);
    
    // Nếu bạn không thể truy cập trực tiếp các biến private trong .cpp 
    // (như ở phiên bản lỗi trước), hãy đảm bảo hai operator này là friend.

    // ... Các hàm thành viên khác (nếu có)
};