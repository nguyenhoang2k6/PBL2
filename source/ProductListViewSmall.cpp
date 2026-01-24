#include <app/ProductListViewSmall.h>
#include <fstream>
#include <algorithm>

// Tải danh sách sản phẩm (bản nhỏ) từ file
void ProductListViewSmall::loadFromFile(SDL_Renderer* renderer, const std::string& filepath) {
    std::vector<Item> items;
    dataFilePath = filepath;

    std::ifstream fin(filepath);
    if (!fin.is_open()) {
        std::cerr << "Cannot open item data file: " << filepath << std::endl;
        return;
    }

    std::string code, name, priceLine;
    while (std::getline(fin, code)) {
        if (code.size() == 0) continue;
        if (!std::getline(fin, name)) break;
        if (!std::getline(fin, priceLine)) break;
        Item it;
        it.setmaItem(code);
        it.setTenItem(name);
        try { it.setPrice(std::stoi(priceLine)); } catch(...) { it.setPrice(0); }
        items.push_back(it);
    }
    fin.close();

    syncWithData(renderer, items);

}

// Xóa sản phẩm theo mã và cập nhật file (bản nhỏ)
bool ProductListViewSmall::removeItem(const std::string& code, SDL_Renderer* renderer) {
    if (dataFilePath.empty()) {
        std::cerr << "Data file path not set. Call loadFromFile first." << std::endl;
        return false;
    }

    std::vector<Item> items;
    std::ifstream fin(dataFilePath);
    if (!fin.is_open()) {
        std::cerr << "Cannot open item data file for reading: " << dataFilePath << std::endl;
        return false;
    }

    std::string codeLine, nameLine, priceLine;
    while (std::getline(fin, codeLine)) {
        if (codeLine.size() == 0) continue;
        if (!std::getline(fin, nameLine)) break;
        if (!std::getline(fin, priceLine)) break;
        Item it;
        it.setmaItem(codeLine);
        it.setTenItem(nameLine);
        try { it.setPrice(std::stoi(priceLine)); } catch(...) { it.setPrice(0); }
        items.push_back(it);
    }
    fin.close();

    auto origCount = items.size();
    items.erase(std::remove_if(items.begin(), items.end(), [&](const Item &it){ return it.getmaItem() == code; }), items.end());
    if (items.size() == origCount) {
        std::cerr << "Item code not found: " << code << std::endl;
        return false;
    }

    try {
        std::string bakPath = dataFilePath + ".bak";
        std::ifstream in(dataFilePath, std::ios::binary);
        std::ofstream out(bakPath, std::ios::binary);
        if (in.is_open() && out.is_open()) out << in.rdbuf();
    } catch(...) {
        std::cerr << "Backup failed for: " << dataFilePath << std::endl;
    }

    std::ofstream fout(dataFilePath, std::ios::trunc);
    if (!fout.is_open()) {
        std::cerr << "Cannot open item data file for writing: " << dataFilePath << std::endl;
        return false;
    }

    for (const auto &it : items) {
        fout << it.getmaItem() << "\n";
        fout << it.getTenItem() << "\n";
        fout << it.getprice() << "\n";
    }
    fout.close();

    syncWithData(renderer, items);
    return true;
}
