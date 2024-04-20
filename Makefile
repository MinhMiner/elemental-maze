all:
	g++ -I src/include -L src/lib -o main src/main.cpp src/game.cpp src/renderWindow.cpp src/button.cpp src/player.cpp src/entity.cpp -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer