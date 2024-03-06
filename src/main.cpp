#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

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

RenderWindow window("Elemental Maze", 1280, 720);

SDL_Texture *background_Texture = window.loadTexture("res/gfx/background.png");
SDL_Texture *player_Texture = window.loadTexture("res/gfx/player.png");
SDL_Texture *brick_wall_Texture = window.loadTexture("res/gfx/brick_wall.png");
SDL_Texture *goal_Texture = window.loadTexture("res/gfx/goal.png");

TTF_Font* font32 = TTF_OpenFont("res/font/font.ttf", 32);
TTF_Font* font64 = TTF_OpenFont("res/font/font.ttf", 64);

SDL_Color white = {255, 255, 255};
SDL_Color black = {0, 0, 0};

int level = 0;
int state = 1;	// state 0: title screen; state 1: levels; state 2: end game

bool gameRunning = true;

SDL_Event event;

Uint64 currentTick = SDL_GetPerformanceCounter();
Uint64 lastTick = 0;
double deltaTime = 0;

bool keyWPressed = false;
bool keyAPressed = false;
bool keySPressed = false;
bool keyDPressed = false;

void loadMaps(std::vector<std::vector<std::vector<Area>>> &levels);
void loadLevels(int &level);

void game();
void titleScreen();
void update();
void graphics();

std::vector<std::vector<std::vector<Area>>> levels;
Player player = Player({32 * 4, 32 * 4}, player_Texture);

int main(int argc, char* args[]) {
    std::cout << "Program started..." << std::endl;

    if (!init()) {
        std::cerr << "Initialization failed." << std::endl;
        return 1;
    }

    
    loadMaps(levels);
    

	while (gameRunning)
	{
		game();
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

    window.render(155, 155, "Elemental Maze", font64, black);
	window.render(150, 150, "Elemental Maze", font64, white);

    window.display();
}

void update() {
	lastTick = currentTick;
	currentTick = SDL_GetPerformanceCounter();
	deltaTime = (double) ((currentTick - lastTick)*1000 / (double) SDL_GetPerformanceFrequency());

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

    player.update(deltaTime, keyWPressed, keyDPressed, keySPressed, keyAPressed, levels[level][0][0]);
}

void graphics() {
	if (state == 1) 
	{
		window.clear();
		window.render(0, 0, background_Texture);
		
		window.renderArea(levels[level][0][0]);
        window.render(player);
		
		window.display();
	}
}



void loadMaps(std::vector<std::vector<std::vector<Area>>> &levels) {
    // Level 1
    std::vector<std::vector<Area>> level1;
    level1.push_back({});   // First row of Areas
    level1[0].push_back(Area({      // Area [0][0]
        Wall({32 * 2, 32 * 2}, brick_wall_Texture), 
        Wall({32 * 3, 32 * 2}, brick_wall_Texture), 
        Wall({32 * 4, 32 * 2}, brick_wall_Texture), 
        Wall({32 * 5, 32 * 2}, brick_wall_Texture), 
        Wall({32 * 6, 32 * 2}, brick_wall_Texture), 
        Wall({32 * 7, 32 * 2}, brick_wall_Texture), 
        Wall({32 * 8, 32 * 2}, brick_wall_Texture), 
        Wall({32 * 9, 32 * 2}, brick_wall_Texture), 
        Wall({32 * 10, 32 * 2}, brick_wall_Texture), 
        Wall({32 * 11, 32 * 2}, brick_wall_Texture), 
        Wall({32 * 12, 32 * 2}, brick_wall_Texture), 
        Wall({32 * 13, 32 * 2}, brick_wall_Texture),
        Wall({32 * 14, 32 * 2}, brick_wall_Texture),
        Wall({32 * 15, 32 * 2}, brick_wall_Texture),
        Wall({32 * 16, 32 * 2}, brick_wall_Texture),
        Wall({32 * 17, 32 * 2}, brick_wall_Texture),
        Wall({32 * 18, 32 * 2}, brick_wall_Texture),
        Wall({32 * 19, 32 * 2}, brick_wall_Texture),
        Wall({32 * 20, 32 * 2}, brick_wall_Texture),
        Wall({32 * 21, 32 * 2}, brick_wall_Texture),
        Wall({32 * 22, 32 * 2}, brick_wall_Texture),
        Wall({32 * 22, 32 * 3}, brick_wall_Texture),
        Wall({32 * 22, 32 * 4}, brick_wall_Texture),
        Wall({32 * 22, 32 * 5}, brick_wall_Texture),
        Wall({32 * 22, 32 * 6}, brick_wall_Texture),
        Wall({32 * 22, 32 * 7}, brick_wall_Texture),
        Wall({32 * 22, 32 * 8}, brick_wall_Texture),
        Wall({32 * 22, 32 * 9}, brick_wall_Texture),
        Wall({32 * 22, 32 * 10}, brick_wall_Texture),
        Wall({32 * 22, 32 * 11}, brick_wall_Texture),
        Wall({32 * 22, 32 * 12}, brick_wall_Texture),
        Wall({32 * 22, 32 * 13}, brick_wall_Texture),
        Wall({32 * 22, 32 * 14}, brick_wall_Texture),
        Wall({32 * 22, 32 * 15}, brick_wall_Texture),
        Wall({32 * 22, 32 * 16}, brick_wall_Texture),
        Wall({32 * 22, 32 * 17}, brick_wall_Texture),
		Wall({32 * 2, 32 * 3}, brick_wall_Texture),
        Wall({32 * 2, 32 * 4}, brick_wall_Texture),
        Wall({32 * 2, 32 * 5}, brick_wall_Texture),
        Wall({32 * 2, 32 * 6}, brick_wall_Texture),
        Wall({32 * 2, 32 * 7}, brick_wall_Texture),
        Wall({32 * 2, 32 * 8}, brick_wall_Texture),
        Wall({32 * 2, 32 * 9}, brick_wall_Texture),
        Wall({32 * 2, 32 * 10}, brick_wall_Texture),
        Wall({32 * 2, 32 * 11}, brick_wall_Texture),
        Wall({32 * 2, 32 * 12}, brick_wall_Texture),
        Wall({32 * 2, 32 * 13}, brick_wall_Texture),
        Wall({32 * 2, 32 * 14}, brick_wall_Texture),
        Wall({32 * 2, 32 * 15}, brick_wall_Texture),
        Wall({32 * 2, 32 * 16}, brick_wall_Texture),
        Wall({32 * 2, 32 * 17}, brick_wall_Texture),	
        Wall({32 * 3, 32 * 17}, brick_wall_Texture), 
        Wall({32 * 4, 32 * 17}, brick_wall_Texture), 
        Wall({32 * 5, 32 * 17}, brick_wall_Texture), 
        Wall({32 * 6, 32 * 17}, brick_wall_Texture), 
        Wall({32 * 7, 32 * 17}, brick_wall_Texture), 
        Wall({32 * 8, 32 * 17}, brick_wall_Texture), 
        Wall({32 * 9, 32 * 17}, brick_wall_Texture), 
        Wall({32 * 10, 32 * 17}, brick_wall_Texture), 
        Wall({32 * 11, 32 * 17}, brick_wall_Texture), 
        Wall({32 * 12, 32 * 17}, brick_wall_Texture), 
        Wall({32 * 13, 32 * 17}, brick_wall_Texture),
        Wall({32 * 14, 32 * 17}, brick_wall_Texture),
        Wall({32 * 15, 32 * 17}, brick_wall_Texture),
        Wall({32 * 16, 32 * 17}, brick_wall_Texture),
        Wall({32 * 17, 32 * 17}, brick_wall_Texture),
        Wall({32 * 18, 32 * 17}, brick_wall_Texture),
        Wall({32 * 19, 32 * 17}, brick_wall_Texture),
        Wall({32 * 20, 32 * 17}, brick_wall_Texture),
        Wall({32 * 21, 32 * 17}, brick_wall_Texture),
    }, {
        
    }, true));
    

    levels.push_back(level1);
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