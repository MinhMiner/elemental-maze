#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <random>
#include <chrono>

#include <fstream>

#include "RenderWindow.h"
#include "Entity.h"
#include "Math.h"
#include "Map.h"
#include "Player.h"

std::ofstream fout("console.log");

bool init()
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		fout << "SDL_Init has failed. Error: " << SDL_GetError() << std::endl;
		return false;
	}	

	if (IMG_Init(IMG_INIT_PNG) == 0) {
		fout << "IMG_init has failed. Error: " << SDL_GetError() << std::endl;
		return false;
	}
		
	if (TTF_Init() != 0) {
		fout << "TTF_init has failed. Error: " << SDL_GetError() << std::endl;
		return false;
	}
		
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
	return true;
}

bool SDLinit = init();

const int FPS = 120;
double totalTime = 0.0;
int score = 0;
int foodScore = 0;
double lastBombSpawned = 0;
double lastFoodSpawned = 0;

RenderWindow window("Bark 'n Bombs", WINDOW_WIDTH, WINDOW_HEIGHT);

SDL_Texture *background_Texture = window.loadTexture("res/gfx/background.png");
SDL_Texture *player_Texture = window.loadTexture("res/gfx/player.png");
SDL_Texture *brick_wall_Texture = window.loadTexture("res/gfx/brick_wall.png");
SDL_Texture *bomb_Texture = window.loadTexture("res/gfx/bomb.png");
SDL_Texture *bone_Texture = window.loadTexture("res/gfx/bone.png");
SDL_Texture *energy_bar_Texture = window.loadTexture("res/gfx/energy_bar.png");
SDL_Texture *energy_bar_outline_Texture = window.loadTexture("res/gfx/energy_bar_outline.png");

TTF_Font* font32 = TTF_OpenFont("res/font/font.ttf", 32);
TTF_Font* font64 = TTF_OpenFont("res/font/font.ttf", 64);
TTF_Font* font128 = TTF_OpenFont("res/font/font.ttf", 128);

SDL_Color white = {255, 255, 255};
SDL_Color black = {0, 0, 0};

int level = 0;
int state = 0;	// state 0: title screen; state 1: play; state 2: end game

bool gameRunning = true;
bool startPlaying = false;

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
void endScreen();

std::vector<Wall> walls;
std::vector<Bomb*> bombs;
std::vector<Food*> foods;
Player player = Player({300, 300}, player_Texture);

int main(int argc, char* args[]) {
    
    fout << "Game started..." << std::endl;

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

    fout << "Closed game successfully\n";
    fout.close();
	return 0;
}


void game() {
	if (state == 0)
	{
		titleScreen();
	}
	else if (state == 1)
	{
		update();
		graphics();
	} 
    else 
    {
        endScreen();
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
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT) {
                    // int mouseX = event.button.x;
                    // int mouseY = event.button.y;

                    state = 1;
                }
                break;
            default:
                break;
    	}
    }

    window.clear();
    window.render(0, 0, background_Texture);

    window.render(305, 305, "Bark 'n Bombs", font128, black);
	window.render(300, 300, "Bark 'n Bombs", font128, white);

    window.render(455, 435, "Click to start", font64, white);
	window.render(450, 430, "Click to start", font64, black);

    

    window.display();
}

void update() {
	lastTick = currentTick;
	currentTick = SDL_GetPerformanceCounter();
	deltaTime = (double) ((currentTick - lastTick)*1000 / (double) SDL_GetPerformanceFrequency());

    totalTime += deltaTime;


    if (!startPlaying) {    // Reset điểm và thời gian lúc mới bắt đầu trò chơi
        fout << "START PLAYING-----------------------\n\n";
        player.setAlive();
        totalTime = 0.0;
        lastBombSpawned = 0.0;
        lastFoodSpawned = 0.0;
        score = 0;
        foodScore = 0;
        player.setEnergy(-20000);
        player.resetFoodCount();
        startPlaying = true;
    }

    score = ((int) (totalTime / 1000)) * 10 + foodScore;

    // fout << "bombs.size() = " << bombs.size() << '\n';
    // fout << "totalTime = " << totalTime << '\n';
    // fout << "lastBombSpawned = " << lastBombSpawned << '\n';



    if ((totalTime - lastBombSpawned) >= 300) {
        std::chrono::system_clock::time_point tp = std::chrono::system_clock::now();
        std::mt19937 gen(tp.time_since_epoch().count());

        std::uniform_real_distribution<float> distributionX(0, WINDOW_WIDTH - 200);
        std::uniform_real_distribution<float> distributionY(164, WINDOW_HEIGHT - 200);


        float randomXVal = distributionX(gen);
        float randomYVal = distributionY(gen);

        Bomb *bomb = new Bomb({randomXVal, randomYVal}, bomb_Texture);
        bombs.push_back(bomb);

        lastBombSpawned = totalTime;
    }
    if ((totalTime - lastFoodSpawned) >= 3000) {
        std::chrono::system_clock::time_point tp = std::chrono::system_clock::now();
        std::mt19937 gen(tp.time_since_epoch().count());

        std::uniform_real_distribution<float> distributionX(0, WINDOW_WIDTH - 64);
        std::uniform_real_distribution<float> distributionY(164, WINDOW_HEIGHT - 64);


        float randomXVal = distributionX(gen);
        float randomYVal = distributionY(gen);

        Food *food = new Food({randomXVal, randomYVal}, bone_Texture, BONE);
        foods.push_back(food);

        fout << "foods.size() = " << foods.size() << '\n';
        fout << "totalTime = " << totalTime << '\n';
        fout << "lastFoodSpawned = " << lastFoodSpawned << "\n\n";

        lastFoodSpawned = totalTime;
    }
    

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
            default:
                break;
    	}
    }

    for (auto it = bombs.begin(); it != bombs.end(); ) {
        (*it)->setAge(deltaTime);

        if ((*it)->shouldDestroy()) {
            delete *it;
            it = bombs.erase(it);
        } else {
            ++it;
        }
    }

    for (auto it = foods.begin(); it != foods.end(); ) {
        (*it)->setAge(deltaTime);

        if ((*it)->shouldDestroy()) {
            delete *it;
            it = foods.erase(it);
        } else {
            ++it;
        }
    }

    Food *foodCollected = nullptr;
    if (player.checkCollisions(player.getPos().x, player.getPos().y, foods, foodCollected)) {
        // fout << "You ate a food!" << '\n';
        // std::cout << "You ate a food!" << '\n';
        foodScore += 10;
        player.setEnergy(-7000);
        player.collectedFood();
        foodCollected->setAge(15000);
    }

    player.setEnergy(deltaTime);

    if (player.checkCollisions(player.getPos().x, player.getPos().y, bombs) || player.getEnergy() <= 0) {
        player.setDead();
        state = 2;
        fout << "You died" << '\n';
        std::cout << "You died" << '\n';
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

        window.render(700, 25, energy_bar_Texture, player.getEnergy() * 1.0/player.getMaxEnergy());
        window.render(700, 25, energy_bar_outline_Texture);
		
        for (Wall &w: walls) {
            window.render(w);
        }
        for (Bomb* b: bombs) {
            window.render(*b);
        }
        for (Food* f: foods) {
            window.render(*f);
        }

        window.render(player, player.isMovingLeft());
		
		window.display();
	} 
    else if (state == 2)
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
        
        // window.render(player, player.isMovingLeft());

        std::string foodCountString = "Food collected: " + std::to_string(player.getFoodCount());
        const char* foodCountCStr = foodCountString.c_str();

        window.render(405, 205, foodCountCStr, font64, white);
	    window.render(400, 200, foodCountCStr, font64, black);

        window.render(405, 305, "Game Over!", font128, black);
	    window.render(400, 300, "Game Over!", font128, white);

        window.render(455, 435, "Click to retry!", font64, white);
	    window.render(450, 430, "Click to retry!", font64, black);


        window.display();
    }
}

void endScreen() {
    if (startPlaying == true) {
        for (auto it = bombs.begin(); it != bombs.end(); ) {
            delete *it;
            it = bombs.erase(it);
            // fout << "success deleted bomb" << '\n';
        }
        bombs.clear();
        // std::cout << "deleted all bombs" << '\n';

        for (auto it = foods.begin(); it != foods.end(); ) {
            delete *it;
            it = foods.erase(it);
        }
        foods.clear();
        
        startPlaying = false;
        // fout << "deleted all foods" << '\n';
        // std::cout << "deleted all foods" << '\n';
    } else
        while (SDL_PollEvent(&event))
        {
            switch(event.type)
            {
                case SDL_QUIT:
                    gameRunning = false;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    if (event.button.button == SDL_BUTTON_LEFT) {
                        // int mouseX = event.button.x;
                        // int mouseY = event.button.y;
                        

                        keyWPressed = false;
                        keyAPressed = false;
                        keySPressed = false;
                        keyDPressed = false;

                        state = 1;
                    }
                    break;
                default:
                    break;
            }
        }

    graphics();
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