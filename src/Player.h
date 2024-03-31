#pragma once
#include "Entity.h"
#include "Map.h"

class Player : public Entity {
    public:
        Player(Vector2f p_pos, SDL_Texture *p_tex) : Entity(p_pos, p_tex)
        {
            setCurrentFrame(0, 0, 64, 40);
            energy = 15000;
            foodCollected = 0;
            speed = 0.25;
        }

        void update(double deltaTime, bool keyWPressed, bool keyDPressed, bool keySPressed, bool keyAPressed, std::vector<Wall*> &walls);
        bool checkCollisions(float x, float y, std::vector<Wall*> &walls);
        bool checkCollisions(float x, float y, std::vector<Bomb*> &bombs);
        bool checkCollisions(float x, float y, std::vector<Food*> &foods, Food* &returnFood);

        bool isDead();
        void setDead();
        void setAlive();
        bool isMovingLeft();
        double getEnergy();
        void updateEnergy(double deltaTime);
        double getMaxEnergy();
        void collectedFood();
        void resetFoodCount();
        int getFoodCount();

        std::vector<Effect> getEffects();
        double getEffectDuration(effectType p_effectName);
        double getEffectMaxDuration(effectType p_effectName);
        void addEffect(Effect p_effect);
        void removeEffect(effectType p_effectName);
        void setEffects(std::vector<Effect> p_effects);
        bool hasEffect(effectType p_effectName);
        void resetEffects();

    private:
        bool died;
        bool movingLeft;
        double lastTurn;
        double energy;
        int foodCollected;
        double speed;
        std::vector<Effect> effects;
};