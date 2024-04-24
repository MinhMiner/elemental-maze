#include "RenderWindow.h"
#include "Entity.h"
#include "Utils.h"
#include "Map.h"
#include "Player.h"

#include "Game.h"

int main(int argc, char* args[]) {
    
    fout << "Game started..." << std::endl;

    if (!init()) {
        std::cerr << "Initialization failed." << std::endl;
        return 1;
    }

	while (gameRunning)
	{
		game();
        limitFPS();
	}

	window.cleanUp();

	SDL_Quit();
	TTF_Quit();
	Mix_CloseAudio();

    fout << "Closed game successfully\n";
    fout.close();
	return 0;
}