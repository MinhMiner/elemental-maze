all:
	g++ -I src/include -L src/lib -o main src/main.cpp src/renderWindow.cpp src/entity.cpp -lmingw32 -lSDL2main -lSDL2 -lSDL2_image