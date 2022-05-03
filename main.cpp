#include "Board.h"
#include "Button.h"
#include "Game.h"
#include "GameBase.h"
#include "Piece.h"
#include "Score.h"
#include "Texture.h"

// Global
SDL_Window* gWindow;
SDL_Renderer* gRenderer;

// Game 
Game game;

// [pieceType(else, I)][rotation(0->3)][next_rotation(CW, CCW)][test(0->4)][x, y];

// SFX


// Pre-declared Function
bool Init();

void Close();

// Data

// *************************************************************************

int main(int argc, char* argv[]) {
  
  if (!Init()) {
    ErrorLog("Cant init. ", SDL_ERROR);
    return 0;
  }

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

  Close();

  return 0;
}
// *************************************************************************
/*
  while (HomeRunning) {

    if (gPlayButton.buttonSprite == BUTTON_DOWN) { // game started

      while (GameRunning) {
        while (SDL_PollEvent(&e)) {
          if (e.type == SDL_KEYDOWN) {
            if (e.key.keysym.sym == SDLK_ESCAPE) {
              GameRunning = false, HomeRunning = false;
            }
            if (e.key.keysym.sym == SDLK_z) {
              Mix_PlayChannel(-1, gRotate, 0);
            }
            if (e.key.keysym.sym == SDLK_x) {
              Mix_PlayChannel(-1, gRotate, 0);
            }
            if (e.key.keysym.sym == SDLK_a) {
              Mix_PlayChannel(-1, gRotate, 0);
            }
            if (e.key.keysym.sym == SDLK_SPACE) {

              if (gScore.GetCombo() > 0) {
                int tmpCombo = gScore.GetCombo();
                switch (tmpCombo) {
                case 1:
                  Mix_PlayChannel(-1, gCombo[1], 0);
                  break;
                case 2:
                  Mix_PlayChannel(-1, gCombo[2], 0);
                  break;
                case 3:
                  Mix_PlayChannel(-1, gCombo[3], 0);
                  break;
                case 4:
                  Mix_PlayChannel(-1, gCombo[4], 0);
                  break;
                case 5:
                  Mix_PlayChannel(-1, gCombo[5], 0);
                  break;
                default:
                  break;
                }
              }
              else
              switch (scoreFlag)
              {
              case HARD_DROP:
                Mix_PlayChannel(-1, gHardDrop, 0);
                break;
              case SINGLE:
                Mix_PlayChannel(-1, gLineClear, 0);
                break;
              case DOUBLE:
                Mix_PlayChannel(-1, gLineClear, 0);
                break;
              case TRIPLE:
                Mix_PlayChannel(-1, gLineClear, 0);
                break;
              case QUAD:
                Mix_PlayChannel(-1, gLineClearQuad, 0);
                break;
              case T_SPIN_SINGLE:
                Mix_PlayChannel(-1, gLineClearSpin, 0);
                break;
              case T_SPIN_DOUBLE:
                Mix_PlayChannel(-1, gLineClearSpin, 0);
                break;
              case T_SPIN_TRIPLE:
                Mix_PlayChannel(-1, gLineClearSpin, 0);
                break;
              case PERFECT_CLEAR:
                Mix_PlayChannel(-1, gLineClearAllClear, 0);
                break;
              default:
                break;
              }
              
            }
            if (e.key.keysym.sym == SDLK_LSHIFT && !holdThisTurn) { // hold piece
              // play hold sound
              

              
            }
          }
        }

        // piece move
        if (SDL_GetTicks() > moveTime) {

          if (!gBoard.IsPosibleMove(gCurrentPiece)) {


            if(gBoard.IsEnded()){
              // game over screen
              
            }
          }
        }
        
        SDL_RenderClear(gRenderer);
        
        // draw board bg
        
        

        // draw board
        gBoard.DrawBoard(gRenderer, gBlock);
        
        // draw the well,  replace with image soon

        //SDL_RenderDrawLine(gRenderer, 483, 82, 483, 602);
        //SDL_RenderDrawLine(gRenderer, 483, 602, 743, 602);
        //SDL_RenderDrawLine(gRenderer, 743, 82, 743, 602);
        SDL_SetRenderDrawColor(gRenderer, 128, 128, 128, 128);
        for(int j = 1; j < 10; j ++)
          SDL_RenderDrawLine(gRenderer, 483 + 26 * j, 82, 483 + 26 * j, 601);
        for(int i = 1; i < 20; i ++)
          SDL_RenderDrawLine(gRenderer, 483, 82 + 26 * i, 743, 82 + 26 * i);
        
        // draw current piece on the board
        gCurrentPiece.DrawPiece(gRenderer, gBlock);
        
        // draw the hold piece
        if(holdPiece != 0)
          gHoldPiece.DrawPiece(gRenderer, gBlock);

        // draw the next 3 piece
        for(int i = 0; i < 3; i ++)
          gNextPiece[i].DrawPiece(gRenderer, gBlock);

        // draw the score and combo
        gScore.RenderScore(gRenderer, gNumber);
        gScore.RenderLevel(gRenderer, gNumber);
        gScore.RenderLineCleared(gRenderer, gNumber);
        if(gScore.GetCombo() > 0)
        gScore.RenderCombo(gRenderer, gNumber);

        // present the window
        SDL_RenderPresent(gRenderer);
      }

      // reset the game (code soon)
      GameRunning = true;
      gBoard.Reset();
      gScore.Reset();
      gPlayButton.buttonSprite = BUTTON_DEFAULT;
    }

    if (gHelpButton.buttonSprite == BUTTON_DOWN) {

      while(HelpRunning) {

        

        SDL_RenderClear(gRenderer);
        gHomeScreen.Render(gRenderer, 0, 0, &gHomeScreenClip);
        gHelpScreen.Render(gRenderer, 10, 10, &gHelpScreenClip);
        gBackButton.Render(gRenderer, gBackButtonTex[gBackButton.buttonSprite], &gBackButtonClip);
        SDL_RenderPresent(gRenderer);
      }
      
      gHelpButton.buttonSprite = BUTTON_DEFAULT;
      gBackButton.buttonSprite = BUTTON_DEFAULT;
    }
  }

  Close();

	return 0;
}*/

bool Init() {
  // sounds

  

  return true;
}

void Close() {

  

  

  IMG_Quit();
  Mix_Quit();
  SDL_Quit();
}



