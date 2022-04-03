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
Piece gCurrentPiece;
Piece gHoldPiece;
Piece gNextPiece[3];
int holdPiece;
int incomingPiece[15];
int arrayForRandom[7] = {1,2,3,4,5,6,7};
int pieceCount;
bool holdThisTurn;
int wallKickData[2][4][2][5][2];
// [pieceType(else, I)][rotation(0->3)][next_rotation(CW, CCW)][test(0->4)][x, y];

// SFX
int musicFlag; // change to Mix_Chunk soon
Mix_Music* gHomeScreenBGM;
Mix_Music* gGameBGM;
Mix_Chunk* gLineClear; // worked
Mix_Chunk* gLineClearQuad; // worked
Mix_Chunk* gLineClearSpin; // added
Mix_Chunk* gCombo[16];
Mix_Chunk* gHardDrop; // worked
Mix_Chunk* gHold; // worked
Mix_Chunk* gMenu[3];
Mix_Chunk* gRotate;
Mix_Chunk* gSpin;
Mix_Chunk* gTopOut; // worked

// Pre-declared Function
bool Init();
void firstPieceBag();
void secondPieceBag();
void DrawPreviewPiece();
void DrawHoldPiece();
void InitWallKick();
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
      holdPiece = NO_PIECE;
      firstPieceBag();
      secondPieceBag();
      moveTime = SDL_GetTicks();
      gCurrentPiece.Init(incomingPiece[pieceCount]);

      while (GameRunning) {
        while (SDL_PollEvent(&e)) {
          if (e.type == SDL_QUIT)
            GameRunning = false, HomeRunning = false;
          if (e.type == SDL_KEYDOWN) {
            if (e.key.keysym.sym == SDLK_j) {
              gCurrentPiece.PieceLeftMove();
              if (!gBoard.IsPosibleMove(gCurrentPiece))
                gCurrentPiece.PieceRightMove();
            }
            if (e.key.keysym.sym == SDLK_k) {
              gCurrentPiece.PieceDownMove();
              if (!gBoard.IsPosibleMove(gCurrentPiece))
                gCurrentPiece.PieceUpMove();
            }
            if (e.key.keysym.sym == SDLK_l) {
              gCurrentPiece.PieceRightMove();
              if (!gBoard.IsPosibleMove(gCurrentPiece))
                gCurrentPiece.PieceLeftMove();
            }
            if (e.key.keysym.sym == SDLK_z) {
              if(gCurrentPiece.pieceType == O_PIECE) {
                gCurrentPiece.PieceCCWRotateMove(0, 0);
                if (!gBoard.IsPosibleMove(gCurrentPiece))
                  gCurrentPiece.PieceCWRotateMove(0, 0);
              }
              else if(gCurrentPiece.pieceType == I_PIECE) {
                gCurrentPiece.PieceCCWRotateMove(0, 0);
                if (!gBoard.IsPosibleMove(gCurrentPiece))
                  gCurrentPiece.PieceCWRotateMove(0, 0);
              }
              else {
                gCurrentPiece.PieceCCWRotateMove(0, 0);
                if (!gBoard.IsPosibleMove(gCurrentPiece))
                  gCurrentPiece.PieceCWRotateMove(0, 0);
              }
            }
            if (e.key.keysym.sym == SDLK_x) {
              int xKick, yKick;
              if(gCurrentPiece.pieceType == O_PIECE) {
                gCurrentPiece.PieceCWRotateMove(0, 0);
                if (!gBoard.IsPosibleMove(gCurrentPiece))
                  gCurrentPiece.PieceCCWRotateMove(0, 0);
              }
              else if(gCurrentPiece.pieceType == I_PIECE) {
                for(int i = 0; i < 5; i ++) {
                  xKick = wallKickData[1][gCurrentPiece.rotation][0][i][0];
                  yKick = wallKickData[1][gCurrentPiece.rotation][0][i][1];
                  gCurrentPiece.PieceCWRotateMove(xKick, yKick);
                  if (!gBoard.IsPosibleMove(gCurrentPiece))
                    gCurrentPiece.PieceCCWRotateMove(-xKick, -yKick);
                  else
                    break;
                }
              }
              else {
                for(int i = 0; i < 5; i ++) {
                  xKick = wallKickData[0][gCurrentPiece.rotation][0][i][0];
                  yKick = wallKickData[0][gCurrentPiece.rotation][0][i][1];   
                  gCurrentPiece.PieceCWRotateMove(xKick, yKick);
                  if(!gBoard.IsPosibleMove(gCurrentPiece))
                    gCurrentPiece.PieceCCWRotateMove(-xKick, -yKick);
                  else  
                    break;
                }
              }
            }
            if (e.key.keysym.sym == SDLK_a) {
              gCurrentPiece.PieceFlipMove();
              if (!gBoard.IsPosibleMove(gCurrentPiece))
                gCurrentPiece.PieceFlipMove();
            }
            if (e.key.keysym.sym == SDLK_SPACE) {
              do{
                gCurrentPiece.PieceDownMove();
              }while(gBoard.IsPosibleMove(gCurrentPiece));
              gCurrentPiece.PieceUpMove();
              holdThisTurn = 0;
              moveTime = SDL_GetTicks();

              pieceCount = (pieceCount + 1) % 14;
              gBoard.MergePiece(gCurrentPiece);
              if (pieceCount == 0)
                secondPieceBag();
              if (pieceCount == 7)
                firstPieceBag();
              gCurrentPiece.Init(incomingPiece[pieceCount]);

              musicFlag = gBoard.DeletePosibleRow();
              
              switch (musicFlag)
              {
                case NO_CLEAR:
                  Mix_PlayChannel(-1, gHardDrop, 0);
                  break;
                case LINE_CLEAR:
                  Mix_PlayChannel(-1, gLineClear, 0);
                  break;
                case QUAD_CLEAR:
                  Mix_PlayChannel(-1, gLineClearQuad, 0);
                  break;
                case SPIN_CLEAR:
                  break;
                case ALL_CLEAR:
                  break;
                default:
                  break;
              } 
            }
            if (e.key.keysym.sym == SDLK_LSHIFT && !holdThisTurn) { // hold piece
              // play hold sound
              Mix_PlayChannel(-1, gHold, 0);
              musicFlag = DEFAULT;

              holdThisTurn = 1;

              if(holdPiece == NO_PIECE) {
                holdPiece = gCurrentPiece.pieceType;
                gHoldPiece.Init(holdPiece);
                moveTime = SDL_GetTicks();

                pieceCount = (pieceCount + 1) % 14;
                if (pieceCount == 0)
                  secondPieceBag();
                if (pieceCount == 7)
                  firstPieceBag();
                gCurrentPiece.Init(incomingPiece[pieceCount]);
              }
              else {
                swap(incomingPiece[pieceCount], holdPiece);
                gHoldPiece.Init(holdPiece);
                moveTime = SDL_GetTicks();

                gCurrentPiece.Init(incomingPiece[pieceCount]);
              }
              gHoldPiece.xOffSet = 4;
              gHoldPiece.yOffSet = -2;
            }
          }
        }

        // piece move
        if (SDL_GetTicks() > moveTime) {
          moveTime += 1000;
          gCurrentPiece.PieceDownMove();

          if (!gBoard.IsPosibleMove(gCurrentPiece)) {
            gCurrentPiece.PieceUpMove();
            holdThisTurn = 0;
            pieceCount = (pieceCount + 1) % 14;
            gBoard.MergePiece(gCurrentPiece);
            if (pieceCount == 0)
              secondPieceBag();
            if (pieceCount == 7)
              firstPieceBag();
            gCurrentPiece.Init(incomingPiece[pieceCount]);

            musicFlag = gBoard.DeletePosibleRow();
            switch (musicFlag)
            {
              case NO_CLEAR:
                Mix_PlayChannel(-1, gHardDrop, 0);
                break;
              case LINE_CLEAR:
                Mix_PlayChannel(-1, gLineClear, 0);
                break;
              case QUAD_CLEAR:
                Mix_PlayChannel(-1, gLineClearQuad, 0);
                break;
              case SPIN_CLEAR:
                break;
              case ALL_CLEAR:
                break;
              default:
                break;
            } 

            if(gBoard.IsEnded()){
              GameRunning = false;
            }
          }
        }
        
        SDL_RenderClear(gRenderer);
        
        // draw board
        gBoardTex.Render(gRenderer, 0, 0, &gBoardTexClip);
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

        // draw the next 3 piece (soon)
        
        // present the window
        SDL_RenderPresent(gRenderer);
      }

      Mix_PlayChannel(-1, gTopOut, 0);

      // game over screen

      // reset the game (code soon)
      GameRunning = true;
      gBoard.Reset();
      gPlayButton.buttonSprite = BUTTON_DEFAULT;
    }
  }

  Close();

	return 0;
}

bool Init() {
  bool success = true;

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    ErrorLog("SDL could not init video. ", SDL_ERROR),
      success = false;
  }
  else if (SDL_Init(SDL_INIT_AUDIO) < 0) {
    ErrorLog("SDL cound not init audio. ", SDL_ERROR);
  }
  else if (!(IMG_Init(IMG_INIT_PNG))) {
    ErrorLog("IMG_Init failed to init. ", IMG_ERROR);
    success = false;
  }
  else if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 2048)) {
    ErrorLog("SDL_mixer failed to init. ", MIX_ERROR);
  }
  else {
    gWindow = SDL_CreateWindow("Tetris", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
      WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 128);  
  }

  srand(time(0));

  gHomeScreen.LoadTextureFromFile("images/home_screen.png", gRenderer);
  gTetrisLogo.LoadTextureFromFile("images/tetris_logo.png", gRenderer);

  gPlayButtonTex[BUTTON_DEFAULT].LoadTextureFromFile("images/play.png", gRenderer);
  gPlayButtonTex[BUTTON_HOVERED].LoadTextureFromFile("images/play_hovered.png", gRenderer);
  gPlayButtonTex[BUTTON_DOWN].LoadTextureFromFile("images/play_clicked.png", gRenderer);
  gHelpButtonTex[BUTTON_DEFAULT].LoadTextureFromFile("images/help.png", gRenderer);
  gHelpButtonTex[BUTTON_HOVERED].LoadTextureFromFile("images/help_hovered.png", gRenderer);
  gHelpButtonTex[BUTTON_DOWN].LoadTextureFromFile("images/help_clicked.png", gRenderer);

  gBoardTex.LoadTextureFromFile("images/plain_play_field.png", gRenderer);

  gBlock[1].LoadTextureFromFile("images/block_cyan.png", gRenderer);
  gBlock[2].LoadTextureFromFile("images/block_lime.png", gRenderer);
  gBlock[3].LoadTextureFromFile("images/block_navy.png", gRenderer);
  gBlock[4].LoadTextureFromFile("images/block_orange.png", gRenderer);
  gBlock[5].LoadTextureFromFile("images/block_pink.png", gRenderer);
  gBlock[6].LoadTextureFromFile("images/block_red.png", gRenderer);
  gBlock[7].LoadTextureFromFile("images/block_yellow.png", gRenderer);

  gLineClear = Mix_LoadWAV("sfx/clearline.wav");
  gLineClearQuad = Mix_LoadWAV("sfx/clearquad.wav");
  gLineClearSpin = Mix_LoadWAV("sfx/clearspin.wav");
  gHardDrop = Mix_LoadWAV("sfx/harddrop.wav");
  gHold = Mix_LoadWAV("sfx/hold.wav");
  gTopOut = Mix_LoadWAV("sfx/topout.wav");

  InitWallKick();
  
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

void InitWallKick() {
  // JLSZT piece wall kick data
  // 0 -> R
  wallKickData[0][0][0][0][0] =  0, wallKickData[0][0][0][0][1] =  0;
  wallKickData[0][0][0][1][0] =  0, wallKickData[0][0][0][1][1] = -1;
  wallKickData[0][0][0][2][0] = -1, wallKickData[0][0][0][2][1] = -1;
  wallKickData[0][0][0][3][0] =  2, wallKickData[0][0][0][3][1] =  0;
  wallKickData[0][0][0][4][0] =  2, wallKickData[0][0][0][4][1] = -1;
  // R -> 0
  wallKickData[0][1][1][0][0] =  0, wallKickData[0][1][1][0][1] =  0;
  wallKickData[0][1][1][1][0] =  0, wallKickData[0][1][1][1][1] =  1;
  wallKickData[0][1][1][2][0] =  1, wallKickData[0][1][1][2][1] =  1;
  wallKickData[0][1][1][3][0] = -2, wallKickData[0][1][1][3][1] =  0;
  wallKickData[0][1][1][4][0] = -2, wallKickData[0][1][1][4][1] =  1;
  // R -> 2
  wallKickData[0][1][0][0][0] =  0, wallKickData[0][1][0][0][1] = 0;
  wallKickData[0][1][0][1][0] =  0, wallKickData[0][1][0][1][1] = 1;
  wallKickData[0][1][0][2][0] =  1, wallKickData[0][1][0][2][1] = 1;
  wallKickData[0][1][0][3][0] = -2, wallKickData[0][1][0][3][1] = 0;
  wallKickData[0][1][0][4][0] = -2, wallKickData[0][1][0][4][1] = 1;
  // 2 -> R
  wallKickData[0][2][1][0][0] =  0, wallKickData[0][2][1][0][1] =  0;
  wallKickData[0][2][1][1][0] =  0, wallKickData[0][2][1][1][1] = -1;
  wallKickData[0][2][1][2][0] = -1, wallKickData[0][2][1][2][1] = -1;
  wallKickData[0][2][1][3][0] =  2, wallKickData[0][2][1][3][1] =  0;
  wallKickData[0][2][1][4][0] =  2, wallKickData[0][2][1][4][1] = -1;
  // 2 -> L
  wallKickData[0][2][0][0][0] =  0, wallKickData[0][2][0][0][1] = 0;
  wallKickData[0][2][0][1][0] =  0, wallKickData[0][2][0][1][1] = 1;
  wallKickData[0][2][0][2][0] = -1, wallKickData[0][2][0][2][1] = 1;
  wallKickData[0][2][0][3][0] =  2, wallKickData[0][2][0][3][1] = 0;
  wallKickData[0][2][0][4][0] =  2, wallKickData[0][2][0][4][1] = 1;
  // L -> 2
  wallKickData[0][3][1][0][0] =  0, wallKickData[0][3][1][0][1] =  0;
  wallKickData[0][3][1][1][0] =  0, wallKickData[0][3][1][1][1] = -1;
  wallKickData[0][3][1][2][0] =  1, wallKickData[0][3][1][2][1] = -1;
  wallKickData[0][3][1][3][0] = -2, wallKickData[0][3][1][3][1] =  0;
  wallKickData[0][3][1][4][0] = -2, wallKickData[0][3][1][4][1] = -1;
  // L -> 0
  wallKickData[0][3][0][0][0] =  0, wallKickData[0][3][0][0][1] =  0;
  wallKickData[0][3][0][1][0] =  0, wallKickData[0][3][0][1][1] = -1;
  wallKickData[0][3][0][2][0] =  1, wallKickData[0][3][0][2][1] = -1;
  wallKickData[0][3][0][3][0] = -2, wallKickData[0][3][0][3][1] =  0;
  wallKickData[0][3][0][4][0] = -2, wallKickData[0][3][0][4][1] = -1;
  // 0 -> L
  wallKickData[0][0][1][0][0] =  0, wallKickData[0][0][1][0][1] = 0;
  wallKickData[0][0][1][1][0] =  0, wallKickData[0][0][1][1][1] = 1;
  wallKickData[0][0][1][2][0] = -1, wallKickData[0][0][1][2][1] = 1;
  wallKickData[0][0][1][3][0] =  2, wallKickData[0][0][1][3][1] = 0;
  wallKickData[0][0][1][4][0] =  2, wallKickData[0][0][1][4][1] = 1;
  
  // I piece wall kick data
  // 0 -> R
  wallKickData[1][0][0][0][0] =  0, wallKickData[1][0][0][0][1] =  0;
  wallKickData[1][0][0][1][0] =  0, wallKickData[1][0][0][1][1] = -2;
  wallKickData[1][0][0][2][0] =  0, wallKickData[1][0][0][2][1] =  1;
  wallKickData[1][0][0][3][0] =  1, wallKickData[1][0][0][3][1] = -2;
  wallKickData[1][0][0][4][0] = -2, wallKickData[1][0][0][4][1] =  1;
  // R -> 0
  wallKickData[1][1][1][0][0] =  0, wallKickData[1][1][1][0][1] =  0;
  wallKickData[1][1][1][1][0] =  0, wallKickData[1][1][1][1][1] =  2;
  wallKickData[1][1][1][2][0] =  0, wallKickData[1][1][1][2][1] = -1;
  wallKickData[1][1][1][3][0] = -1, wallKickData[1][1][1][3][1] =  2;
  wallKickData[1][1][1][4][0] =  2, wallKickData[1][1][1][4][1] = -1;
  // R -> 2 // fix y
  wallKickData[1][1][0][0][0] =  0, wallKickData[1][1][0][0][1] =  0;
  wallKickData[1][1][0][1][0] =  0, wallKickData[1][1][0][1][1] = -1;
  wallKickData[1][1][0][2][0] =  0, wallKickData[1][1][0][2][1] =  2;
  wallKickData[1][1][0][3][0] = -2, wallKickData[1][1][0][3][1] = -1;
  wallKickData[1][1][0][4][0] =  1, wallKickData[1][1][0][4][1] =  2;
  // 2 -> R
  wallKickData[1][2][1][0][0] =  0, wallKickData[1][2][1][0][1] =  0;
  wallKickData[1][2][1][1][0] =  0, wallKickData[1][2][1][1][1] =  1;
  wallKickData[1][2][1][2][0] =  0, wallKickData[1][2][1][2][1] = -2;
  wallKickData[1][2][1][3][0] =  2, wallKickData[1][2][1][3][1] =  1;
  wallKickData[1][2][1][4][0] = -1, wallKickData[1][2][1][4][1] = -2;
  // 2 -> L
  wallKickData[1][2][0][0][0] =  0, wallKickData[1][2][0][0][1] =  0;
  wallKickData[1][2][0][1][0] =  0, wallKickData[1][2][0][1][1] =  2;
  wallKickData[1][2][0][2][0] =  0, wallKickData[1][2][0][2][1] = -1;
  wallKickData[1][2][0][3][0] = -1, wallKickData[1][2][0][3][1] =  2;
  wallKickData[1][2][0][4][0] =  2, wallKickData[1][2][0][4][1] = -1;
  // L -> 2
  wallKickData[1][3][1][0][0] =  0, wallKickData[1][3][1][0][1] =  0;
  wallKickData[1][3][1][1][0] =  0, wallKickData[1][3][1][1][1] = -2;
  wallKickData[1][3][1][2][0] =  0, wallKickData[1][3][1][2][1] =  1;
  wallKickData[1][3][1][3][0] =  1, wallKickData[1][3][1][3][1] = -2;
  wallKickData[1][3][1][4][0] = -2, wallKickData[1][3][1][4][1] =  1;
  // L -> 0  
  wallKickData[1][3][0][0][0] =  0, wallKickData[1][3][0][0][1] =  0;
  wallKickData[1][3][0][1][0] =  0, wallKickData[1][3][0][1][1] =  1;
  wallKickData[1][3][0][2][0] =  0, wallKickData[1][3][0][2][1] = -2;
  wallKickData[1][3][0][3][0] =  2, wallKickData[1][3][0][3][1] =  1;
  wallKickData[1][3][0][4][0] = -1, wallKickData[1][3][0][4][1] = -2;
  // 0 -> L
  wallKickData[1][2][0][0][0] =  0, wallKickData[1][2][0][0][1] =  0;
  wallKickData[1][2][0][1][0] =  0, wallKickData[1][2][0][1][1] = -1;
  wallKickData[1][2][0][2][0] =  0, wallKickData[1][2][0][2][1] =  2;
  wallKickData[1][2][0][3][0] = -2, wallKickData[1][2][0][3][1] = -1;
  wallKickData[1][2][0][4][0] =  1, wallKickData[1][2][0][4][1] =  2;
}

void Close() {
  gHomeScreen.FreeTexture();
  gTetrisLogo.FreeTexture();
  for(int i = 0; i < BUTTON_TOTAL; i ++)
    gPlayButtonTex[i].FreeTexture(),
    gHelpButtonTex[i].FreeTexture();
  for(int i = 0; i < TOTAL_BLOCK_COLOR; i ++)
    gBlock[i].FreeTexture();
  
  Mix_FreeChunk(gLineClear);
  Mix_FreeChunk(gLineClearQuad);
  Mix_FreeChunk(gLineClearSpin);
  Mix_FreeChunk(gHardDrop);
  Mix_FreeChunk(gHold);
  Mix_FreeChunk(gTopOut);

  SDL_DestroyWindow(gWindow);
  gWindow = NULL;
  SDL_DestroyRenderer(gRenderer);
  gRenderer = NULL;

  IMG_Quit();
  Mix_Quit();
  SDL_Quit();
}

