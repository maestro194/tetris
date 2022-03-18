#include "Board.h"
#include "Button.h"
#include "Game.h"
#include "GameBase.h"
#include "Pieces.h"

// Global
SDL_Window* gWindow;
SDL_Renderer* gRenderer;

// Board
Board gBoard;

// Texture

// Homescreen
Texture gHomeScreen; // fullscreen
Texture gTetrisLogo; // 600x424
Texture gPlayButtonTex[BUTTON_TOTAL]; // 350x150
Texture gHelpButtonTex[BUTTON_TOTAL]; // 350x150

// Playfield
Texture gBoardTex;
// 33px*33px block 
Texture gBlock[TOTAL_BLOCK_COLOR]; // I L O revL S T Z

SDL_Rect gHomeScreenClip = {0, 0, 1600, 1000};
SDL_Rect gTetrisLogoClip = {500, 0, 600, 424};

// Button
Button gPlayButton(625, 550, 350, 150);
Button gHelpButton(625, 750, 350, 150);

SDL_Rect gPlayButtonClip = {625, 550, 350, 150};
SDL_Rect gHelpButtonClip = {625, 750, 350, 150};

// Pieces
std::queue<Piece> incomingPieces;
Piece currentPiece;

// Pre-declared Function
bool Init();
void Close();

int main(int argc, char* argv[]) {
  
  if (!Init()) {
    ErrorLog("Cant init. ", SDL_ERROR);
    return 0;
  }

  bool HomeRunning = true,
       GameRunning = true;
  SDL_Event e;
  int cnt = 0;
  
  while (HomeRunning) {
    while (SDL_PollEvent(&e) != 0) {
      if (e.type == SDL_QUIT)
        HomeRunning = false;

      if (e.type == SDL_MOUSEMOTION || e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP) {
        gPlayButton.HandleEvent(&e);
        gHelpButton.HandleEvent(&e);
      }
    }

    SDL_RenderClear(gRenderer);
    gHomeScreen.Render(gRenderer, 0, 0, &gHomeScreenClip);
    gTetrisLogo.Render(gRenderer, 500, 0, &gTetrisLogoClip);
    gPlayButton.Render(gRenderer, gPlayButtonTex[gPlayButton.buttonSprite], &gPlayButtonClip);
    gHelpButton.Render(gRenderer, gHelpButtonTex[gHelpButton.buttonSprite], &gHelpButtonClip);
    SDL_RenderPresent(gRenderer);

    if (gPlayButton.buttonSprite == BUTTON_DOWN) { // game started
      while (GameRunning) {
        while (SDL_PollEvent(&e)) {
          if (e.type == SDL_QUIT)
            GameRunning = false;
        }

        SDL_RenderClear(gRenderer);
        gHomeScreen.Render(gRenderer, 0, 0, &gHomeScreenClip);
        SDL_RenderPresent(gRenderer);
      }

      // reset the game (code soon)
      GameRunning = true;
      gPlayButton.buttonSprite = BUTTON_DEFAULT;
    }
  }

  Close();

	return 0;
}

bool Init() {
  bool success = true;

  if (SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    ErrorLog("SDL could not initialize. ", SDL_ERROR),
      success = false;
  }
  else {
    if (!(IMG_Init(IMG_INIT_PNG))) {
      ErrorLog("IMG_Init failed to init. ", IMG_ERROR);
      success = false;
    }
    else {
      gWindow = SDL_CreateWindow("Tetris", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
      gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
      SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);  
    }
  }

  gHomeScreen.LoadTextureFromFile("images/home_screen.png", gRenderer);
  gTetrisLogo.LoadTextureFromFile("images/tetris_logo.png", gRenderer);

  gPlayButtonTex[BUTTON_DEFAULT].LoadTextureFromFile("images/play.png", gRenderer);
  gPlayButtonTex[BUTTON_HOVERED].LoadTextureFromFile("images/play_hovered.png", gRenderer);
  gPlayButtonTex[BUTTON_DOWN].LoadTextureFromFile("images/play_clicked.png", gRenderer);
  gHelpButtonTex[BUTTON_DEFAULT].LoadTextureFromFile("images/help.png", gRenderer);

  // gBoardTex.LoadTextureFromFile();

  gBlock[0].LoadTextureFromFile("images/block_cyan.png", gRenderer);
  gBlock[1].LoadTextureFromFile("images/block_lime.png", gRenderer);
  gBlock[2].LoadTextureFromFile("images/block_navy.png", gRenderer);
  gBlock[3].LoadTextureFromFile("images/block_orange.png", gRenderer);
  gBlock[4].LoadTextureFromFile("images/block_pink.png", gRenderer);
  gBlock[5].LoadTextureFromFile("images/block_red.png", gRenderer);
  gBlock[6].LoadTextureFromFile("images/block_yellow.png", gRenderer);
  
  return success;
}

void Close() {
  gHomeScreen.FreeTexture();
  gTetrisLogo.FreeTexture();
  for(int i = 0; i < BUTTON_TOTAL; i ++)
    gPlayButtonTex[i].FreeTexture(),
    gHelpButtonTex[i].FreeTexture();
  for(int i = 0; i < TOTAL_BLOCK_COLOR; i ++)
    gBlock[i].FreeTexture();

  SDL_DestroyWindow(gWindow);
  gWindow = NULL;
  SDL_DestroyRenderer(gRenderer);
  gRenderer = NULL;

  IMG_Quit();
  SDL_Quit();
}

