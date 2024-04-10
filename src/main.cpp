// #include <SDL2/SDL.h>
// #include <SDL2/SDL_image.h>
// #include <SDL2/SDL_ttf.h>
// #include <SDL2/SDL_mixer.h>
// #include <random>
// #include <chrono>
// #include <cmath>

// #include <fstream>

#include "RenderWindow.h"
#include "Entity.h"
#include "Math.h"
#include "Map.h"
#include "Player.h"

#include "Config.h"

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

Player player = Player({200, 200}, player_Texture);

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
    if (!startTitleScreen) {
        buttons.emplace_back(new Button({490, 450}, start_Button_Texture, START_BUTTON));
        startTitleScreen = true;
    }
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);

    while (SDL_PollEvent(&event))
    {
    	switch(event.type)
    	{
            case SDL_QUIT:
                gameRunning = false;
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT) {
                    keyMousePressed = true;
                    // state = 1;
                }
                break;
            case SDL_MOUSEBUTTONUP:
                if (event.button.button == SDL_BUTTON_LEFT) {
                    keyMousePressed = false;
                }
                break;
            default:
                break;
    	}
    }

    window.clear();
    window.render(0, 0, background_Texture);

    window.render(645, 283, "Bark 'n Bombs", font128, black, true);
	window.render(640, 278, "Bark 'n Bombs", font128, white, true);

    // window.render(645, 479, "Click to start", font64, white, true);
	// window.render(640, 474, "Click to start", font64, black, true);

    for (Button* b: buttons) {
        window.render(*b);
        b->update(mouseX, mouseY, keyMousePressed);

        if (b->getType() == START_BUTTON && b->isClicked()) {
            state = 1;
        }
    }

    window.display();
}

void update() {
    totalTime += deltaTime;

    if (!startPlaying) {    // Reset điểm và thời gian lúc mới bắt đầu trò chơi
        buttons.clear();
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
            bombs.emplace_back(new Bomb({randomXVal, randomYVal}, bomb_Texture));

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

        if (!checkCollisions(randomXVal, randomYVal, 50, 50, walls) && sqrt(pow(player.getPos().x - randomXVal, 2) + pow(player.getPos().y - randomYVal, 2)) > 400) {
            if (randomFoodSeed <= 60) {
                foods.emplace_back(new Food({randomXVal, randomYVal}, bone_Texture, BONE));
            } else if (randomFoodSeed <= 70) {
                foods.emplace_back(new Food({randomXVal, randomYVal}, fish_Texture, FISH));
            } else if (randomFoodSeed <= 80) {
                foods.emplace_back(new Food({randomXVal, randomYVal}, steak_Texture, STEAK));
            } else if (randomFoodSeed <= 90) {
                foods.emplace_back(new Food({randomXVal, randomYVal}, chicken_Texture, CHICKEN));
            } else if (randomFoodSeed <= 100) {
                foods.emplace_back(new Food({randomXVal, randomYVal}, gift_Texture, GIFT));
            }

            lastFoodSpawned = totalTime;
        }
    }
    
	while (SDL_PollEvent(&event))
    {
        int mouseX, mouseY;
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
                mouseX = event.button.x;
                mouseY = event.button.y;
                keyMousePressed = true;

                std::cout << "Mouse click at (" << mouseX << ", " << mouseY << ")\n";
                fout << "Mouse click at (" << mouseX << ", " << mouseY << ")\n";
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
        } else if (foodCollected->getFoodType() == GIFT) {
            foodScore += 150;
            player.updateEnergy(-20000);
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
                fout << "Score: " << score << '\n';
                std::cout << "Score: " << score << '\n';               
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

        window.render(1000, 25, energy_bar_Texture, player.getEnergy()/player.getMaxEnergy());
        window.render(1000, 25, energy_bar_outline_Texture);

        if (player.getEffectDuration(SHIELD) > 0)
            window.render(900, 25, shield_Texture, player.getEffectDuration(SHIELD)/player.getEffectMaxDuration(SHIELD));
		
        if (player.getEffectDuration(DASH) > 0)
            window.render(800, 25, dash_Texture, player.getEffectDuration(DASH)/player.getEffectMaxDuration(DASH));

        if (player.getEffectDuration(SPEED) > 0)
            window.render(700, 25, speed_Texture, player.getEffectDuration(SPEED)/player.getEffectMaxDuration(SPEED));

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
        for (Button* &b: buttons) {
            window.render(*b);
        }

        std::string foodCountString = "Food collected: " + std::to_string(player.getFoodCount());
        const char* foodCountCStr = foodCountString.c_str();

        window.render(645, 305, foodCountCStr, font64, white, true);
	    window.render(640, 300, foodCountCStr, font64, black, true);

        window.render(645, 455, "Game Over!", font128, black, true);
	    window.render(640, 450, "Game Over!", font128, white, true);

        // window.render(645, 555, "Click to retry!", font64, white, true);
	    // window.render(640, 550, "Click to retry!", font64, black, true);


        window.display();
    }
}

void endScreen() {
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
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

        // for (auto it = walls.begin(); it != walls.end(); ) {
        //     delete *it;
        //     it = walls.erase(it);
        // }
        // walls.clear();
        
        buttons.emplace_back(new Button({490, 550}, play_again_Button_Texture, PLAY_AGAIN_BUTTON));
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

                        // keyWPressed = false;
                        // keyAPressed = false;
                        // keySPressed = false;
                        // keyDPressed = false;

                        // state = 1;
                        keyMousePressed = true;
                    }
                    break;
                case SDL_MOUSEBUTTONUP:
                    if (event.button.button == SDL_BUTTON_LEFT) {
                        keyMousePressed = false;
                    }
                    break;
                default:
                    break;
            }
        }

    for (Button* b: buttons) {
        b->update(mouseX, mouseY, keyMousePressed);

        if (b->getType() == PLAY_AGAIN_BUTTON && b->isClicked()) {
            keyWPressed = false;
            keyAPressed = false;
            keySPressed = false;
            keyDPressed = false;
            state = 1;
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
        player.setPos(608, 422);
        break;
    case 1:
        player.setPos(608, 422);
        
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
    case 2:
        player.setPos(608, 422);

        walls.emplace_back(new Wall({0, 164}, brick_wall_Texture));
        walls.emplace_back(new Wall({64, 164}, brick_wall_Texture));
        walls.emplace_back(new Wall({128, 164}, brick_wall_Texture));
        walls.emplace_back(new Wall({0, 228}, brick_wall_Texture));
        walls.emplace_back(new Wall({64, 228}, brick_wall_Texture));
        walls.emplace_back(new Wall({0, 292}, brick_wall_Texture));

        walls.emplace_back(new Wall({0, 528}, brick_wall_Texture));
        walls.emplace_back(new Wall({0, 592}, brick_wall_Texture));
        walls.emplace_back(new Wall({64, 592}, brick_wall_Texture));
        walls.emplace_back(new Wall({0, 656}, brick_wall_Texture));
        walls.emplace_back(new Wall({64, 656}, brick_wall_Texture));
        walls.emplace_back(new Wall({128, 656}, brick_wall_Texture));

        walls.emplace_back(new Wall({1088, 164}, brick_wall_Texture));
        walls.emplace_back(new Wall({1152, 164}, brick_wall_Texture));
        walls.emplace_back(new Wall({1216, 164}, brick_wall_Texture));
        walls.emplace_back(new Wall({1152, 228}, brick_wall_Texture));
        walls.emplace_back(new Wall({1216, 228}, brick_wall_Texture));
        walls.emplace_back(new Wall({1216, 292}, brick_wall_Texture));

        walls.emplace_back(new Wall({1216, 528}, brick_wall_Texture));
        walls.emplace_back(new Wall({1216, 592}, brick_wall_Texture));
        walls.emplace_back(new Wall({1152, 592}, brick_wall_Texture));
        walls.emplace_back(new Wall({1216, 656}, brick_wall_Texture));
        walls.emplace_back(new Wall({1152, 656}, brick_wall_Texture));
        walls.emplace_back(new Wall({1088, 656}, brick_wall_Texture));

        walls.emplace_back(new Wall({300, 410}, brick_wall_Texture));
        walls.emplace_back(new Wall({364, 410}, brick_wall_Texture));
        walls.emplace_back(new Wall({428, 410}, brick_wall_Texture));
        walls.emplace_back(new Wall({788, 410}, brick_wall_Texture));
        walls.emplace_back(new Wall({852, 410}, brick_wall_Texture));
        walls.emplace_back(new Wall({916, 410}, brick_wall_Texture));

        break;
    default:
        break;
    }
}