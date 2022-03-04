#include "Game.h"

Game::Game() {
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		std::cout << "SDL could not initialize! SDL_Error: " << '\n' << SDL_GetError();
	if (!(IMG_Init(IMG_INIT_PNG)))
		std::cout << "IMG_Init failed to init! Error: " << IMG_GetError() << '\n';

	gWindow = SDL_CreateWindow("Tetris", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
		WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
	gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDER_ACCELERATED);

}

Game::~Game() {
	SDL_DestroyWindow(gWindow);
	SDL_DestroyRenderer(gRenderer);
	
	SDL_Quit();
}