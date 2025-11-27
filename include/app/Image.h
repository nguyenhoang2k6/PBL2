#pragma once
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <string>
#include <iostream>

class Image {
private:
    SDL_Texture* texture;
    SDL_FRect dstRect; // SDL3 dùng FRect (float) cho tọa độ vẽ

public:
    // Constructor: Load ảnh từ đường dẫn
    Image(SDL_Renderer* renderer, std::string path, float x, float y, float w, float h) {
        // 1. Load ảnh thành texture
        texture = IMG_LoadTexture(renderer, path.c_str());
        
        if (texture == nullptr) {
            // Nếu không tìm thấy ảnh, hãy load một ảnh mặc định (placeholder)
            // Hoặc in lỗi ra console để debug
            std::cerr << "Khong load duoc anh: " << path << " - Loi: " << SDL_GetError() << std::endl;
            // Tùy chọn: Load ảnh "no-image.png" nếu cần
        }

        // 2. Thiết lập vị trí và kích thước
        dstRect = { x, y, w, h };
    }

    ~Image() {
        // CỰC KỲ QUAN TRỌNG: Phải hủy texture khi không dùng nữa
        if (texture != nullptr) {
            SDL_DestroyTexture(texture);
        }
    }

    void setPosition(float x, float y) {
        dstRect.x = x;
        dstRect.y = y;
    }

    void render(SDL_Renderer* renderer) {
        if (texture != nullptr) {
            // Vẽ texture lên màn hình
            SDL_RenderTexture(renderer, texture, nullptr, &dstRect);
        } else {
            // Nếu không có ảnh, có thể vẽ một hình chữ nhật rỗng để giữ chỗ
            SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255); // Màu xám
            SDL_RenderRect(renderer, &dstRect);
        }
    }
};