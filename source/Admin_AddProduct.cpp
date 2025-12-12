#include <app/Admin_AddProduct.h>
#include <app/App.h>        
#include <app/Button.h>
#include <app/Label.h>
#include <app/TextBox.h>
#include <app/Color.h>
#include <app/AdImage.h>
#include <SDL3_image/SDL_image.h>
#include <fstream>
#include <sstream>

Admin_AddProduct::Admin_AddProduct(App* app) :Screen(app) {
    label_title = nullptr;
    label_productCode = nullptr;
    label_productName = nullptr;
    label_price = nullptr;
    label_errorCode = nullptr;
    label_errorName = nullptr;
    label_errorPrice = nullptr;
    label_success = nullptr;
    label_image = nullptr;

    textbox_code = nullptr;
    textbox_name = nullptr;
    textbox_price = nullptr;

    button_save = nullptr;
    button_back = nullptr;
    button_addImage = nullptr;

    imagePath = "";
    imageTexture = nullptr;

    errorCode = false;
    errorName = false;
    errorPrice = false;
    success = false;
}

bool Admin_AddProduct::Init() {
    TTF_Font* font1 = app->getFont1();
    TTF_Font* font2 = app->getFont2();
    TTF_Font* font3 = app->getFont3();
    SDL_Renderer* renderer = app->getRenderer();

    if (!font1 || !font2 || !font3 || !renderer) {
        std::cerr << "Lỗi: Không lấy được tài nguyên từ App!" << std::endl;
        return false;
    }

    // Title
    label_title = new Label("Thêm Sản Phẩm Mới", COLOR_WARNING, 950.0f, 80.0f, font3, renderer);
    
    // Product Code Label
    label_productCode = new Label("Mã Sản Phẩm:", COLOR_BLACK, 200.0f, 220.0f, font2, renderer);
    label_errorCode = new Label("Mã sản phẩm không được để trống hoặc đã tồn tại!", COLOR_UI_RED, 200.0f, 330.0f, font1, renderer);
    
    // Product Name Label
    label_productName = new Label("Tên Sản Phẩm:", COLOR_BLACK, 200.0f, 410.0f, font2, renderer);
    label_errorName = new Label("Tên sản phẩm không được để trống!", COLOR_UI_RED, 200.0f, 520.0f, font1, renderer);
    
    // Price Label
    label_price = new Label("Giá Tiền:", COLOR_BLACK, 200.0f, 600.0f, font2, renderer);
    label_errorPrice = new Label("Giá tiền phải là số và lớn hơn 0!", COLOR_UI_RED, 200.0f, 710.0f, font1, renderer);
    
    // Image Label
    label_image = new Label("Ảnh Sản Phẩm:", COLOR_BLACK, 1350.0f, 220.0f, font2, renderer);
    label_success = new Label("Thêm sản phẩm thành công!", COLOR_UI_GREEN, 900.0f, 1100.0f, font2, renderer);

    // TextBoxes
    textbox_code = new TextBox(700.0f, 240.0f, 600.0f, 70.0f, COLOR_WHITE, COLOR_BLACK, app, font1, "Nhập mã sản phẩm", COLOR_GRAY_SILVER);
    textbox_name = new TextBox(700.0f, 430.0f, 600.0f, 70.0f, COLOR_WHITE, COLOR_BLACK, app, font1, "Nhập tên sản phẩm", COLOR_GRAY_SILVER);
    textbox_price = new TextBox(700.0f, 620.0f, 600.0f, 70.0f, COLOR_WHITE, COLOR_BLACK, app, font1, "Nhập giá tiền", COLOR_GRAY_SILVER);

    // Buttons
    button_addImage = new Button(1350.0f, 650.0f, 600.0f, 70.0f, COLOR_UI_BLUE, "Chọn Ảnh", renderer, font1, COLOR_WHITE);
    button_save = new Button(800.0f, 1050.0f, 300.0f, 80.0f, COLOR_UI_GREEN, "Lưu", renderer, font2, COLOR_WHITE);
    button_back = new Button(1250.0f, 1050.0f, 300.0f, 80.0f, COLOR_UI_RED, "Quay Lại", renderer, font2, COLOR_WHITE);

    if (!label_title || !label_productCode || !label_productName || !label_price ||
        !label_errorCode || !label_errorName || !label_errorPrice || !label_success || !label_image ||
        !textbox_code || !textbox_name || !textbox_price ||
        !button_save || !button_back || !button_addImage) {
        std::cerr << "Tạo UI thất bại!" << std::endl;
        return false;
    }
    return true;
}

Admin_AddProduct::~Admin_AddProduct() {
    delete label_title;
    delete label_productCode;
    delete label_productName;
    delete label_price;
    delete label_errorCode;
    delete label_errorName;
    delete label_errorPrice;
    delete label_success;
    delete label_image;
    delete textbox_code;
    delete textbox_name;
    delete textbox_price;
    delete button_save;
    delete button_back;
    delete button_addImage;
    
    if (imageTexture) {
        SDL_DestroyTexture(imageTexture);
        imageTexture = nullptr;
    }
}

void Admin_AddProduct::handleEvent(const SDL_Event& e) {
    if(textbox_code) textbox_code->handleEvent(e);
    if(textbox_name) textbox_name->handleEvent(e);
    if(textbox_price) textbox_price->handleEvent(e);
    if(button_save) button_save->handleEvent(e);
    if(button_back) button_back->handleEvent(e);
    if(button_addImage) button_addImage->handleEvent(e);

    if (button_back && button_back->isClicked()) {
        app->changeScreen("Admin_SP");
    }

    if (button_addImage && button_addImage->isClicked()) {
        std::string filePath = OpenFileDialog();
        if (!filePath.empty()) {
            imagePath = filePath;
            // Xóa texture cũ nếu có
            if (imageTexture) {
                SDL_DestroyTexture(imageTexture);
                imageTexture = nullptr;
            }
            // Tải ảnh mới
            SDL_Surface* surface = IMG_Load(imagePath.c_str());
            if (surface) {
                SDL_Renderer* renderer = app->getRenderer();
                if (renderer) {
                    imageTexture = SDL_CreateTextureFromSurface(renderer, surface);
                }
                SDL_DestroySurface(surface);
            }
        }
    }

    if(button_save && button_save->isClicked()) {
        if (validateInput()) {
            if (saveProduct()) {
                success = true;
                errorCode = false;
                errorName = false;
                errorPrice = false;
                clearForm();
            } else {
                success = false;
                errorCode = true;
            }
        }
    }
}

void Admin_AddProduct::update() {
    if(textbox_code) textbox_code->update();
    if(textbox_name) textbox_name->update();
    if(textbox_price) textbox_price->update();
    if(button_save) button_save->update();
    if(button_back) button_back->update();
    if(button_addImage) button_addImage->update();
}

void Admin_AddProduct::render(SDL_Renderer* renderer) {
    if(label_title) label_title->render(renderer);
    if(label_productCode) label_productCode->render(renderer);
    if(label_productName) label_productName->render(renderer);
    if(label_price) label_price->render(renderer);
    if(label_image) label_image->render(renderer);

    if(textbox_code) textbox_code->render(renderer);
    if(textbox_name) textbox_name->render(renderer);
    if(textbox_price) textbox_price->render(renderer);

    if(button_save) button_save->render(renderer);
    if(button_back) button_back->render(renderer);
    if(button_addImage) button_addImage->render(renderer);

    // Render ảnh đã chọn
    if (imageTexture) {
        SDL_FRect imageRect = {1350.0f, 280.0f, 600.0f, 350.0f};
        SDL_RenderTexture(renderer, imageTexture, nullptr, &imageRect);
    } else {
        // Render placeholder khi chưa chọn ảnh
        SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
        SDL_FRect imageRect = {1350.0f, 280.0f, 600.0f, 350.0f};
        SDL_RenderFillRect(renderer, &imageRect);
        SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
        SDL_RenderRect(renderer, &imageRect);
    }

    if (errorCode && label_errorCode) {
        label_errorCode->render(renderer);
    }
    if (errorName && label_errorName) {
        label_errorName->render(renderer);
    }
    if (errorPrice && label_errorPrice) {
        label_errorPrice->render(renderer);
    }
    if (success && label_success) {
        label_success->render(renderer);
    }
}

void Admin_AddProduct::onEnter() {
    clearForm();
}

void Admin_AddProduct::onExit() {
}

bool Admin_AddProduct::validateInput() {
    std::string code = textbox_code->getText();
    std::string name = textbox_name->getText();
    std::string priceStr = textbox_price->getText();

    errorCode = false;
    errorName = false;
    errorPrice = false;

    // Validate code
    if (code.empty()) {
        errorCode = true;
    } else {
        // Check if code already exists
        std::ifstream file("data/Item/Item.txt");
        std::string line;
        while (std::getline(file, line)) {
            if (line == code) {
                errorCode = true;
                file.close();
                break;
            }
            std::getline(file, line); // skip name
            std::getline(file, line); // skip price
        }
        file.close();
    }

    // Validate name
    if (name.empty()) {
        errorName = true;
    }

    // Validate price
    if (priceStr.empty()) {
        errorPrice = true;
    } else {
        try {
            int price = std::stoi(priceStr);
            if (price <= 0) {
                errorPrice = true;
            }
        } catch (...) {
            errorPrice = true;
        }
    }

    return !errorCode && !errorName && !errorPrice;
}

bool Admin_AddProduct::saveProduct() {
    std::string code = textbox_code->getText();
    std::string name = textbox_name->getText();
    std::string priceStr = textbox_price->getText();

    try {
        int price = std::stoi(priceStr);

        // Save image to data/Image/ folder with product code as filename
        if (!imagePath.empty()) {
            // SaveImageToSystem returns relative path, but we just need to copy the file
            // Let's directly copy image to data/Image/<code>.png
            std::string destImagePath = "data/Image/" + code + ".png";
            std::ifstream src(imagePath, std::ios::binary);
            std::ofstream dst(destImagePath, std::ios::binary);
            if (src.is_open() && dst.is_open()) {
                dst << src.rdbuf();
                dst.close();
                src.close();
                std::cerr << "Ảnh đã lưu: " << destImagePath << std::endl;
            }
        }

        // Append to Item.txt file
        std::ofstream file("data/Item/Item.txt", std::ios::app);
        if (!file.is_open()) {
            std::cerr << "Không thể mở file Item.txt!" << std::endl;
            return false;
        }

        file << code << std::endl;
        file << name << std::endl;
        file << price << std::endl;

        file.close();

        std::cerr << "Đã thêm sản phẩm: " << code << std::endl;
        return true;
    } catch (...) {
        std::cerr << "Lỗi khi lưu sản phẩm!" << std::endl;
        return false;
    }
}

void Admin_AddProduct::clearForm() {
    if (textbox_code) textbox_code->setText("");
    if (textbox_name) textbox_name->setText("");
    if (textbox_price) textbox_price->setText("");
    
    // Xóa texture ảnh cũ
    if (imageTexture) {
        SDL_DestroyTexture(imageTexture);
        imageTexture = nullptr;
    }
    
    imagePath = "";
    success = false;
    errorCode = false;
    errorName = false;
    errorPrice = false;
}
