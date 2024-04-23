#include "Game.h"

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

std::ofstream fout("console.log");

const int FPS = 120;

SDL_Color white = {255, 255, 255};
SDL_Color black = {0, 0, 0};

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
SDL_Texture *select_map_Button_Texture = window.loadTexture("res/gfx/select_map_button.png");
SDL_Texture *pause_Button_Texture = window.loadTexture("res/gfx/pause_button.png");
SDL_Texture *home_Button_Texture = window.loadTexture("res/gfx/home_button.png");
SDL_Texture *pause_screen_overlay_Texture = window.loadTexture("res/gfx/pause_screen_overlay.png");
SDL_Texture *map_1_Texture = window.loadTexture("res/gfx/map_1.png");
SDL_Texture *map_2_Texture = window.loadTexture("res/gfx/map_2.png");

TTF_Font* font32 = TTF_OpenFont("res/font/font.ttf", 32);
TTF_Font* font64 = TTF_OpenFont("res/font/font.ttf", 64);
TTF_Font* font128 = TTF_OpenFont("res/font/font.ttf", 128);

int level = 1;
stateID state = TITLE_SCREEN;

Player player = Player({200, 200}, player_Texture);

bool gameRunning = true;
bool startPlaying = false;
bool startTitleScreen = false;
bool startEndScreen = false;
bool startSelectMapScreen = false;

double deltaTime = 0;
double totalTime = 0.0;
int score = 0;
int foodScore = 0;
double lastBombSpawned = 0;
double lastFoodSpawned = 0;

double delayBetweenBombs;
double delayBetweenFoods;

inputKeys inputQueue;

std::vector<Wall*> walls;
std::vector<Bomb*> bombs;
std::vector<Food*> foods;
std::vector<Button*> buttons;

std::chrono::system_clock::time_point tp = std::chrono::system_clock::now();
std::mt19937 gen(tp.time_since_epoch().count());

SDL_Event event;

Uint64 currentTick = SDL_GetPerformanceCounter();
Uint64 lastTick = 0;

void limitFPS() {
    double targetFrameTime = 1000.0 / FPS;
    if (deltaTime < targetFrameTime) {
        Uint32 delayMilliseconds = (Uint32)(targetFrameTime - deltaTime);
        SDL_Delay(delayMilliseconds);
    }
}

void game() {
    updateDeltatime();

	if (state == TITLE_SCREEN)
	{
		titleScreen();
	}
	else if (state == PLAY_SCREEN)
	{
		playScreen();
	}
    else if (state == SELECT_MAP_SCREEN)
    {
        selectMapScreen();
    }
    else if (state == PAUSE_SCREEN)
    {
        pauseScreen();
    }
    else if (state == END_SCREEN)
    {
        endScreen();
    }
}

void updateDeltatime() {
    lastTick = currentTick;
	currentTick = SDL_GetPerformanceCounter();
	deltaTime = (double) ((currentTick - lastTick)*1000 / (double) SDL_GetPerformanceFrequency());
}

void initState(stateID state) {
    switch (state)
    {
    case TITLE_SCREEN:
        buttons.clear();
        buttons.emplace_back(new Button({490, 450}, start_Button_Texture, START_BUTTON));
        buttons.emplace_back(new Button({490, 570}, select_map_Button_Texture, SELECT_MAP_BUTTON));
        startTitleScreen = true;
        inputQueue.keyMousePressed = false;
        break;
    case PLAY_SCREEN:
        buttons.clear();
        buttons.emplace_back(new Button({1225, 25}, pause_Button_Texture, PAUSE_BUTTON));
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
        loadMaps();
        startPlaying = true;
        inputQueue.keyMousePressed = false;
        break;
    case END_SCREEN:
        buttons.clear();
        bombs.clear();
        foods.clear();
        buttons.emplace_back(new Button({490, 500}, play_again_Button_Texture, PLAY_AGAIN_BUTTON));
        buttons.emplace_back(new Button({490, 610}, home_Button_Texture, HOME_BUTTON));
        startEndScreen = true;
        inputQueue.keyMousePressed = false;
        break;
    case SELECT_MAP_SCREEN:
        buttons.clear();
        buttons.emplace_back(new Button({20, 150}, map_1_Texture, MAP_1_BUTTON));
        buttons.emplace_back(new Button({660, 150}, map_2_Texture, MAP_2_BUTTON));
        startSelectMapScreen = true;
        inputQueue.keyMousePressed = false;
        break;
    default:
        break;
    }
}

void getInput() {
    SDL_GetMouseState(&inputQueue.mouseX, &inputQueue.mouseY);

    while (SDL_PollEvent(&event))
    {
    	switch(event.type)
    	{
            case SDL_QUIT:
                gameRunning = false;
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT)
                    inputQueue.keyMousePressed = true;
                break;
            case SDL_MOUSEBUTTONUP:
                if (event.button.button == SDL_BUTTON_LEFT)
                    inputQueue.keyMousePressed = false;
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_w)
                    inputQueue.keyWPressed = true;
                else if (event.key.keysym.sym == SDLK_a)
                    inputQueue.keyAPressed = true;
                else if (event.key.keysym.sym == SDLK_s)
                    inputQueue.keySPressed = true;
                else if (event.key.keysym.sym == SDLK_d)
                    inputQueue.keyDPressed = true;
                else if (event.key.keysym.sym == SDLK_SPACE)
                    inputQueue.keySpacePressed = true;   
                break;
            case SDL_KEYUP:
                if (event.key.keysym.sym == SDLK_w)
                    inputQueue.keyWPressed = false;
                else if (event.key.keysym.sym == SDLK_a)
                    inputQueue.keyAPressed = false;
                else if (event.key.keysym.sym == SDLK_s)
                    inputQueue.keySPressed = false;
                else if (event.key.keysym.sym == SDLK_d)
                    inputQueue.keyDPressed = false;
                else if (event.key.keysym.sym == SDLK_SPACE)
                    inputQueue.keySpacePressed = false;
                break;
            default:
                break;
    	}
    }
}

void buttonEvents() {
    for (Button* b: buttons) {
        b->update(inputQueue);

        if (b->getType() == START_BUTTON && b->isClicked()) {
            state = PLAY_SCREEN;
            inputQueue.keyMousePressed = false;
            startTitleScreen = false;
        }
        if (b->getType() == PLAY_AGAIN_BUTTON && b->isClicked()) {
            inputQueue.keyWPressed = false;
            inputQueue.keyAPressed = false;
            inputQueue.keySPressed = false;
            inputQueue.keyDPressed = false;
            inputQueue.keyMousePressed = false;
            state = PLAY_SCREEN;
            startEndScreen = false;
        }
        if (b->getType() == HOME_BUTTON && b->isClicked()) {
            inputQueue.keyMousePressed = false;
            state = TITLE_SCREEN;
            startEndScreen = false;
        }
        if (b->getType() == PAUSE_BUTTON && b->isClicked()) {
            if (state != PAUSE_SCREEN)
                state = PAUSE_SCREEN;
            else
                state = PLAY_SCREEN;

            inputQueue.keyMousePressed = false;
        }
        if (b->getType() == SELECT_MAP_BUTTON && b->isClicked()) {
            state = SELECT_MAP_SCREEN;
            inputQueue.keyMousePressed = false;
            startTitleScreen = false;
        }
        if (b->getType() == MAP_1_BUTTON && b->isClicked()) {
            level = 1;
            state = PLAY_SCREEN;
            inputQueue.keyMousePressed = false;
            startSelectMapScreen = false;
        }
        if (b->getType() == MAP_2_BUTTON && b->isClicked()) {
            level = 2;
            state = PLAY_SCREEN;
            inputQueue.keyMousePressed = false;
            startSelectMapScreen = false;
        }
    }
}

void titleScreen() {
    if (!startTitleScreen) {
        initState(TITLE_SCREEN);
    }

    getInput();
    buttonEvents();
    graphics();
}

void spawnBomb(const double &delayBetweenBombs) {
    if ((totalTime - lastBombSpawned) >= delayBetweenBombs) {
        std::uniform_real_distribution<float> distributionX(0, WINDOW_WIDTH - 200);
        std::uniform_real_distribution<float> distributionY(164, WINDOW_HEIGHT - 200);

        float randomXVal = distributionX(gen);
        float randomYVal = distributionY(gen);

        if (!checkCollisions(randomXVal + 100, randomYVal + 100, 100, 100, walls)) {
            bombs.emplace_back(new Bomb({randomXVal, randomYVal}, bomb_Texture));

            lastBombSpawned = totalTime;
        } 
    }
}

void spawnFood(const double &delayBetweenFoods) {
    if ((totalTime - lastFoodSpawned) >= delayBetweenFoods) {
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
}

void updateObjects() {
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
}

void playerCollectFoodEvent() {
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
}

void checkPlayerGetBombed() {
    if (player.checkCollisions(player.getPos().x, player.getPos().y, bombs) || player.getEnergy() <= 0) {
        if (player.hasEffect(SHIELD) && player.getEnergy() > 0) {
            player.removeEffect(SHIELD);
            player.addEffect({INVINCIBLE, 1, 50});
        } else {
            player.setDead();
            state = END_SCREEN;
            startPlaying = false;
        }
    }
}

void playScreen() {
    if (!startPlaying) {
        initState(PLAY_SCREEN);
    }

    totalTime += deltaTime;
    score = ((int) (totalTime / 1000)) * 10 + foodScore;

    delayBetweenBombs = 300;
    delayBetweenFoods = 3000;
    
    spawnBomb(delayBetweenBombs);
    spawnFood(delayBetweenFoods);
    
	getInput();
    buttonEvents();
    player.update(deltaTime, inputQueue, walls);

    updateObjects();
    playerCollectFoodEvent();

    if (player.hasEffect(DASH) && inputQueue.keySpacePressed) {
        player.dash(inputQueue);
    }

    if (!player.hasEffect(INVINCIBLE))
        checkPlayerGetBombed();

    graphics();
}

void pauseScreen() {
    getInput();
    buttonEvents();
    graphics();
}

void selectMapScreen() {
    if (!startSelectMapScreen)
        initState(SELECT_MAP_SCREEN);

    getInput();
    buttonEvents();
    graphics();
}

void graphics() {
    window.clear();
    if (state == TITLE_SCREEN) {
        window.render(0, 0, title_screen_background_Texture);

        window.render(645, 283, "Bark 'n Bombs", font128, black, true);
        window.render(640, 278, "Bark 'n Bombs", font128, white, true);

        for (Button* b: buttons) {
            window.render(*b);
        }
    }
    else if (state == SELECT_MAP_SCREEN) {
        window.render(0, 0, title_screen_background_Texture);

        window.render(645, 83, "Choose the map you want to play below", font32, black, true);
        window.render(643, 81, "Choose the map you want to play below", font32, white, true);

        for (Button* b: buttons) {
            window.render(*b);
        }
    }
	else if (state == PLAY_SCREEN || state == PAUSE_SCREEN) 
	{
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
        for (Button* b: buttons) {
            window.render(*b);
        }
        for (Food* f: foods) {
            window.render(*f);
        }

        window.render(player, player.isMovingLeft());

        if (state == PAUSE_SCREEN)
            window.render(0, 0, pause_screen_overlay_Texture);
	} 
    else if (state == END_SCREEN)
    {
		window.render(0, 0, background_Texture);
		
        for (Button* &b: buttons) {
            window.render(*b);
        }

        std::string foodCountString = "Food collected: " + std::to_string(player.getFoodCount());
        const char* foodCountCStr = foodCountString.c_str();

        window.render(645, 255, "Game Over!", font128, black, true);
	    window.render(640, 250, "Game Over!", font128, white, true);

        window.render(645, 385, foodCountCStr, font64, white, true);
	    window.render(640, 380, foodCountCStr, font64, black, true);

        std::string scoreString = "Score: " + std::to_string(score);
        const char* scoreCStr = scoreString.c_str();

        window.render(645, 455, scoreCStr, font64, black, true);
	    window.render(640, 450, scoreCStr, font64, white, true);
    }
    window.display();
}

void endScreen() {
    if (!startEndScreen) {
        initState(END_SCREEN);
    } else
        getInput();

    buttonEvents();
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
        walls.clear();
        player.setPos(608, 422);
        break;
    case 1:
        walls.clear();
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
        walls.clear();
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