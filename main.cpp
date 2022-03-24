#include "Board.h"
#include "Button.h"
#include "Game.h"
#include "GameBase.h"
#include "Pieces.h"
#include "Texture.h"

// Global
SDL_Window* gWindow;
SDL_Renderer* gRenderer;
int moveTime;

// Board
Board gBoard;

// Texture

// Homescreen
Texture gHomeScreen; // fullscreen
Texture gTetrisLogo; // 600x424
Texture gPlayButtonTex[BUTTON_TOTAL]; // 250x100
Texture gHelpButtonTex[BUTTON_TOTAL]; // 250x100

// Playfield
Texture gBoardTex;// 1920x1057
// 33px*33px block 
Texture gBlock[TOTAL_BLOCK_COLOR]; // I L O revL S T Z

SDL_Rect gHomeScreenClip = {0, 0, 1280, 720};
SDL_Rect gTetrisLogoClip = {415, 0, 450, 318};
SDL_Rect gBoardTexClip = {0, 0, 1280, 720};
SDL_Rect gBlockClip = {0, 0, BLOCK_WIDTH, BLOCK_HEIGHT};

// Button
Button gPlayButton(515, 450, 250, 100);
Button gHelpButton(515, 575, 250, 100);

SDL_Rect gPlayButtonClip = {515, 450, 250, 100};
SDL_Rect gHelpButtonClip = {515, 575, 250, 100};

// Pieces
Piece currentPiece;
int incomingPiece[15];
int arrayForRandom[7] = {1,2,3,4,5,6,7};
int pieceCount;

// Pre-declared Function
bool Init();
void firstPieceBag();
void secondPieceBag();
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
    gTetrisLogo.Render(gRenderer, 415, 0, &gTetrisLogoClip);
    gPlayButton.Render(gRenderer, gPlayButtonTex[gPlayButton.buttonSprite], &gPlayButtonClip);
    gHelpButton.Render(gRenderer, gHelpButtonTex[gHelpButton.buttonSprite], &gHelpButtonClip);
    SDL_RenderPresent(gRenderer);

    if (gPlayButton.buttonSprite == BUTTON_DOWN) { // game started
      // game value initialization
      pieceCount = 0;
      firstPieceBag();
      secondPieceBag();
      moveTime = SDL_GetTicks();
      currentPiece.Init(incomingPiece[pieceCount]);

      while (GameRunning) {
        while (SDL_PollEvent(&e)) {
          if (e.type == SDL_QUIT)
            GameRunning = false, HomeRunning = false;
        }

        // piece move
        if (SDL_GetTicks() > moveTime) {
          moveTime += 300;
          currentPiece.PieceDownMove();

          if (!gBoard.IsPosibleMove(currentPiece)) {
            currentPiece.PieceUpMove();
            pieceCount = (pieceCount + 1) % 14;
            gBoard.MergePiece(currentPiece);
            if (pieceCount == 0) {
              firstPieceBag();
              secondPieceBag();
            }
            currentPiece.Init(incomingPiece[pieceCount]);
          }
        }
        
        SDL_RenderClear(gRenderer);
        
        // draw board
        gBoardTex.Render(gRenderer, 0, 0, &gBoardTexClip);
        gBoard.DrawBoard(gRenderer, gBlock);
        
        // draw the well,  replace with image soon
        SDL_RenderDrawLine(gRenderer, 483, 82, 483, 602);
        SDL_RenderDrawLine(gRenderer, 483, 602, 743, 602);
        SDL_RenderDrawLine(gRenderer, 743, 82, 743, 602);
        SDL_SetRenderDrawColor(gRenderer, 128, 128, 128, 128);
        for(int j = 1; j < 10; j ++)
          SDL_RenderDrawLine(gRenderer, 483 + 26 * j, 82, 483 + 26 * j, 602);
        for(int i = 1; i < 20; i ++)
          SDL_RenderDrawLine(gRenderer, 483, 82 + 26 * i, 743, 82 + 26 * i);
        
        // draw current piece on the board
        currentPiece.DrawPiece(gRenderer, gBlock);
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
      SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 128);  
    }
  }

  srand(time(0));

  gHomeScreen.LoadTextureFromFile("images/home_screen.png", gRenderer);
  gTetrisLogo.LoadTextureFromFile("images/tetris_logo.png", gRenderer);

  gPlayButtonTex[BUTTON_DEFAULT].LoadTextureFromFile("images/play.png", gRenderer);
  gPlayButtonTex[BUTTON_HOVERED].LoadTextureFromFile("images/play_hovered.png", gRenderer);
  gPlayButtonTex[BUTTON_DOWN].LoadTextureFromFile("images/play_clicked.png", gRenderer);
  gHelpButtonTex[BUTTON_DEFAULT].LoadTextureFromFile("images/help.png", gRenderer);
  gHelpButtonTex[BUTTON_HOVERED].LoadTextureFromFile("images/play_hovered.png", gRenderer);
  gHelpButtonTex[BUTTON_DOWN].LoadTextureFromFile("images/play_clicked.png", gRenderer);

  gBoardTex.LoadTextureFromFile("images/plain_play_field.png", gRenderer);

  gBlock[1].LoadTextureFromFile("images/block_cyan.png", gRenderer);
  gBlock[2].LoadTextureFromFile("images/block_lime.png", gRenderer);
  gBlock[3].LoadTextureFromFile("images/block_navy.png", gRenderer);
  gBlock[4].LoadTextureFromFile("images/block_orange.png", gRenderer);
  gBlock[5].LoadTextureFromFile("images/block_pink.png", gRenderer);
  gBlock[6].LoadTextureFromFile("images/block_red.png", gRenderer);
  gBlock[7].LoadTextureFromFile("images/block_yellow.png", gRenderer);
  
  return success;
}

void firstPieceBag() {
  random_shuffle(arrayForRandom, arrayForRandom + 7);
  for (int i = 0; i < 7; i++)
    incomingPiece[i] = arrayForRandom[i];
}

void secondPieceBag() {
  random_shuffle(arrayForRandom, arrayForRandom + 7);
  for (int i = 7; i < 14; i++)
    incomingPiece[i] = arrayForRandom[i - 7];
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

