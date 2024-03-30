#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#include "Entity.h"
#include "Player.h"
#include "Map.h"

const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;

class RenderWindow 
{
public:
	RenderWindow();
	RenderWindow(const char* p_title, int p_w, int p_h);
	SDL_Texture* loadTexture(const char* p_filePath);
	void cleanUp();
	void clear();
	void render(Entity& p_entity);
	void render(Player& p_player, bool movingLeft);
	void render(float p_x, float p_y, SDL_Texture* p_tex);
	void render(float p_x, float p_y, SDL_Texture* p_tex, double percent_w);
	void render(float p_x, float p_y, const char* p_text, TTF_Font* p_font, SDL_Color p_textColor);
	void display();
private:
	SDL_Window* window;
	SDL_Renderer* renderer;
};