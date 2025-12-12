#include <app/Admin_AddEmploy.h>
#include <app/App.h>        
#include <app/Button.h>
#include <app/Label.h>
#include <app/TextBox.h>
#include <app/Color.h>
#include <app/Nhanvien.h>
#include <fstream>
#include <sstream>

Admin_AddEmploy::Admin_AddEmploy(App* app) :Screen(app) {
    label_title = nullptr;
    label_maNV = nullptr;
    label_ten = nullptr;
    label_chucVu = nullptr;
    label_tuoi = nullptr;
    label_diaChi = nullptr;
    label_sdt = nullptr;
    label_password = nullptr;
    label_errorMaNV = nullptr;
    label_errorTen = nullptr;
    label_errorChucVu = nullptr;
    label_errorTuoi = nullptr;
    label_errorDiaChi = nullptr;
    label_errorSDT = nullptr;
    label_errorPassword = nullptr;
    label_success = nullptr;

    textbox_maNV = nullptr;
    textbox_ten = nullptr;
    textbox_chucVu = nullptr;
    textbox_tuoi = nullptr;
    textbox_diaChi = nullptr;
    textbox_sdt = nullptr;
    textbox_password = nullptr;

    button_save = nullptr;
    button_back = nullptr;

    errorMaNV = false;
    errorTen = false;
    errorChucVu = false;
    errorTuoi = false;
    errorDiaChi = false;
    errorSDT = false;
    errorPassword = false;
    success = false;
}

bool Admin_AddEmploy::Init() {
    TTF_Font* font1 = app->getFont1();
    TTF_Font* font2 = app->getFont2();
    TTF_Font* font3 = app->getFont3();
    SDL_Renderer* renderer = app->getRenderer();

    if (!font1 || !font2 || !font3 || !renderer) {
        std::cerr << "Lỗi: Không lấy được tài nguyên từ App!" << std::endl;
        return false;
    }

    // Title
    label_title = new Label("Thêm Nhân Viên Mới", COLOR_WARNING, 950.0f, 80.0f, font3, renderer);
    
    // Mã NV Label
    label_maNV = new Label("Mã NV:", COLOR_BLACK, 150.0f, 200.0f, font2, renderer);
    label_errorMaNV = new Label("Mã nhân viên không được để trống hoặc đã tồn tại!", COLOR_UI_RED, 150.0f, 300.0f, font1, renderer);
    
    // Tên Label
    label_ten = new Label("Tên:", COLOR_BLACK, 150.0f, 360.0f, font2, renderer);
    label_errorTen = new Label("Tên không được để trống!", COLOR_UI_RED, 150.0f, 460.0f, font1, renderer);
    
    // Chức Vụ Label
    label_chucVu = new Label("Chức Vụ:", COLOR_BLACK, 150.0f, 520.0f, font2, renderer);
    label_errorChucVu = new Label("Chức vụ không được để trống!", COLOR_UI_RED, 150.0f, 620.0f, font1, renderer);
    
    // Tuổi Label
    label_tuoi = new Label("Tuổi:", COLOR_BLACK, 150.0f, 680.0f, font2, renderer);
    label_errorTuoi = new Label("Tuổi phải là số và từ 18 đến 100!", COLOR_UI_RED, 150.0f, 780.0f, font1, renderer);
    
    // Địa Chỉ Label
    label_diaChi = new Label("Địa Chỉ:", COLOR_BLACK, 1100.0f, 200.0f, font2, renderer);
    label_errorDiaChi = new Label("Địa chỉ không được để trống!", COLOR_UI_RED, 1100.0f, 300.0f, font1, renderer);
    
    // SDT Label
    label_sdt = new Label("SDT:", COLOR_BLACK, 1100.0f, 360.0f, font2, renderer);
    label_errorSDT = new Label("SDT phải là số và có 10 chữ số!", COLOR_UI_RED, 1100.0f, 460.0f, font1, renderer);
    
    // Mật Khẩu Label
    label_password = new Label("Mật Khẩu:", COLOR_BLACK, 1100.0f, 520.0f, font2, renderer);
    label_errorPassword = new Label("Mật khẩu không được để trống!", COLOR_UI_RED, 1100.0f, 620.0f, font1, renderer);
    
    label_success = new Label("Thêm nhân viên thành công!", COLOR_UI_GREEN, 800.0f, 850.0f, font2, renderer);

    // TextBoxes
    textbox_maNV = new TextBox(600.0f, 220.0f, 400.0f, 70.0f, COLOR_WHITE, COLOR_BLACK, app, font1, "Nhập mã NV", COLOR_GRAY_SILVER);
    textbox_ten = new TextBox(600.0f, 380.0f, 400.0f, 70.0f, COLOR_WHITE, COLOR_BLACK, app, font1, "Nhập tên", COLOR_GRAY_SILVER);
    textbox_chucVu = new TextBox(600.0f, 540.0f, 400.0f, 70.0f, COLOR_WHITE, COLOR_BLACK, app, font1, "Nhập chức vụ", COLOR_GRAY_SILVER);
    textbox_tuoi = new TextBox(600.0f, 700.0f, 400.0f, 70.0f, COLOR_WHITE, COLOR_BLACK, app, font1, "Nhập tuổi", COLOR_GRAY_SILVER);
    
    textbox_diaChi = new TextBox(1550.0f, 220.0f, 400.0f, 70.0f, COLOR_WHITE, COLOR_BLACK, app, font1, "Nhập địa chỉ", COLOR_GRAY_SILVER);
    textbox_sdt = new TextBox(1550.0f, 380.0f, 400.0f, 70.0f, COLOR_WHITE, COLOR_BLACK, app, font1, "Nhập SDT", COLOR_GRAY_SILVER);
    textbox_password = new TextBox(1550.0f, 540.0f, 400.0f, 70.0f, COLOR_WHITE, COLOR_BLACK, app, font1, "Nhập mật khẩu", COLOR_GRAY_SILVER);

    // Buttons
    button_save = new Button(700.0f, 900.0f, 300.0f, 80.0f, COLOR_UI_GREEN, "Lưu", renderer, font2, COLOR_WHITE);
    button_back = new Button(1150.0f, 900.0f, 300.0f, 80.0f, COLOR_UI_RED, "Quay Lại", renderer, font2, COLOR_WHITE);

    if (!label_title || !label_maNV || !label_ten || !label_chucVu || !label_tuoi || !label_diaChi || !label_sdt || !label_password ||
        !label_errorMaNV || !label_errorTen || !label_errorChucVu || !label_errorTuoi || !label_errorDiaChi || !label_errorSDT || !label_errorPassword || !label_success ||
        !textbox_maNV || !textbox_ten || !textbox_chucVu || !textbox_tuoi || !textbox_diaChi || !textbox_sdt || !textbox_password ||
        !button_save || !button_back) {
        std::cerr << "Tạo UI thất bại!" << std::endl;
        return false;
    }
    return true;
}

Admin_AddEmploy::~Admin_AddEmploy() {
    delete label_title;
    delete label_maNV;
    delete label_ten;
    delete label_chucVu;
    delete label_tuoi;
    delete label_diaChi;
    delete label_sdt;
    delete label_password;
    delete label_errorMaNV;
    delete label_errorTen;
    delete label_errorChucVu;
    delete label_errorTuoi;
    delete label_errorDiaChi;
    delete label_errorSDT;
    delete label_errorPassword;
    delete label_success;
    
    delete textbox_maNV;
    delete textbox_ten;
    delete textbox_chucVu;
    delete textbox_tuoi;
    delete textbox_diaChi;
    delete textbox_sdt;
    delete textbox_password;
    
    delete button_save;
    delete button_back;
}

void Admin_AddEmploy::handleEvent(const SDL_Event& e) {
    if(textbox_maNV) textbox_maNV->handleEvent(e);
    if(textbox_ten) textbox_ten->handleEvent(e);
    if(textbox_chucVu) textbox_chucVu->handleEvent(e);
    if(textbox_tuoi) textbox_tuoi->handleEvent(e);
    if(textbox_diaChi) textbox_diaChi->handleEvent(e);
    if(textbox_sdt) textbox_sdt->handleEvent(e);
    if(textbox_password) textbox_password->handleEvent(e);
    if(button_save) button_save->handleEvent(e);
    if(button_back) button_back->handleEvent(e);

    if (button_back && button_back->isClicked()) {
        app->changeScreen("Admin_NV");
    }

    if(button_save && button_save->isClicked()) {
        if (validateInput()) {
            if (saveEmployee()) {
                success = true;
                errorMaNV = false;
                errorTen = false;
                errorChucVu = false;
                errorTuoi = false;
                errorDiaChi = false;
                errorSDT = false;
                errorPassword = false;
                clearForm();
            } else {
                success = false;
                errorMaNV = true;
            }
        }
    }
}

void Admin_AddEmploy::update() {
    if(textbox_maNV) textbox_maNV->update();
    if(textbox_ten) textbox_ten->update();
    if(textbox_chucVu) textbox_chucVu->update();
    if(textbox_tuoi) textbox_tuoi->update();
    if(textbox_diaChi) textbox_diaChi->update();
    if(textbox_sdt) textbox_sdt->update();
    if(textbox_password) textbox_password->update();
    if(button_save) button_save->update();
    if(button_back) button_back->update();
}

void Admin_AddEmploy::render(SDL_Renderer* renderer) {
    if(label_title) label_title->render(renderer);
    if(label_maNV) label_maNV->render(renderer);
    if(label_ten) label_ten->render(renderer);
    if(label_chucVu) label_chucVu->render(renderer);
    if(label_tuoi) label_tuoi->render(renderer);
    if(label_diaChi) label_diaChi->render(renderer);
    if(label_sdt) label_sdt->render(renderer);
    if(label_password) label_password->render(renderer);

    if(textbox_maNV) textbox_maNV->render(renderer);
    if(textbox_ten) textbox_ten->render(renderer);
    if(textbox_chucVu) textbox_chucVu->render(renderer);
    if(textbox_tuoi) textbox_tuoi->render(renderer);
    if(textbox_diaChi) textbox_diaChi->render(renderer);
    if(textbox_sdt) textbox_sdt->render(renderer);
    if(textbox_password) textbox_password->render(renderer);

    if(button_save) button_save->render(renderer);
    if(button_back) button_back->render(renderer);

    if (errorMaNV && label_errorMaNV) {
        label_errorMaNV->render(renderer);
    }
    if (errorTen && label_errorTen) {
        label_errorTen->render(renderer);
    }
    if (errorChucVu && label_errorChucVu) {
        label_errorChucVu->render(renderer);
    }
    if (errorTuoi && label_errorTuoi) {
        label_errorTuoi->render(renderer);
    }
    if (errorDiaChi && label_errorDiaChi) {
        label_errorDiaChi->render(renderer);
    }
    if (errorSDT && label_errorSDT) {
        label_errorSDT->render(renderer);
    }
    if (errorPassword && label_errorPassword) {
        label_errorPassword->render(renderer);
    }
    if (success && label_success) {
        label_success->render(renderer);
    }
}

void Admin_AddEmploy::onEnter() {
    clearForm();
}

void Admin_AddEmploy::onExit() {
}

bool Admin_AddEmploy::validateInput() {
    std::string maNV = textbox_maNV->getText();
    std::string ten = textbox_ten->getText();
    std::string chucVu = textbox_chucVu->getText();
    std::string tuoiStr = textbox_tuoi->getText();
    std::string diaChi = textbox_diaChi->getText();
    std::string sdt = textbox_sdt->getText();
    std::string password = textbox_password->getText();

    errorMaNV = maNV.empty() || NhanVien::exists(maNV);
    errorTen = ten.empty();
    errorChucVu = chucVu.empty();
    errorDiaChi = diaChi.empty();
    errorSDT = sdt.empty() || sdt.length() != 10;
    errorPassword = password.empty();

    errorTuoi = false;
    int tuoi = 0;
    try {
        tuoi = std::stoi(tuoiStr);
        if (tuoi < 18 || tuoi > 100) {
            errorTuoi = true;
        }
    } catch (...) {
        errorTuoi = !tuoiStr.empty();
    }

    return !errorMaNV && !errorTen && !errorChucVu && !errorTuoi && !errorDiaChi && !errorSDT && !errorPassword;
}

bool Admin_AddEmploy::saveEmployee() {
    try {
        std::string maNV = textbox_maNV->getText();
        std::string ten = textbox_ten->getText();
        std::string chucVu = textbox_chucVu->getText();
        int tuoi = std::stoi(textbox_tuoi->getText());
        std::string diaChi = textbox_diaChi->getText();
        std::string sdt = textbox_sdt->getText();
        std::string password = textbox_password->getText();

        NhanVien nv(maNV, ten, chucVu, tuoi, diaChi, sdt);
        return NhanVien::add(nv, password);
    } catch (...) {
        return false;
    }
}

void Admin_AddEmploy::clearForm() {
    if (textbox_maNV) textbox_maNV->setText("");
    if (textbox_ten) textbox_ten->setText("");
    if (textbox_chucVu) textbox_chucVu->setText("");
    if (textbox_tuoi) textbox_tuoi->setText("");
    if (textbox_diaChi) textbox_diaChi->setText("");
    if (textbox_sdt) textbox_sdt->setText("");
    if (textbox_password) textbox_password->setText("");
    
    success = false;
    errorMaNV = false;
    errorTen = false;
    errorChucVu = false;
    errorTuoi = false;
    errorDiaChi = false;
    errorSDT = false;
    errorPassword = false;
}
