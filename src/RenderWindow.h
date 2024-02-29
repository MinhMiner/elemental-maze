#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#include "Entity.h"

class RenderWindow 
{
public:
	RenderWindow();
	RenderWindow(const char* p_title, int p_w, int p_h);
	SDL_Texture* loadTexture(const char* p_filePath);
	void cleanUp();
	void clear();
	void render(Entity& p_entity);
	void render(int x, int y, SDL_Texture* p_tex);
	void render(float p_x, float p_y, const char* p_text, TTF_Font* p_font, SDL_Color p_textColor);
	void display();
private:
	SDL_Window* window;
	SDL_Renderer* renderer;
};