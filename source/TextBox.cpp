#include <app/TextBox.h>
#include <algorithm>
#include <cstring>

TextBox::TextBox(float x, float y, float w, float h, SDL_Color bgColor, SDL_Color textColor,App* app,TTF_Font* font,const string& placeholder, SDL_Color placeholderColor)
    : rect{x,y,w,h}, bgColor(bgColor),textColor(textColor),text(""), textTexture(nullptr), texW(0), texH(0), focused(false), app(app),renderer(app->getRenderer()),font(font), lastBlink(0), caretVisible(true),caretPos(0), placeholder(placeholder), placeholderColor(placeholderColor),placeholderTexture(nullptr),needsTextureUpdate(false), scrollOffset(0.0f) {
    if (font && !placeholder.empty()) {
        SDL_Surface* surf = TTF_RenderText_Solid(font, placeholder.c_str(),placeholder.size(),placeholderColor);
        if (surf) {
            placeholderTexture = SDL_CreateTextureFromSurface(renderer, surf);
            SDL_DestroySurface(surf);
        }
    }
}

TextBox::~TextBox() {
    if(textTexture) {
        SDL_DestroyTexture(textTexture);
        textTexture = nullptr;
    }
    if(placeholderTexture) {
        SDL_DestroyTexture(placeholderTexture);
        placeholderTexture = nullptr;
    }
}

void TextBox::updateTexture() {
    if (textTexture) {
        SDL_DestroyTexture(textTexture);
        textTexture = nullptr;
        texW =0;
        texH =0;
    }

    if (!text.empty() && font && renderer) {
        SDL_Surface* surf = TTF_RenderText_Solid(font, text.c_str(),text.size(),textColor);
        if (surf) {
            textTexture = SDL_CreateTextureFromSurface(renderer, surf);
            texW = surf->w;
            texH = surf->h;
            SDL_DestroySurface(surf);
        }
    }
}

void TextBox::update() {
    if(focused) {
        Uint32 now = SDL_GetTicks();
        if (now - lastBlink > 500) {
            caretVisible = !caretVisible;
            lastBlink = now;
        }
    } else {
        caretVisible = false;
    }

    if (needsTextureUpdate) {
        updateTexture();
        needsTextureUpdate = false;
    }
}

void TextBox::handleEvent(const SDL_Event& e) {
    if(e.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
        int mx = e.button.x, my = e.button.y;
        bool wasFocused = focused;

        SDL_FPoint mousePoint = { (float)mx, (float)my };
        focused = SDL_PointInRectFloat(&mousePoint, &rect);

        if (focused) {
            // Khi click vào, đặt con trỏ về cuối dòng cho tiện (đơn giản hóa logic click)
            caretPos = (int)text.length(); 
            
            if (!wasFocused) {
                SDL_StartTextInput(app->getWindow());
            }
        } else if (wasFocused) {
            SDL_StopTextInput(app->getWindow());
        }
    }

    if(!focused) return;

    if (e.type == SDL_EVENT_TEXT_INPUT) {
        text += e.text.text;
        caretPos += (int)strlen(e.text.text);
        needsTextureUpdate = true;
    } else if (e.type == SDL_EVENT_KEY_DOWN) {
        if (e.key.key == SDLK_BACKSPACE && !text.empty() && caretPos > 0) {
            // Xóa ký tự ĐỨNG TRƯỚC con trỏ
            text.erase(caretPos - 1, 1);
            caretPos--; // Lùi con trỏ lại 1 bước
            needsTextureUpdate = true;
        }
        else if (e.key.key == SDLK_LEFT) {
            // Di chuyển con trỏ sang trái
            if (caretPos > 0) caretPos--;
            // Reset blink để người dùng thấy con trỏ di chuyển ngay lập tức
            caretVisible = true; 
            lastBlink = SDL_GetTicks();
        }
        else if (e.key.key == SDLK_RIGHT) {
            // Di chuyển con trỏ sang phải
            if (caretPos < (int)text.length()) caretPos++;
            caretVisible = true;
            lastBlink = SDL_GetTicks();
        }
    }
}

void TextBox::render(SDL_Renderer* renderer) {
    // 1. Vẽ nền (Luôn vẽ nền đầy đủ)
    SDL_SetRenderDrawColor(renderer, bgColor.r, bgColor.g, bgColor.b, bgColor.a);
    SDL_RenderFillRect(renderer, &rect);

    // 2. Tính toán vị trí con trỏ (Caret Position) theo pixel
    int caretPixelX = 0;
    int h = 0;
    if (font) {
        string beforeCaret = text.substr(0, caretPos);
        // Dùng hàm đúng mà bạn đã tìm được (TTF_GetTextSize hoặc TTF_SizeText)
        TTF_Text* temptext = TTF_CreateText(0, font, beforeCaret.c_str(), beforeCaret.size());
        TTF_GetTextSize(temptext, &caretPixelX, &h);
        // Giải phóng tài nguyên tạm thời
        TTF_DestroyText(temptext);
    }

    // 3. LOGIC CUỘN (SCROLLING)
    float padding = 5.0f;
    float visibleWidth = rect.w - (padding * 2); // Chiều rộng khả dụng
    
    // Nếu con trỏ đi quá mép phải -> Đẩy offset lên để đuổi theo con trỏ
    if (caretPixelX - scrollOffset > visibleWidth) {
        scrollOffset = caretPixelX - visibleWidth;
    }
    // Nếu con trỏ đi quá mép trái (khi xóa hoặc bấm mũi tên trái) -> Kéo offset về
    if (caretPixelX < scrollOffset) {
        scrollOffset = (float)caretPixelX;
    }

    // Nếu văn bản ngắn hơn khung, reset về 0 để không bị hở bên trái
    if (texW < visibleWidth) {
        scrollOffset = 0;
    }

    // 4. THIẾT LẬP VÙNG CẮT (CLIPPING)
    // Chỉ cho phép vẽ bên trong hình chữ nhật của TextBox
    // (Phải ép kiểu về int vì SDL_SetRenderClipRect dùng int)
    SDL_Rect clipRect = {
        (int)rect.x, (int)rect.y,
        (int)rect.w, (int)rect.h
    };
    SDL_SetRenderClipRect(renderer, &clipRect);

    // 5. Vẽ Chữ
    float textY = rect.y + (rect.h - (texH > 0 ? texH : TTF_GetFontHeight(font))) / 2.0f;

    if(text.empty()) {
        // Vẽ Placeholder (Không cuộn)
        if (placeholderTexture) {
            float phW, phH;
            SDL_GetTextureSize(placeholderTexture, &phW, &phH);
            SDL_FRect dst = { rect.x + padding, rect.y + (rect.h - phH) / 2, phW, phH };
            SDL_RenderTexture(renderer, placeholderTexture, nullptr, &dst);
        }
    } 
    else {
        // Vẽ Text thật (ÁP DỤNG scrollOffset)
        if (textTexture) {
            SDL_FRect dst = { 
                rect.x + padding - scrollOffset, // Dịch chuyển X ngược lại
                textY, 
                (float)texW, (float)texH 
            };
            SDL_RenderTexture(renderer, textTexture, nullptr, &dst);
        }
    }

    // 6. Vẽ Con Trỏ (Caret) - (ÁP DỤNG scrollOffset)
    if(focused && caretVisible) {
        float caretHeight = (float)TTF_GetFontHeight(font);
        SDL_FRect caretRect = { 
            rect.x + padding + caretPixelX - scrollOffset,  // Vị trí X theo scroll
            rect.y + (rect.h - caretHeight) / 2.0f, 
            2.0f, 
            caretHeight
        };
        
        SDL_SetRenderDrawColor(renderer, textColor.r, textColor.g, textColor.b, textColor.a);
        SDL_RenderFillRect(renderer, &caretRect);
    }

    // 7. QUAN TRỌNG: Hủy vùng cắt để không ảnh hưởng các phần vẽ sau
    SDL_SetRenderClipRect(renderer, nullptr);
}