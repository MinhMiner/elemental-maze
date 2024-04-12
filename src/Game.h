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

enum stateID {
    TITLE_SCREEN,
    PLAY_SCREEN,
    END_SCREEN
};

int level = 1;
stateID state = TITLE_SCREEN;

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