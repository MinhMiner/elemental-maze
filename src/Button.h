#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Math.h"

class Button {
    public:
        Button(Vector2f p_pos, SDL_Texture *p_tex):pos(p_pos), tex(p_tex) {
            is_hovered = false;
            is_clicked = false;
        }
        ~Button() {}

    private:
        Vector2f pos;
        SDL_Texture *tex;
        bool is_hovered;
        bool is_clicked;
};