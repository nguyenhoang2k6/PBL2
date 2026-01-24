#pragma once
#include <app/Screen.h>
#include <iostream>
#include <SDL3/SDL.h>
#include <app/HoaDon.h>
#include <app/InvoiceListView.h>

class Button;
class Label;
class TextBox;
class ProductListViewSmall;

class NVHD : public Screen
{
private:
    Label* label_title;
    Label* label_tongtien;
    TextBox* textbox_maHD;
    TextBox* textbox_ngayHD;
    TextBox* textbox_maNV;
    Button* button_submit;
    Button* button_back;
    Hoadon* hoadon;
    ProductListViewSmall* productList;
    InvoiceListView* invoiceListUI;
    Label* notificationLabel;
    float notificationTimer;
    float notificationDuration;
    bool notificationPersist;
    // Render the invoice items list (name, quantity, price)
    void renderHoaDon(SDL_Renderer* renderer);

public:
    NVHD(App* app);
    ~NVHD();
    bool Init() override;

    void handleEvent(const SDL_Event& e) override;
    void update() override;
    void render(SDL_Renderer* renderer) override;
    void onEnter() override;
    void onExit() override;
};
