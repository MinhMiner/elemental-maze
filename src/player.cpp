#include "Player.h"
#include "Math.h"

void Player::update(double deltaTime, bool keyWPressed, bool keyDPressed, bool keySPressed, bool keyAPressed)
{
    double speed = 0.5;

    setVelocity(0, 0);

    if (keyWPressed)
        setVelocity(getVelocity().x, - speed * deltaTime);
    if (keyDPressed)
        setVelocity(speed * deltaTime, getVelocity().y);
    if (keySPressed)
        setVelocity(getVelocity().x, speed * deltaTime);
    if (keyAPressed)
        setVelocity(- speed * deltaTime, getVelocity().y);

    setPos(getPos().x + getVelocity().x, getPos().y + getVelocity().y);
}