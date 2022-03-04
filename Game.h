#ifndef GAME_H
#define GAME_H

class Game {
public:
	SDL_Window* gWindow = NULL;
	SDL_Renderer* gRenderer = NULL;
	SDL_Event* gEvent = NULL;
private:
	Game();
	~Game();
}

#endif