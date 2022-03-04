all:
	g++ -Isrc/include -Lsrc/lib -o main.exe main.cpp -lmingw32 -lSDL2main -lSDL2 -lSDL2_image
