#include "Player.h"
#include "Map.h"
#include "Math.h"
#include "RenderWindow.h"

void Player::update(double deltaTime, bool keyWPressed, bool keyDPressed, bool keySPressed, bool keyAPressed, std::vector<Wall> &walls)
{
    double speed = 0.25;

    // if (getVelocity().x != 0 || getVelocity().y != 0)
    //     lastTurn += deltaTime;

    if (getVelocity().x > 0) {
        if (movingLeft == true) {
            movingLeft = false;
            lastTurn = 0;
        } else {
            lastTurn += deltaTime;
        }
    } else if (getVelocity().x < 0) {
        if (movingLeft == false) {
            movingLeft = true;
            lastTurn = 0;
        } else {
            lastTurn += deltaTime;
        }
    } else if (getVelocity().y != 0) {
        lastTurn += deltaTime;
    }

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

    const int frameInterval = 100;
    const int maxFrames = 6;

    int frameIndex = (int) (((int) lastTurn % (frameInterval * maxFrames)) / frameInterval);
    frameIndex = std::min(frameIndex, maxFrames);

    int frameX = frameIndex * 64;
    setCurrentFrame(frameX, 0, getCurrentFrame().w, getCurrentFrame().h);
}

bool Player::checkCollisions(float x, float y, std::vector<Wall> &walls)
{
    SDL_Rect dest;
    dest.x = x;
    dest.y = y + 8;
    dest.w = 64;
    dest.h = 32;

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

bool Player::checkCollisions(float x, float y, std::vector<Bomb*> &bombs)
{
    SDL_Rect dest;
    dest.x = x;
    dest.y = y;
    dest.w = 64;
    dest.h = 40;

    bool collision = false;
    for (auto &b: bombs)
    {
        if (b->getAge() < 950 || b->getAge() > 1000)
            continue;

        int outerSafeZone = 20;

        SDL_Rect temp;
        temp.x = b->getPos().x + outerSafeZone;
        temp.y = b->getPos().y + outerSafeZone;
        temp.w = b->getCurrentFrame().w - outerSafeZone * 2;
        temp.h = b->getCurrentFrame().h - outerSafeZone * 2;

        if (SDL_HasIntersection(&temp, &dest)) {
            collision = true;
            break;
        }
    }
    return collision;
}

bool Player::checkCollisions(float x, float y, std::vector<Food*> &foods, Food* &returnFood)
{
    SDL_Rect dest;
    dest.x = x;
    dest.y = y;
    dest.w = 64;
    dest.h = 40;

    bool collision = false;
    for (auto &f: foods)
    {
        // if (f->getAge() < 950 || f->getAge() > 1000)
        //     continue;

        SDL_Rect temp;
        temp.x = f->getPos().x;
        temp.y = f->getPos().y;
        temp.w = f->getCurrentFrame().w;
        temp.h = f->getCurrentFrame().h;

        if (SDL_HasIntersection(&temp, &dest)) {
            collision = true;
            returnFood = f;
            break;
        }
    }
    return collision;
}

bool Player::isDead()
{
    return died;
}

void Player::setDead()
{
    died = true;
}

void Player::setAlive()
{
    died = false;
}

bool Player::isMovingLeft()
{
    return movingLeft;
}

double Player::getEnergy()
{
    return energy;
}

void Player::setEnergy(double deltaTime)
{
    energy -= deltaTime;
    if (energy > maxEnergy)
        energy = maxEnergy;
    if (energy < 0)
        energy = 0;
}

double Player::getMaxEnergy()
{
    return maxEnergy;
}

// double Player::getLastTurn()
// {
//     return lastTurn;
// }

// void Player::resetLastTurn()
// {
//     lastTurn = 0;
// }