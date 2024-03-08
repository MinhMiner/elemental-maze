#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <random>
#include <chrono>

#include "RenderWindow.h"
#include "Entity.h"
#include "Math.h"
#include "Map.h"
#include "Player.h"

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

const int FPS = 120;
double totalTime = 0.0;
int score = 0;
bool canSpawnBomb = false;

RenderWindow window("Bark 'n Bombs", WINDOW_WIDTH, WINDOW_HEIGHT);

SDL_Texture *background_Texture = window.loadTexture("res/gfx/background.png");
SDL_Texture *player_Texture = window.loadTexture("res/gfx/player.png");
SDL_Texture *brick_wall_Texture = window.loadTexture("res/gfx/brick_wall.png");
SDL_Texture *bomb_Texture = window.loadTexture("res/gfx/bomb.png");

TTF_Font* font32 = TTF_OpenFont("res/font/font.ttf", 32);
TTF_Font* font64 = TTF_OpenFont("res/font/font.ttf", 64);

SDL_Color white = {255, 255, 255};
SDL_Color black = {0, 0, 0};

int level = 0;
int state = 1;	// state 0: title screen; state 1: play; state 2: end game

bool gameRunning = true;

SDL_Event event;

Uint64 currentTick = SDL_GetPerformanceCounter();
Uint64 lastTick = 0;
double deltaTime = 0;

bool keyWPressed = false;
bool keyAPressed = false;
bool keySPressed = false;
bool keyDPressed = false;

void loadMaps(std::vector<Wall> &walls);
void loadLevels(int &level);

void game();
void titleScreen();
void update();
void graphics();

std::vector<Wall> walls;
std::vector<Bomb*> bombs;
Player player = Player({300, 300}, player_Texture);

int main(int argc, char* args[]) {
    std::cout << "Program started..." << std::endl;

    if (!init()) {
        std::cerr << "Initialization failed." << std::endl;
        return 1;
    }

    
    loadMaps(walls);
    

	while (gameRunning)
	{
		game();
        double targetFrameTime = 1000.0 / FPS;
        if (deltaTime < targetFrameTime) {
            Uint32 delayMilliseconds = (Uint32)(targetFrameTime - deltaTime);
            SDL_Delay(delayMilliseconds);
        }
	}

	window.cleanUp();

	SDL_Quit();
	TTF_Quit();
	Mix_CloseAudio();
	return 0;
}


void game() {
	if (state == 0)
	{
		titleScreen();
        totalTime = 0.0;
        score = 0;
	}
	else
	{
		update();
		graphics();
	}
}

void titleScreen() {
    while (SDL_PollEvent(&event))
    {
    	switch(event.type)
    	{
            case SDL_QUIT:
                gameRunning = false;
                break;
    	}
    }

    window.clear();
    window.render(0, 0, background_Texture);

    window.render(155, 155, "Bark 'n Bombs", font64, black);
	window.render(150, 150, "Bark 'n Bombs", font64, white);

    window.display();
}

void update() {
	lastTick = currentTick;
	currentTick = SDL_GetPerformanceCounter();
	deltaTime = (double) ((currentTick - lastTick)*1000 / (double) SDL_GetPerformanceFrequency());

    totalTime += deltaTime;
    score = ((int) (totalTime / 1000)) * 10;

    if (score % 20 == 0 && canSpawnBomb) {
        std::chrono::system_clock::time_point tp = std::chrono::system_clock::now();
        std::mt19937 gen(tp.time_since_epoch().count());

        std::uniform_real_distribution<float> distributionX(0, WINDOW_WIDTH - 300);
        std::uniform_real_distribution<float> distributionY(164, WINDOW_HEIGHT - 300);

        for (int i = 0; i < 5; i++)
        {
            
            float randomXVal = distributionX(gen);
            float randomYVal = distributionY(gen);

            Bomb *bomb = new Bomb({randomXVal, randomYVal}, bomb_Texture);
            bombs.push_back(bomb);
        }

        canSpawnBomb = false;
    }
    if (score % 20 != 0)
        canSpawnBomb = true;
    

	while (SDL_PollEvent(&event))
    {
    	switch(event.type)
    	{
            case SDL_QUIT:
                gameRunning = false;
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_w)
                    keyWPressed = true;
                else if (event.key.keysym.sym == SDLK_a)
                    keyAPressed = true;
                else if (event.key.keysym.sym == SDLK_s)
                    keySPressed = true;
                else if (event.key.keysym.sym == SDLK_d)
                    keyDPressed = true;
                break;
            case SDL_KEYUP:
                if (event.key.keysym.sym == SDLK_w)
                    keyWPressed = false;
                else if (event.key.keysym.sym == SDLK_a)
                    keyAPressed = false;
                else if (event.key.keysym.sym == SDLK_s)
                    keySPressed = false;
                else if (event.key.keysym.sym == SDLK_d)
                    keyDPressed = false;
                break;
    	}
    }

    for (auto it = bombs.begin(); it != bombs.end(); ) {
        (*it)->setAge(deltaTime);

        if ((*it)->shouldDestroy()) {
            delete *it;
            it = bombs.erase(it);  // Iterator is updated to the next valid position
        } else {
            ++it;
    }
}
    player.update(deltaTime, keyWPressed, keyDPressed, keySPressed, keyAPressed, walls);
}

void graphics() {
	if (state == 1) 
	{
		window.clear();
		window.render(0, 0, background_Texture);

        std::string scoreString = "Score: " + std::to_string(score);
        const char* scoreCStr = scoreString.c_str();

        window.render(15, 15, scoreCStr, font64, black);
	    window.render(10, 10, scoreCStr, font64, white);
		
        for (Wall &w: walls) {
            window.render(w);
        }
        for (Bomb* b: bombs) {
            window.render(*b);
        }

        window.render(player);
		
		window.display();
	}
}

void loadMaps(std::vector<Wall> &walls) {
    // Level 1
    // walls.push_back(Wall({0, 100}, brick_wall_Texture));
}

void loadLevel(int &level) {
    switch (level)
    {
    case 0:
        /* code */
        break;
    
    default:
        break;
    }
}