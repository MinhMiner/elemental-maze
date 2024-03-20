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
            // speedDuration = 0;
        }

        void update(double deltaTime, bool keyWPressed, bool keyDPressed, bool keySPressed, bool keyAPressed, std::vector<Wall> &walls);
        bool checkCollisions(float x, float y, std::vector<Wall> &walls);
        bool checkCollisions(float x, float y, std::vector<Bomb*> &bombs);
        bool checkCollisions(float x, float y, std::vector<Food*> &foods, Food* &returnFood);
        // void setSpeed(double p_speed);
        // void setSpeedDuration(double p_duration);
        // void setShieldDuration(double p_duration);
        // void setDashDuration(double p_duration);
        // double getShieldDuration();
        double getMaxShieldDuration();
        double getMaxDashDuration();
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
        // bool hasShield();
        // bool isInvincible();

        std::vector<Effect> getEffects();
        double getEffectDuration(effectType p_effectName);
        void addEffect(Effect p_effect);
        void removeEffect(effectType p_effectName);
        void setEffects(std::vector<Effect> p_effects);
        bool hasEffect(effectType p_effectName);
        void resetEffects();
        // double getLastTurn();
        // void resetLastTurn();

    private:
        bool died;
        bool movingLeft;
        double lastTurn;
        double energy;
        // const double maxEnergy = 15000;
        int foodCollected;
        double speed;
        std::vector<Effect> effects;
        // double speedDuration;
        // double highestShieldDuration;
        // double highestDashDuration;
        // bool shield;
        // bool invincible;
        // double dashDuration;
        // Vector2f pos;
        // Vector2f velocity;
        // SDL_Texture *tex;
        // SDL_Rect currentFrame;
};