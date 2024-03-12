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

void Entity::setVelocity(float x, float y)
{
    velocity.x = x;
    velocity.y = y;
}

Vector2f Entity::getVelocity()
{
    return velocity;
}

SDL_Texture* Entity::getTex()
{
    return tex;
}

void Entity::update(double deltaTime)
{
    pos.x += velocity.x * deltaTime;
    pos.y += velocity.y * deltaTime;
}

SDL_Rect Entity::getCurrentFrame()
{
    return currentFrame;
}

void Entity::setCurrentFrame(int x, int y, int w, int h)
{
    currentFrame.x = x;
    currentFrame.y = y;
    currentFrame.w = w;
    currentFrame.h = h;
}

double Bomb::getAge()
{
    return age;
}

void Bomb::setAge(double &deltaTime)
{
    age += deltaTime;
    if (maxAgeReached())
        explode();
}

void Bomb::explode()
{
    const int frameInterval = 50;
    const int maxFrames = 11;

    int frameIndex = (int) ((age - maxAge) / frameInterval);
    frameIndex = std::min(frameIndex, maxFrames);

    int frameX = frameIndex * 300;
    setCurrentFrame(frameX, 0, getCurrentFrame().w, getCurrentFrame().h);

    destroyed = (age >= maxAge + frameInterval * (maxFrames - 1));
}

bool Bomb::maxAgeReached()
{
    return (age >= maxAge);
}

bool Bomb::shouldDestroy()
{
    return destroyed;
}

foodType Food::getFoodType()
{
    return type;
}

double Food::getAge()
{
    return age;
}

void Food::setAge(double deltaTime)
{
    age += deltaTime;
    if (maxAgeReached())
        destroyed = true;
}

bool Food::maxAgeReached()
{
    return (age >= maxAge);
}

bool Food::shouldDestroy()
{
    return destroyed;
}