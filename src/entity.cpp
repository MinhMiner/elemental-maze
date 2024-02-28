#include "Entity.h"

Entity::Entity()
{}

Entity::Entity(Vector2f p_pos, SDL_Texture *p_tex):pos(p_pos), tex(p_tex)
{
    currentFrame.x = 0;
    currentFrame.y = 0;

    SDL_QueryTexture(p_tex, NULL, NULL, &currentFrame.w, &currentFrame.h);
}

void Entity::setPos(float x, float y)
{
    pos.x = x;
    pos.y = y;
}

Vector2f Entity::getPos()
{
    return pos;
}

SDL_Texture* Entity::getTex()
{
    return tex;
}

void Entity::update(double deltaTime)
{}

SDL_Rect Entity::getCurrentFrame()
{
    return currentFrame;
}