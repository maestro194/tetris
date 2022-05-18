#ifndef GAME_H
#define GAME_H

#include "GameBase.h"
#include "Board.h"
#include "Button.h"
#include "Piece.h"
#include "Score.h"
#include "Texture.h"

class Game{
public:
  Game();
  ~Game();

  void HomeScreen();
  void RenderHomeScreen();

  void GameScreen();
  void RenderGameScreen();

  void PauseScreen();
  void RenderPauseScreen();

  void GameOverScreen();
  void RenderGameOverScreen();
  
  void HelpScreen();
  void RenderHelpScreen();

  void GamePlaySound(int scoreFlag);
  void GameReset();

  int GetScreen();

private:
  // global
  SDL_Window* window;
  SDL_Renderer* renderer;

  int screen;

  // Homescreen
  Texture gHomeScreen; // fullscreen
  Texture gTetrisLogo; // 600x424
  Texture gPlayButtonTex[BUTTON_TOTAL]; // 250x100
  Texture gHelpButtonTex[BUTTON_TOTAL]; // 250x100

  Button gPlayButton;
  Button gHelpButton;

  SDL_Rect gHomeScreenClip = {0, 0, 1280, 720};
  SDL_Rect gTetrisLogoClip = {415, 0, 450, 318};
  SDL_Rect gPlayButtonClip = {515, 450, 200, 65};
  SDL_Rect gHelpButtonClip = {515, 560, 200, 65};

  Mix_Music* gHomeScreenBGM; // worked
  Mix_Chunk* gMenu[TOTAL_MENU_SFX];
  
  Mix_Chunk* gCombo[6];
  Mix_Chunk* gLineClear; // worked
  Mix_Chunk* gLineClearQuad; // worked
  Mix_Chunk* gLineClearSpin;
  Mix_Chunk* gLineClearAllClear;
  Mix_Chunk* gHardDrop; // worked
  Mix_Chunk* gHold; // worked
  Mix_Chunk* gRotate; // worked
  Mix_Chunk* gTopOut; // worked

    // Playscreen
    Board gBoard;
    Score gScore;

    Texture gPauseScreen;
    Texture gPauseContinueTex[BUTTON_TOTAL];
    Texture gPauseRetryTex[BUTTON_TOTAL];
    Texture gPauseBackTex[BUTTON_TOTAL];

    Button gPauseContinue;
    Button gPauseRetry;
    Button gPauseBack;

    SDL_Rect gPauseScreenClip = {0, 0, 1280, 720};
    SDL_Rect gPauseContinueClip = {540, 450, 200, 65};
    SDL_Rect gPauseRetryClip = {540, 530, 200, 65};
    SDL_Rect gPauseBackClip = {540, 610, 200, 65};

    Mix_Music* gGameBGM; // worked

    int moveTime;
    int gravityLevel[16] = {1000, 1000, 643, 404, 249, 150, 88, 50, 28, 15, 8, 4, 2, 1, 1, 1};
    int scoreFlag;

    // gameover screen
    Texture gGameOverScreen; // fullscreen
    Texture gRetryGOButtonTex[BUTTON_TOTAL];
    Texture gBackGOButtonTex[BUTTON_TOTAL];
    
    Button gRetryGOButton;
    Button gBackGOButton;

    SDL_Rect gGameOverScreenClip = { 0, 0, 1280, 720 };
    SDL_Rect gRetryGOButtonClip = { 565, 400, 200, 65 };
    SDL_Rect gBackGOButtonClip = { 565, 510, 200, 65 };

    // Helpscreen
    Texture gHelpScreen; // fullscreen
    Texture gBackButtonTex[BUTTON_TOTAL]; // 150x50

    Button gBackButton;

    SDL_Rect gHelpScreenClip = {10, 10, 1260, 700};
    SDL_Rect gBackButtonClip = {25, 25, 150, 50};
    
};

#endif