#pragma once
#include "Entity.h"
#include <vector>


class Wall : public Entity {
    public:
        Wall(Vector2f p_pos, SDL_Texture *p_tex) : Entity(p_pos, p_tex)
        {}

        bool checkCollisions(Entity &p_Entity) {
            SDL_Rect temp = p_Entity.getCurrentFrame();
            return SDL_HasIntersection(&temp, &currentFrame);
        }

    private:
        // Vector2f pos;
        // SDL_Texture *tex;
        SDL_Rect currentFrame;
};

