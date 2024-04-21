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
#include "Utils.h"
#include "Map.h"
#include "Player.h"
#include "Button.h"

bool init();

extern std::ofstream fout;

extern bool SDLinit;

extern const int FPS;

extern SDL_Color white;
extern SDL_Color black;

extern RenderWindow window;

extern SDL_Texture *title_screen_background_Texture;
extern SDL_Texture *background_Texture;
extern SDL_Texture *player_Texture;
extern SDL_Texture *brick_wall_Texture;
extern SDL_Texture *bomb_Texture;
extern SDL_Texture *bone_Texture;
extern SDL_Texture *energy_bar_Texture;
extern SDL_Texture *energy_bar_outline_Texture;
extern SDL_Texture *fish_Texture;
extern SDL_Texture *steak_Texture;
extern SDL_Texture *chicken_Texture;
extern SDL_Texture *gift_Texture;
extern SDL_Texture *speed_Texture;
extern SDL_Texture *dash_Texture;
extern SDL_Texture *shield_Texture;
extern SDL_Texture *start_Button_Texture;
extern SDL_Texture *play_again_Button_Texture;

extern TTF_Font* font32;
extern TTF_Font* font64;
extern TTF_Font* font128;

extern int level;
extern stateID state;

extern Player player;

extern bool gameRunning;
extern bool startPlaying;
extern bool startTitleScreen;

extern double deltaTime;
extern double totalTime;
extern int score;
extern int foodScore;
extern double lastBombSpawned;
extern double lastFoodSpawned;

extern double delayBetweenBombs;
extern double delayBetweenFoods;


extern inputKeys inputQueue;

void limitFPS();
void updateDeltatime();
void initState(stateID state);
void getInput();
void buttonEvents();
void loadMaps();
void spawnBomb(const double &delayBetweenBombs);
void spawnFood(const double &delayBetweenFoods);
void updateObjects();

void game();
void titleScreen();
void update();
void graphics();
void endScreen();

extern std::vector<Wall*> walls;
extern std::vector<Bomb*> bombs;
extern std::vector<Food*> foods;
extern std::vector<Button*> buttons;

extern std::chrono::system_clock::time_point tp;
extern std::mt19937 gen;

bool checkCollisions(float x, float y, float w, float h, std::vector<Wall*> &walls);

extern SDL_Event event;

extern Uint64 currentTick;
extern Uint64 lastTick;