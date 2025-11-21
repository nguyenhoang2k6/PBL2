#include <app/TextBox.h>
#include <SDL3_ttf/SDL_ttf.h>

TextBox::TextBox(float x, float y, float w, float h, SDL_Color bgColor, SDL_Color textColor,App* app,TTF_Font* font,const string& placeholder, SDL_Color placeholderColor)
    : rect{x,y,w,h}, bgColor(bgColor),textColor(textColor),text(""), textTexture(nullptr), texW(0), texH(0), focused(false), app(app),renderer(app->getRenderer()),font(font), lastBlink(0), caretVisible(true),caretPos(0), placeholder(placeholder), placeholderColor(placeholderColor),placeholderTexture(nullptr),needsTextureUpdate(false){
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

        if (focused && !wasFocused) {
            SDL_StartTextInput(app->getWindow());
        } else if (!focused && wasFocused) {
            SDL_StopTextInput(app->getWindow());
        }
    }

    if(!focused) return;

    if (e.type == SDL_EVENT_TEXT_INPUT) {
        text += e.text.text;
        needsTextureUpdate = true;
    } else if (e.type == SDL_EVENT_KEY_DOWN) {
        if (e.key.key == SDLK_BACKSPACE && !text.empty()) {
            text.pop_back();
            needsTextureUpdate = true;
        }
        
    }
}

void TextBox::render(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, bgColor.r, bgColor.g, bgColor.b, bgColor.a);
    SDL_RenderFillRect(renderer, &rect);
    
    if(text.empty() && !focused) {
        if (placeholderTexture) {
            float phW, phH;
            SDL_GetTextureSize(placeholderTexture, &phW, &phH);
            SDL_FRect dst = { rect.x + 5, rect.y + (rect.h - phH) / 2, phW, phH };
            SDL_RenderTexture(renderer, placeholderTexture, nullptr, &dst);
        }
    }
    else {
        if (textTexture) {
            SDL_FRect dst = { rect.x + 5, rect.y + (rect.h - texH) / 2, (float)texW, (float)texH };
            SDL_RenderTexture(renderer, textTexture, nullptr, &dst);
        }
    }
    if(focused && caretVisible) {
        string beforeCaret = text.substr(0, caretPos);
        int w = 0, h = 0;
        
        if(!beforeCaret.empty() && font) {
            TTF_Text* text_object = TTF_CreateText(nullptr,font,beforeCaret.c_str(),beforeCaret.size());
            if (text_object) {
                TTF_GetTextSize(text_object, &w, &h);
                TTF_DestroyText(text_object);
            } else {
                std::cerr << "Không thể tạo TTF_Text: " << SDL_GetError() << std::endl;
            }
        }

        float caretHeight = TTF_GetFontHeight(font);
        
        SDL_FRect caretRect = { 
            rect.x + 5 + (float)w,
            rect.y + (rect.h - caretHeight) / 2.0f,
            2.0f,
            caretHeight
        };
        
        SDL_SetRenderDrawColor(renderer, textColor.r, textColor.g, textColor.b, textColor.a);
        SDL_RenderFillRect(renderer, &caretRect);
    }
}