#include <app/Button.h>

Button::Button(float x, float y, float w, float h, SDL_Color color,const string& text, SDL_Renderer* renderer, TTF_Font* f, SDL_Color tColor):rect{x,y,w,h},bgColor(color),label(text),hovered(false),pressed(false),wasClicked(false),font(f),textColor(tColor) ,renderer(renderer){
    SDL_Surface* surf = TTF_RenderText_Solid(font, label.c_str(), label.size(), textColor); 
    if (surf) { 
        textTexture = SDL_CreateTextureFromSurface(renderer, surf); 
        SDL_DestroySurface(surf); 
    }

}

Button::~Button() {
    if(textTexture) {
        SDL_DestroyTexture(textTexture);
        textTexture = nullptr;
    }
}

void Button::render(SDL_Renderer* renderer) {
    SDL_Color renderColor = bgColor;
    if (pressed) {
        renderColor = { (Uint8)(bgColor.r / 2), (Uint8)(bgColor.g / 2), (Uint8)(bgColor.b / 2), bgColor.a };
    } else if (hovered) {
        renderColor = { (Uint8)SDL_min(bgColor.r + 30, 255), 
                        (Uint8)SDL_min(bgColor.g + 30, 255), 
                        (Uint8)SDL_min(bgColor.b + 30, 255), 
                        bgColor.a };
    }
    
    SDL_SetRenderDrawColor(renderer, renderColor.r, renderColor.g, renderColor.b, renderColor.a);
    SDL_RenderFillRect(renderer,&rect);

    if (textTexture) {
        float texW, texH;
        SDL_GetTextureSize(textTexture, &texW, &texH);
        SDL_FRect textRect = {
            rect.x + (rect.w - texW) / 2.0f,
            rect.y + (rect.h - texH) / 2.0f,
            texW,
            texH
        };
        SDL_RenderTexture(renderer, textTexture, nullptr, &textRect);
    }
}

void Button::update() {
    wasClicked = false;

    float mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);

    hovered = isHovered(mouseX, mouseY);
    
    if (!hovered) {
        pressed = false;
    }
}

void Button::handleEvent(const SDL_Event& e) {
    if (hovered) {
        if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
            if (e.button.button == SDL_BUTTON_LEFT) {
                pressed = true;
                wasClicked = false;
            }
        }
        else if (e.type == SDL_EVENT_MOUSE_BUTTON_UP) {
            if (e.button.button == SDL_BUTTON_LEFT) {
                if (pressed) {
                    wasClicked = true;
                }
                pressed = false;
            }
        }
    }

    if (e.type == SDL_EVENT_MOUSE_MOTION) {
        if (!hovered) {
            pressed = false;
        }
    }
}

bool Button::isHovered(float mouseX, float mouseY) const {
    return mouseX >= rect.x && mouseX <= rect.x + rect.w &&
           mouseY >= rect.y && mouseY <= rect.y + rect.h;
}

bool Button::isClicked() const {
    return wasClicked;
}

void Button::setLabel(const string& text) {
    label = text;

    if (textTexture) {
        SDL_DestroyTexture(textTexture);
        textTexture = nullptr;
    }
    
    if (renderer && font) {
        SDL_Surface* surf = TTF_RenderText_Solid(font,label.c_str(),label.size(),textColor);
        if (surf) {
            textTexture = SDL_CreateTextureFromSurface(renderer, surf);
            SDL_DestroySurface(surf);
        }
    }
}

void Button::setColor(SDL_Color color) {
    bgColor = color;
}

string Button::getLabel() const {
    return label;
}
