#include "Board.h"
#include "Button.h"
#include "Game.h"
#include "GameBase.h"
#include "Piece.h"
#include "Score.h"
#include "Texture.h"

// Game 
Game game;

// *************************************************************************

int main(int argc, char* argv[]) {
  bool GameRunning = true;
  SDL_Event e;

  while(GameRunning){
    if(game.GetScreen() == HOME_SCREEN)
      game.HomeScreen();
    else if(game.GetScreen() == GAME_SCREEN)
      game.GameScreen();
    else if(game.GetScreen() == HELP_SCREEN)
      game.HelpScreen();
    else
      GameRunning = false;
  }

  IMG_Quit();
  Mix_Quit();
  SDL_Quit();

  return 0;
}