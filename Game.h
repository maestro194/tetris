#ifndef GAME_H
#define GAME_H

#include "GameBase.h"

class Game {
public:
	Game();
	~Game();
	
	void Init(SDL_Window* window, SDL_Renderer* renderer);
	void CleanUp(SDL_Window* window, SDL_Renderer* renderer);
private:
	
};

#endif