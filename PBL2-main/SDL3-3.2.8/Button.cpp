#include "Button.h"

Button::Button(float x, float y, float w, float h, SDL_Color color,string text, SDL_Renderer* renderer, TTF_Font* f, SDL_Color tColor):rect{x,y,w,h},bgColor(color),label(text),font(f),textColor(tColor) {
    SDL_Surface* surf = TTF_RenderText_Solid(font, label.c_str(), label.size(), textColor);
    textTexture = SDL_CreateTextureFromSurface(renderer, surf);
    SDL_DestroySurface(surf);
}

Button::~Button() {
    if(textTexture) {
        SDL_DestroyTexture(textTexture);
        textTexture = nullptr;
    }
}

void Button::render(SDL_Renderer* renderer) {
    SDL_Color renderColor = hovered ? SDL_Color{ bgColor.r + 30, bgColor.g + 30, bgColor.b + 30, bgColor.a } : bgColor;
    SDL_SetRenderDrawColor(renderer, renderColor.r, renderColor.g, renderColor.b, renderColor.a);
    SDL_RenderFillRect(renderer, &rect);

    if (textTexture) {
        float texW, texH;
        SDL_GetTextureSize(textTexture, &texW, &texH);
        SDL_FRect textRect = {
            rect.x + (rect.w - texW) / 2.0f,
            rect.y + (rect.h - texH) / 2.0f,
            static_cast<float>(texW),
            static_cast<float>(texH)
        };
        SDL_RenderTexture(renderer, textTexture, nullptr, &textRect);
    }
}
void Button::handleEvent(const SDL_Event& e) {
    if (e.type == SDL_EVENT_MOUSE_MOTION || 
        e.type == SDL_EVENT_MOUSE_BUTTON_DOWN || 
        e.type == SDL_EVENT_MOUSE_BUTTON_UP) {
            int mouseX = e.motion.x;
            int mouseY = e.motion.y;
            hovered = isHovered(mouseX,mouseY);
            if(hovered && e.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
                pressed = true;
            } else if (hovered && e.type == SDL_EVENT_MOUSE_BUTTON_UP) {
                pressed = false;
            }
            
        }
}

bool Button::isHovered(int mouseX, int mouseY) const {
    return mouseX >= rect.x && mouseX <= rect.x + rect.w &&
           mouseY >= rect.y && mouseY <= rect.y + rect.h;
}

bool Button::isClicked(int mouseX, int mouseY) const {
    return isHovered(mouseX, mouseY) && pressed;
}

void Button::setLabel(const string& text) {
    label = text;
    if (textTexture) {
        SDL_DestroyTexture(textTexture);
        textTexture = nullptr;
    }
}

void Button::setColor(SDL_Color color) {
    bgColor = color;
}

string Button::getLabel() const {
    return label;
}
