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
SDL_Texture *fish_Texture = window.loadTexture("res/gfx/fish.png");
SDL_Texture *steak_Texture = window.loadTexture("res/gfx/steak.png");
SDL_Texture *chicken_Texture = window.loadTexture("res/gfx/chicken.png");


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
Player player = Player({200, 200}, player_Texture);

std::chrono::system_clock::time_point tp = std::chrono::system_clock::now();
std::mt19937 gen(tp.time_since_epoch().count());

bool checkCollisions(float x, float y, float w, float h, std::vector<Wall*> &walls);

int main(int argc, char* args[]) {
    
    fout << "Game started..." << std::endl;

    if (!init()) {
        std::cerr << "Initialization failed." << std::endl;
        return 1;
    }

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
    lastTick = currentTick;
	currentTick = SDL_GetPerformanceCounter();
	deltaTime = (double) ((currentTick - lastTick)*1000 / (double) SDL_GetPerformanceFrequency());

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
    totalTime += deltaTime;

    if (!startPlaying) {    // Reset điểm và thời gian lúc mới bắt đầu trò chơi
        fout << "START PLAYING-----------------------\n\n";
        player.setAlive();
        totalTime = 0.0;
        lastBombSpawned = 0.0;
        lastFoodSpawned = 0.0;
        score = 0;
        foodScore = 0;
        player.updateEnergy(-20000);
        player.resetEffects();
        player.resetFoodCount();
        startPlaying = true;
        loadMaps();
    }

    score = ((int) (totalTime / 1000)) * 10 + foodScore;

    if ((totalTime - lastBombSpawned) >= 300) {
        std::uniform_real_distribution<float> distributionX(0, WINDOW_WIDTH - 200);
        std::uniform_real_distribution<float> distributionY(164, WINDOW_HEIGHT - 200);

        float randomXVal = distributionX(gen);
        float randomYVal = distributionY(gen);

        if (!checkCollisions(randomXVal + 100, randomYVal + 100, 100, 100, walls)) {
            Bomb *bomb = new Bomb({randomXVal, randomYVal}, bomb_Texture);
            bombs.push_back(bomb);
            // bombs.emplace_back(new Bomb({randomXVal, randomYVal}, bomb_Texture))

            lastBombSpawned = totalTime;
        } 
    }
    if ((totalTime - lastFoodSpawned) >= 3000) {
        std::uniform_real_distribution<float> distributionX(0, WINDOW_WIDTH - 64);
        std::uniform_real_distribution<float> distributionY(164, WINDOW_HEIGHT - 64);

        float randomXVal = distributionX(gen);
        float randomYVal = distributionY(gen);

        std::uniform_int_distribution<int> generateFoodSeed(1, 100);
        int randomFoodSeed = generateFoodSeed(gen);

        Food *food = nullptr;
        if (randomFoodSeed <= 70) {
            food = new Food({randomXVal, randomYVal}, bone_Texture, BONE);
        } else if (randomFoodSeed <= 80) {
            food = new Food({randomXVal, randomYVal}, fish_Texture, FISH);
        } else if (randomFoodSeed <= 90) {
            food = new Food({randomXVal, randomYVal}, steak_Texture, STEAK);
        } else if (randomFoodSeed <= 100) {
            food = new Food({randomXVal, randomYVal}, chicken_Texture, CHICKEN);
        }

        foods.push_back(food);

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
            case SDL_MOUSEBUTTONDOWN:
                // int mouseX = event.button.x;
                // int mouseY = event.button.y;
                keyMousePressed = true;
                break;
            case SDL_MOUSEBUTTONUP:
                keyMousePressed = false;
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
        if (foodCollected->getFoodType() == BONE) {
            foodScore += 10;
            player.updateEnergy(-3500);
        } else if (foodCollected->getFoodType() == FISH) {
            foodScore += 50;
            player.updateEnergy(-7000);
            player.addEffect({SPEED, 0.35, 2000});
        } else if (foodCollected->getFoodType() == STEAK) {
            foodScore += 75;
            player.updateEnergy(-10000);
            player.addEffect({SHIELD, 1, 10000});
        } else if (foodCollected->getFoodType() == CHICKEN) {
            foodScore += 60;
            player.updateEnergy(-8000);
            player.addEffect({DASH, 1, 30000});
        }
            player.collectedFood();
            foodCollected->setAge(15000);
    }
    
    player.update(deltaTime, keyWPressed, keyDPressed, keySPressed, keyAPressed, walls);

    if (player.hasEffect(DASH) && keyMousePressed) {
        player.updateEnergy(-200);
        player.addEffect({SPEED, 1, 150});
        player.addEffect({INVINCIBLE, 1, 250});
        player.removeEffect(DASH);
        keyMousePressed = false;
    }

    if (!player.hasEffect(INVINCIBLE))
        if (player.checkCollisions(player.getPos().x, player.getPos().y, bombs) || player.getEnergy() <= 0) {
            if (player.hasEffect(SHIELD) && player.getEnergy() > 0) {
                player.removeEffect(SHIELD);
                player.addEffect({INVINCIBLE, 1, 50});
            } else {
                player.setDead();
                state = 2;
                fout << "You died" << '\n';
                std::cout << "You died" << '\n';            
            }
        }
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

        window.render(700, 25, energy_bar_Texture, player.getEnergy()/player.getMaxEnergy());
        window.render(700, 25, energy_bar_outline_Texture);

        if (player.getEffectDuration(SHIELD) > 0)
            window.render(500, 25, steak_Texture, player.getEffectDuration(SHIELD)/player.getMaxShieldDuration());
		
        if (player.getEffectDuration(DASH) > 0)
            window.render(400, 25, chicken_Texture, player.getEffectDuration(DASH)/player.getMaxDashDuration()/3);

        for (Wall* &w: walls) {
            window.render(*w);
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
		
        for (Wall* &w: walls) {
            window.render(*w);
        }

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
        }
        bombs.clear();

        for (auto it = foods.begin(); it != foods.end(); ) {
            delete *it;
            it = foods.erase(it);
        }
        foods.clear();

        for (auto it = walls.begin(); it != walls.end(); ) {
            delete *it;
            it = walls.erase(it);
        }
        walls.clear();
        
        startPlaying = false;
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

bool checkCollisions(float x, float y, float w, float h, std::vector<Wall*> &walls)
{
    SDL_Rect dest;
    dest.x = x;
    dest.y = y;
    dest.w = w;
    dest.h = h;

    bool collision = false;
    for (Wall* &w: walls)
    {
        SDL_Rect temp;
        temp.x = w->getPos().x;
        temp.y = w->getPos().y;
        SDL_QueryTexture(w->getTex(), NULL, NULL, &temp.w, &temp.h);

        if (SDL_HasIntersection(&temp, &dest)) {
            collision = true;
            break;
        }
    }
    return collision;
}

void loadMaps() {
    switch (level)
    {
    case 0:
        walls.emplace_back(new Wall({320, 314}, brick_wall_Texture));
        walls.emplace_back(new Wall({320, 378}, brick_wall_Texture));
        walls.emplace_back(new Wall({320, 442}, brick_wall_Texture));
        walls.emplace_back(new Wall({320, 506}, brick_wall_Texture));
        walls.emplace_back(new Wall({384, 314}, brick_wall_Texture));
        walls.emplace_back(new Wall({384, 378}, brick_wall_Texture));
        walls.emplace_back(new Wall({384, 442}, brick_wall_Texture));
        walls.emplace_back(new Wall({384, 506}, brick_wall_Texture));

        walls.emplace_back(new Wall({832, 314}, brick_wall_Texture));
        walls.emplace_back(new Wall({832, 378}, brick_wall_Texture));
        walls.emplace_back(new Wall({832, 442}, brick_wall_Texture));
        walls.emplace_back(new Wall({832, 506}, brick_wall_Texture));
        walls.emplace_back(new Wall({896, 314}, brick_wall_Texture));
        walls.emplace_back(new Wall({896, 378}, brick_wall_Texture));
        walls.emplace_back(new Wall({896, 442}, brick_wall_Texture));
        walls.emplace_back(new Wall({896, 506}, brick_wall_Texture));

        break;
    default:
        break;
    }
}