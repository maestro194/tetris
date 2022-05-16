#ifndef SNAKE_H
#define SNAKE_H

#include "GameBase.h"
#include "Texture.h"

class Snake{
public:
  Snake();
  ~Snake();

  void Game();

  bool GameTick();
  void FoodGen();
  void Draw();

private:
  SDL_Window* window;
  SDL_Renderer* renderer;

  Texture sHead;
  Texture sBody;
  Texture sBodyTurn;
  Texture sTail;
  Texture sFood;

  int gameTick = 0;
  int dx = 1;
  int dy = 0;
  int foodX;
  int foodY;

  deque< pair<int, int> > snakeQueue;
};

#endif