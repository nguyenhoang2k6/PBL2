#include <app/ProductListView.h>
#include <fstream>
#include <filesystem>
#include <algorithm>

// Tải danh sách sản phẩm từ file và hiển thị
void ProductListView::loadFromFile(SDL_Renderer* renderer, const std::string& filepath) {
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
        try {
            int price = std::stoi(priceLine);
            it.setPrice(price);
        } catch (...) {
            it.setPrice(0);
        }

        items.push_back(it);
    }

    fin.close();

    syncWithData(renderer, items);

    if (app) {
        if (!hdrCode) hdrCode = new Label("Mã SP", COLOR_BLACK, startX + 10.0f, startY - headerHeight + 10.0f, app->getFont2(), renderer);
        if (!hdrName) hdrName = new Label("Tên", COLOR_BLACK, startX + 200.0f, startY - headerHeight + 10.0f, app->getFont2(), renderer);
        if (!hdrPrice) hdrPrice = new Label("Giá", COLOR_BLACK, startX + 600.0f, startY - headerHeight + 10.0f, app->getFont2(), renderer);
    }
}

// Xóa sản phẩm theo mã và cập nhật file
bool ProductListView::removeItem(const std::string& code, SDL_Renderer* renderer) {
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
        if (in.is_open() && out.is_open()) {
            out << in.rdbuf();
        }
    } catch (...) {
        std::cerr << "Backup failed for: " << dataFilePath << std::endl;
    }

    // Rewrite file
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
