#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Math.h"

class Entity
{
public:
	Entity();
	Entity(Vector2f p_pos, SDL_Texture *p_tex);
	
	void setPos(float x, float y);
	Vector2f getPos();

	SDL_Texture* getTex();
	void update(double deltaTime);
	SDL_Rect getCurrentFrame();

private:
	Vector2f pos;
	SDL_Rect currentFrame;
	SDL_Texture* tex;
};