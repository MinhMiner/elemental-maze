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
        limitFPS(FPS, deltaTime);
        // double targetFrameTime = 1000.0 / FPS;
        // if (deltaTime < targetFrameTime) {
        //     Uint32 delayMilliseconds = (Uint32)(targetFrameTime - deltaTime);
        //     SDL_Delay(delayMilliseconds);
        // }
	}

	window.cleanUp();

	SDL_Quit();
	TTF_Quit();
	Mix_CloseAudio();

    fout << "Closed game successfully\n";
    fout.close();
	return 0;
}


