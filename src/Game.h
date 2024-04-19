#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <random>
#include <chrono>
#include <cmath>

#include <fstream>

#include "RenderWindow.h"
#include "Entity.h"
#include "Math.h"
#include "Map.h"
#include "Player.h"
#include "Button.h"

#include "Config.h"

enum stateID {
    TITLE_SCREEN,
    PLAY_SCREEN,
    END_SCREEN
};

bool init()
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		std::cout << "SDL_Init has failed. Error: " << SDL_GetError() << std::endl;
		return false;
	}	

	if (IMG_Init(IMG_INIT_PNG) == 0) {
		std::cout << "IMG_init has failed. Error: " << SDL_GetError() << std::endl;
		return false;
	}
		
	if (TTF_Init() != 0) {
		std::cout << "TTF_init has failed. Error: " << SDL_GetError() << std::endl;
		return false;
	}
		
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
	return true;
}

bool SDLinit = init();

RenderWindow window("Bark 'n Bombs", WINDOW_WIDTH, WINDOW_HEIGHT);

SDL_Texture *title_screen_background_Texture = window.loadTexture("res/gfx/title_screen_background.png");
SDL_Texture *background_Texture = window.loadTexture("res/gfx/background.png");
SDL_Texture *player_Texture = window.loadTexture("res/gfx/player.png");
SDL_Texture *brick_wall_Texture = window.loadTexture("res/gfx/brick_wall.png");
SDL_Texture *bomb_Texture = window.loadTexture("res/gfx/bomb.png");
SDL_Texture *bone_Texture = window.loadTexture("res/gfx/bone.png");
SDL_Texture *energy_bar_Texture = window.loadTexture("res/gfx/energy_bar.png");
SDL_Texture *energy_bar_outline_Texture = window.loadTexture("res/gfx/energy_bar_outline.png");
SDL_Texture *fish_Texture = window.loadTexture("res/gfx/fish.png");
SDL_Texture *steak_Texture = window.loadTexture("res/gfx/steak.png");
SDL_Texture *chicken_Texture = window.loadTexture("res/gfx/chicken.png");
SDL_Texture *gift_Texture = window.loadTexture("res/gfx/gift.png");
SDL_Texture *speed_Texture = window.loadTexture("res/gfx/speed.png");
SDL_Texture *dash_Texture = window.loadTexture("res/gfx/dash.png");
SDL_Texture *shield_Texture = window.loadTexture("res/gfx/shield.png");
SDL_Texture *start_Button_Texture = window.loadTexture("res/gfx/start_button.png");
SDL_Texture *play_again_Button_Texture = window.loadTexture("res/gfx/play_again_button.png");

TTF_Font* font32 = TTF_OpenFont("res/font/font.ttf", 32);
TTF_Font* font64 = TTF_OpenFont("res/font/font.ttf", 64);
TTF_Font* font128 = TTF_OpenFont("res/font/font.ttf", 128);

int level = 1;
stateID state = TITLE_SCREEN;

Player player = Player({200, 200}, player_Texture);

bool gameRunning = true;
bool startPlaying = false;
bool startTitleScreen = false;

double deltaTime = 0;
double totalTime = 0.0;
int score = 0;
int foodScore = 0;
double lastBombSpawned = 0;
double lastFoodSpawned = 0;

bool keyWPressed = false;
bool keyAPressed = false;
bool keySPressed = false;
bool keyDPressed = false;
bool keyMousePressed = false;

void loadMaps();

void game();
void titleScreen();
void update();
void graphics();
void endScreen();

std::vector<Wall*> walls;
std::vector<Bomb*> bombs;
std::vector<Food*> foods;
std::vector<Button*> buttons;

std::chrono::system_clock::time_point tp = std::chrono::system_clock::now();
std::mt19937 gen(tp.time_since_epoch().count());

bool checkCollisions(float x, float y, float w, float h, std::vector<Wall*> &walls);

SDL_Event event;

Uint64 currentTick = SDL_GetPerformanceCounter();
Uint64 lastTick = 0;