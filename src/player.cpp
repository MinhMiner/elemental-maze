#include "Player.h"
#include "Map.h"
#include "Math.h"
#include "RenderWindow.h"

void Player::update(double deltaTime, bool keyWPressed, bool keyDPressed, bool keySPressed, bool keyAPressed, std::vector<Wall> &walls)
{
    double speed = 0.25;

    setVelocity(0, 0);

    if (keyWPressed)
        setVelocity(getVelocity().x, - speed * deltaTime);
    if (keyDPressed)
        setVelocity(speed * deltaTime, getVelocity().y);
    if (keySPressed)
        setVelocity(getVelocity().x, speed * deltaTime);
    if (keyAPressed)
        setVelocity(- speed * deltaTime, getVelocity().y);

    // if (!checkCollisions(getPos().x + getVelocity().x, getPos().y + getVelocity().y, walls))
    //     setPos(getPos().x + getVelocity().x, getPos().y + getVelocity().y);

    if (!checkCollisions(getPos().x + getVelocity().x, getPos().y, walls))
        setPos(getPos().x + getVelocity().x, getPos().y);
    if (!checkCollisions(getPos().x, getPos().y + getVelocity().y, walls))
        setPos(getPos().x, getPos().y + getVelocity().y);

    if (getPos().x < 0)
        setPos(0, getPos().y);
    if (getPos().y < 164)
        setPos(getPos().x, 164);
    if (getPos().x + getCurrentFrame().w > WINDOW_WIDTH)
        setPos(WINDOW_WIDTH - getCurrentFrame().w, getPos().y);
    if (getPos().y + getCurrentFrame().h > WINDOW_HEIGHT)
        setPos(getPos().x, WINDOW_HEIGHT - getCurrentFrame().h);
}

bool Player::checkCollisions(float x, float y, std::vector<Wall> &walls)
{
    SDL_Rect dest;
    dest.x = x;
    dest.y = y;
    SDL_QueryTexture(getTex(), NULL, NULL, &dest.w, &dest.h);

    bool collision = false;
    for (Wall &w: walls)
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