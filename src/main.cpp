#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include "RenderWindow.h"
#include "Entity.h"
#include "Math.h"
#include "Map.h"

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

RenderWindow window("Elemental Maze", 800, 640);

SDL_Texture *background_Texture = window.loadTexture("res/gfx/background.png");
SDL_Texture *player_Texture = window.loadTexture("res/gfx/player.png");
SDL_Texture *brick_wall_Texture = window.loadTexture("res/gfx/brick_wall.png");
SDL_Texture *goal_Texture = window.loadTexture("res/gfx/goal.png");

TTF_Font* font32 = TTF_OpenFont("res/font/font.ttf", 32);
TTF_Font* font64 = TTF_OpenFont("res/font/font.ttf", 64);

SDL_Color white = {255, 255, 255};
SDL_Color black = {0, 0, 0};

int level = 0;
int state = 1;

bool gameRunning = true;

SDL_Event event;

Uint64 currentTick = SDL_GetPerformanceCounter();
Uint64 lastTick = 0;
double deltaTime = 0;

void loadMaps(std::vector<std::vector<std::vector<Area>>> &levels);
void loadLevels(int &level);

void game();
void titleScreen();
void update();
void graphics();

std::vector<std::vector<std::vector<Area>>> levels;

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
	while (SDL_PollEvent(&event))
    {
    	switch(event.type)
    	{
            case SDL_QUIT:
                gameRunning = false;
                break;
    	}
    }
}

void graphics() {
	if (state == 1) 
	{
		window.clear();
		window.render(0, 0, background_Texture);
		for (size_t i = 0; i < levels[level].size(); ++i)	// Check each row of areas of each level
		{
			/* std::vector<std::vector<std::vector<Area>>> &levels; */
			for (size_t j = 0; j < levels[level][i].size(); j++)	// Check each area of each row
			{
				if (levels[level][i][j].isValid())	// Check if that area exists
				{
					for (Wall &w : levels[level][i][j].getWalls())	// Render all walls in that area
					{
						window.render(w);
					}
					
				}
				
			}
			
		}
		
		window.display();
	}
}



void loadMaps(std::vector<std::vector<std::vector<Area>>> &levels) {
    // Level 1
    std::vector<std::vector<Area>> level1;
    level1.push_back({});   // First row of Areas
    level1[0].push_back(Area({      // Area [0][0]
        Wall({64, 64}, brick_wall_Texture), 
        Wall({96, 64}, brick_wall_Texture), 
        Wall({128, 64}, brick_wall_Texture), 
        Wall({160, 64}, brick_wall_Texture), 
        Wall({192, 64}, brick_wall_Texture), 
        Wall({224, 64}, brick_wall_Texture), 
        Wall({256, 64}, brick_wall_Texture), 
        Wall({288, 64}, brick_wall_Texture), 
        Wall({320, 64}, brick_wall_Texture), 
        Wall({352, 64}, brick_wall_Texture), 
        Wall({384, 64}, brick_wall_Texture), 
        Wall({416, 64}, brick_wall_Texture)
    }, {
        // vector of Entities
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