#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Math.h"

class Button {
    public:
        Button(Vector2f p_pos, int p_w, int p_h, SDL_Texture *p_tex):pos(p_pos), tex(p_tex) {
            hovered = false;
            clicked = false;

            currentFrame.x = 0;
            currentFrame.y = 0;

            SDL_QueryTexture(p_tex, NULL, NULL, &currentFrame.w, &currentFrame.h);
        }
        ~Button() {}
        Vector2f getPos() { return pos; }
        void setPos(Vector2f p_pos) { pos = p_pos; }
        bool isHovered() { return isHovered; }
        bool isClicked() { return isClicked; }
        void update(const int &mouseX, const int &mouseY, const bool &keyMousePressed);

    private:
        Vector2f pos;
        SDL_Texture *tex;
        SDL_Rect currentFrame;
        bool hovered;
        bool clicked;
};