#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Math.h"

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
		const double maxAge = 1000.0;
};