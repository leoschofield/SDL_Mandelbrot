all:
	g++ -Isrc/include -Lsrc/lib -o main main.cpp -lmingw32 -lSDL2main -lSDL2_mixer -lSDL2_image -lSDL2 -Wall