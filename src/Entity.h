#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <vector>
#include "Utils.h"

class Entity {
	public:
		Entity();
		Entity(Vector2f p_pos, SDL_Texture *p_tex);
		
		void setPos(float x, float y);
		Vector2f getPos();

		void setVelocity(float x, float y);
		Vector2f getVelocity();

		SDL_Texture* getTex();
		void update(double deltaTime);
		SDL_Rect getCurrentFrame();
		void setCurrentFrame(int x, int y, int w, int h);

	private:
		Vector2f pos;
		Vector2f velocity;
		SDL_Rect currentFrame;
		SDL_Texture* tex;
};

class Bomb : public Entity {
    public:
        Bomb(Vector2f p_pos, SDL_Texture *p_tex) : Entity(p_pos, p_tex)
        {
			age = 0;
			destroyed = false;
			setCurrentFrame(0, 0, 300, 300);
		}
		~Bomb() {};

        double getAge();
		void setAge(double &deltaTime);
		void explode();
		bool maxAgeReached();
		bool shouldDestroy();

    private:
		double age;
		bool destroyed;
		const double maxAge = 750.0;
};

class Food : public Entity {
	public:
        Food(Vector2f p_pos, SDL_Texture *p_tex, foodType p_type) : Entity(p_pos, p_tex)
        {
			type = p_type;
			age = 0;
			destroyed = false;
			if (p_type == FISH) {
				int variation = (int) (p_pos.x * p_pos.y) % 6;
				setCurrentFrame(variation * 48, 0, 48, 48);
			} else if (p_type == CHICKEN) {
				int variation = (int) (p_pos.x * p_pos.y) % 3;
				setCurrentFrame(variation * 45, 0, 45, 45);
			}
		}
		~Food() {};
		foodType getFoodType();
        double getAge();
		void setAge(double deltaTime);
		bool maxAgeReached();
		bool shouldDestroy();

	private:
		foodType type;
		double age;
		bool destroyed;
		const double maxAge = 7500.0;
};

