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
extern SDL_Texture *select_map_Button_Texture;
extern SDL_Texture *home_Button_Texture;
extern SDL_Texture *back_Button_Texture;
extern SDL_Texture *reset_Button_Texture;
extern SDL_Texture *pause_Button_Texture;
extern SDL_Texture *best_score_Button_Texture;
extern SDL_Texture *pause_screen_overlay_Texture;
extern SDL_Texture *best_score_background_Texture;
extern SDL_Texture *map_1_Texture;
extern SDL_Texture *map_2_Texture;
extern SDL_Texture *map_3_Texture;
extern SDL_Texture *map_4_Texture;

extern Mix_Chunk* button_click_Sfx;
extern Mix_Chunk* eat_bone_Sfx;
extern Mix_Chunk* eat_meat_Sfx;
extern Mix_Chunk* gift_Sfx;
extern Mix_Chunk* dash_Sfx;
extern Mix_Chunk* shield_Sfx;
extern Mix_Chunk* bgm_Sfx;

extern TTF_Font* font32;
extern TTF_Font* font64;
extern TTF_Font* font128;

extern int level;
extern stateID state;

extern Player player;

extern bool gameRunning;
extern bool startPlaying;
extern bool startTitleScreen;
extern bool startEndScreen;
extern bool startSelectMapScreen;
extern bool startBestScoreScreen;
extern bool newRecord;

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
void playerCollectFoodEvent();
void checkPlayerGetBombed();

void loadBestScores();
void saveBestScores();
void game();
void titleScreen();
void playScreen();
void pauseScreen();
void selectMapScreen();
void bestScoreScreen();
void graphics();
void endScreen();

extern std::vector<Wall*> walls;
extern std::vector<Bomb*> bombs;
extern std::vector<Food*> foods;
extern std::vector<Button*> buttons;

extern int bestScores[4];

extern std::chrono::system_clock::time_point tp;
extern std::mt19937 gen;

bool checkCollisions(float x, float y, float w, float h, std::vector<Wall*> &walls);

extern SDL_Event event;

extern Uint64 currentTick;
extern Uint64 lastTick;