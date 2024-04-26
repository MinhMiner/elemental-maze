#include "RenderWindow.h"
#include "Entity.h"
#include "Utils.h"
#include "Map.h"
#include "Player.h"

#include "Game.h"

int main(int argc, char* args[]) {
    if (!init()) {
        std::cerr << "Initialization failed." << std::endl;
        return 1;
    }

	loadBestScores();
	while (gameRunning)
	{
		game();
        limitFPS();
	}
	saveBestScores();

	window.cleanUp();

	SDL_Quit();
	TTF_Quit();
	Mix_CloseAudio();
	return 0;
}