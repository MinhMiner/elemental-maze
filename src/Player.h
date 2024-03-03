#pragma once
#include "Entity.h"

class Player : public Entity {
    public:
        Player(Vector2f p_pos, SDL_Texture *p_tex) : Entity(p_pos, p_tex)
        {}

    private:
        Vector2f pos;
        SDL_Texture *tex;
        SDL_Rect currentFrame;
};