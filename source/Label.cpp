#include <app/Label.h>

// Khởi tạo nhãn văn bản
Label::Label(const string& text, SDL_Color textColor,float x,float y,TTF_Font* font, SDL_Renderer* renderer) :text(text),textColor(textColor),x(x),y(y),font(font), textTexture(nullptr),texW(0),texH(0), renderer(renderer) {
    SDL_Surface* surf = TTF_RenderText_Solid(font, text.c_str(), text.size(), textColor);
    if (surf) {
        textTexture = SDL_CreateTextureFromSurface(renderer, surf);
        texW = surf->w;
        texH = surf->h;
        SDL_DestroySurface(surf);
    }
};
// Giải phóng texture của nhãn
Label::~Label() {
    if(textTexture) {
        SDL_DestroyTexture(textTexture);
        textTexture = nullptr;
    }
}

// Cập nhật nội dung nhãn
void Label::setText(const string& newText) {
    text = newText;
    if (textTexture) {
        SDL_DestroyTexture(textTexture);
        textTexture = nullptr;
    }

    SDL_Surface* surf = TTF_RenderText_Solid(font,text.c_str(),text.size(),textColor);
    if (surf) {
        textTexture = SDL_CreateTextureFromSurface(renderer, surf);
        texW = surf->w;
        texH = surf->h;
        SDL_DestroySurface(surf);
    }
}

// Vẽ nhãn lên màn hình
void Label::render(SDL_Renderer* renderer) const {
    if (textTexture) {
        SDL_FRect dst = { x, y, (float)texW, (float)texH };
        SDL_RenderTexture(renderer, textTexture, nullptr, &dst);
    }
}
