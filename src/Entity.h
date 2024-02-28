#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Math.h"

class Entity
{
public:
	Entity();
	
	void setPos(float x, float y);

	SDL_Texture* getTex();
	void update(double deltaTime);
	SDL_Rect getCurrentFrame();

private:
	SDL_Rect currentFrame;
	SDL_Texture* tex;
};