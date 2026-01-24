#include <app/TextBox.h>
#include <algorithm>
#include <cstring>

// Khởi tạo ô nhập văn bản
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

// Giải phóng tài nguyên texture
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

// Cập nhật texture khi nội dung thay đổi
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

// Cập nhật nhấp nháy con trỏ và texture
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

// Xử lý sự kiện chuột và bàn phím
void TextBox::handleEvent(const SDL_Event& e) {
    if(e.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
        int mx = e.button.x, my = e.button.y;
        bool wasFocused = focused;

        SDL_FPoint mousePoint = { (float)mx, (float)my };
        focused = SDL_PointInRectFloat(&mousePoint, &rect);

        if (focused) {
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
            text.erase(caretPos - 1, 1);
            caretPos--;
            needsTextureUpdate = true;
        }
        else if (e.key.key == SDLK_LEFT) {
            if (caretPos > 0) caretPos--;
            caretVisible = true; 
            lastBlink = SDL_GetTicks();
        }
        else if (e.key.key == SDLK_RIGHT) {
            if (caretPos < (int)text.length()) caretPos++;
            caretVisible = true;
            lastBlink = SDL_GetTicks();
        }
    }
}

// Vẽ ô nhập và con trỏ
void TextBox::render(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, bgColor.r, bgColor.g, bgColor.b, bgColor.a);
    SDL_RenderFillRect(renderer, &rect);

    int caretPixelX = 0;
    int h = 0;
    if (font) {
        string beforeCaret = text.substr(0, caretPos);
        TTF_Text* temptext = TTF_CreateText(0, font, beforeCaret.c_str(), beforeCaret.size());
        TTF_GetTextSize(temptext, &caretPixelX, &h);
        TTF_DestroyText(temptext);
    }

    float padding = 5.0f;
    float visibleWidth = rect.w - (padding * 2);
    
    if (caretPixelX - scrollOffset > visibleWidth) {
        scrollOffset = caretPixelX - visibleWidth;
    }
    if (caretPixelX < scrollOffset) {
        scrollOffset = (float)caretPixelX;
    }

    if (texW < visibleWidth) {
        scrollOffset = 0;
    }

    SDL_Rect clipRect = {
        (int)rect.x, (int)rect.y,
        (int)rect.w, (int)rect.h
    };
    SDL_SetRenderClipRect(renderer, &clipRect);

    float textY = rect.y + (rect.h - (texH > 0 ? texH : TTF_GetFontHeight(font))) / 2.0f;

    if(text.empty()) {
        if (placeholderTexture) {
            float phW, phH;
            SDL_GetTextureSize(placeholderTexture, &phW, &phH);
            SDL_FRect dst = { rect.x + padding, rect.y + (rect.h - phH) / 2, phW, phH };
            SDL_RenderTexture(renderer, placeholderTexture, nullptr, &dst);
        }
    } 
    else {
        if (textTexture) {
            SDL_FRect dst = { 
                rect.x + padding - scrollOffset,
                textY, 
                (float)texW, (float)texH 
            };
            SDL_RenderTexture(renderer, textTexture, nullptr, &dst);
        }
    }

    if(focused && caretVisible) {
        float caretHeight = (float)TTF_GetFontHeight(font);
        SDL_FRect caretRect = { 
            rect.x + padding + caretPixelX - scrollOffset,
            rect.y + (rect.h - caretHeight) / 2.0f, 
            2.0f, 
            caretHeight
        };
        
        SDL_SetRenderDrawColor(renderer, textColor.r, textColor.g, textColor.b, textColor.a);
        SDL_RenderFillRect(renderer, &caretRect);
    }

    SDL_SetRenderClipRect(renderer, nullptr);
}
