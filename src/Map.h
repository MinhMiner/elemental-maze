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

class Area {
    public:
        Area(std::vector<Wall> p_Walls, std::vector<Entity> p_Entities, bool p_valid)
        :Walls(p_Walls), Entities(p_Entities), valid(p_valid)
        {}
        bool isValid() {
            return valid;
        }
        bool checkCollisions(Entity &p_Entity) {
            bool collision = false;
            for (auto &it: Walls)
            {
                if (it.checkCollisions(p_Entity)) {
                    collision = true;
                    break;
                }
            }
            return collision;
        }
        std::vector<Wall> getWalls() {
            return Walls;
        }
        std::vector<Entity> getEntities() {
            return Entities;
        }

    private:
        std::vector<Wall> Walls;
        std::vector<Entity> Entities;
        bool valid;
};

