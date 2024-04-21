#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Utils.h"



class Button {
    public:
        Button(Vector2f p_pos, SDL_Texture *p_tex, buttonType p_id):pos(p_pos), tex(p_tex), id(p_id) {
            hovered = false;
            clicked = false;

            currentFrame.x = 0;
            currentFrame.y = 0;

            SDL_QueryTexture(p_tex, NULL, NULL, &currentFrame.w, &currentFrame.h);

            currentFrame.h /= 2;
        }
        
        ~Button() {}
        Vector2f getPos() { return pos; }
        void setPos(Vector2f p_pos) { pos = p_pos; }
        SDL_Rect getCurrentFrame() { return currentFrame; }
        SDL_Texture* getTex() { return tex; }
        buttonType getType() { return id; }
        bool isHovered() { return hovered; }
        bool isClicked() { return clicked; }
        void update(const inputKeys &inputQueue);

    private:
        Vector2f pos;
        SDL_Texture *tex;
        SDL_Rect currentFrame;
        buttonType id;
        bool hovered;
        bool clicked;
};