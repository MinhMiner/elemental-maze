#include "Player.h"
#include "Map.h"
#include "Math.h"

void Player::update(double deltaTime, bool keyWPressed, bool keyDPressed, bool keySPressed, bool keyAPressed, Area area)
{
    double speed = 0.2;

    setVelocity(0, 0);

    if (keyWPressed)
        setVelocity(getVelocity().x, - speed * deltaTime);
    if (keyDPressed)
        setVelocity(speed * deltaTime, getVelocity().y);
    if (keySPressed)
        setVelocity(getVelocity().x, speed * deltaTime);
    if (keyAPressed)
        setVelocity(- speed * deltaTime, getVelocity().y);

    if (!checkCollisions(getPos().x + getVelocity().x, getPos().y + getVelocity().y, area))
        setPos(getPos().x + getVelocity().x, getPos().y + getVelocity().y);

}

bool Player::checkCollisions(float x, float y, Area area)
{
    SDL_Rect dest;
    dest.x = x;
    dest.y = y;
    SDL_QueryTexture(getTex(), NULL, NULL, &dest.w, &dest.h);

    bool collision = false;
    for (Wall &w: area.getWalls())
    {
        SDL_Rect temp;
        temp.x = w.getPos().x;
        temp.y = w.getPos().y;
        SDL_QueryTexture(w.getTex(), NULL, NULL, &temp.w, &temp.h);

        if (SDL_HasIntersection(&temp, &dest)) {
            collision = true;
            break;
        }
    }
    return collision;
}