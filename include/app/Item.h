#pragma once
#include <iostream>
#include <string>

// Khai báo sớm
class Item;

class Item {
private:
    std::string maItem;
    std::string tenItem;
    int price;

public:
    Item() : price(0) {}

    ~Item() {}

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
    
};
