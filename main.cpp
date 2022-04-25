#include "Board.h"
#include "Button.h"
#include "GameBase.h"
#include "Pieces.h"
#include "Score.h"
#include "Texture.h"

// Global
SDL_Window* gWindow;
SDL_Renderer* gRenderer;

// Board
Board gBoard;
int moveTime;
int gravity;
int combo;
int score;
// Score
Score gScore;

// Texture

// Homescreen
Texture gHomeScreen; // fullscreen
Texture gHelpScreen; // fullscreen
Texture gGameOverScreen; // fullscreen
Texture gTetrisLogo; // 600x424
Texture gPlayButtonTex[BUTTON_TOTAL]; // 250x100
Texture gHelpButtonTex[BUTTON_TOTAL]; // 250x100
Texture gBackButtonTex[BUTTON_TOTAL]; // 150x50

// Playfield
Texture gBoardTex; // fullscreen
// 25px*25px block 
Texture gBlock[TOTAL_BLOCK_COLOR]; // I L O revL S T Z

SDL_Rect gHomeScreenClip = {0, 0, 1280, 720};
SDL_Rect gHelpScreenClip = {10, 10, 1260, 700};
SDL_Rect gGameOverScreenClip = {0, 0, 1280, 720};
SDL_Rect gTetrisLogoClip = {415, 0, 450, 318};
SDL_Rect gBoardTexClip = {0, 0, 1280, 720};
SDL_Rect gBlockClip = {0, 0, BLOCK_WIDTH, BLOCK_HEIGHT};

// Button
Button gPlayButton(515, 450, 250, 100);
Button gHelpButton(515, 575, 250, 100);
Button gBackButton(25, 25, 150, 50);

SDL_Rect gPlayButtonClip = {515, 450, 250, 100};
SDL_Rect gHelpButtonClip = {515, 575, 250, 100};
SDL_Rect gBackButtonClip = {25, 25, 150, 50};

// Pieces
Piece gCurrentPiece;
Piece gHoldPiece;
Piece gNextPiece[3];
int holdPiece;
int incomingPiece[15];
int arrayForRandom[7] = {1,2,3,4,5,6,7};
int pieceCount;
int lineClear;
int xKick, yKick;
bool holdThisTurn;
// [pieceType(else, I)][rotation(0->3)][next_rotation(CW, CCW)][test(0->4)][x, y];

// SFX
int musicFlag; // change to Mix_Chunk soon
int scoreFlag;
Mix_Music* gHomeScreenBGM; // worked
Mix_Music* gGameBGM; // worked
Mix_Chunk* gLineClear; // worked
Mix_Chunk* gLineClearQuad; // worked
Mix_Chunk* gLineClearSpin;
Mix_Chunk* gLineClearAllClear;
Mix_Chunk* gHardDrop; // worked
Mix_Chunk* gHold; // worked
Mix_Chunk* gMenu[3];
Mix_Chunk* gRotate; // worked
Mix_Chunk* gTopOut; // worked

// Pre-declared Function
bool Init();
void firstPieceBag();
void secondPieceBag();
void InitWallKick();
void Close();

// Data
int wallKickData[2][4][2][5][2];

// ***************************************************************************

int main(int argc, char* argv[]) {
  
  if (!Init()) {
    ErrorLog("Cant init. ", SDL_ERROR);
    return 0;
  }

  bool HomeRunning = true,
       GameRunning = true;
  SDL_Event e;
  int cnt = 0;

  Mix_PlayMusic(gHomeScreenBGM, -1);
  
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
      // change the music
      Mix_HaltMusic();
      Mix_PlayMusic(gGameBGM, -1);
      // game value initialization
      pieceCount = 0;
      holdPiece = NO_PIECE;
      firstPieceBag();
      secondPieceBag();
      moveTime = SDL_GetTicks();
      gCurrentPiece.Init(incomingPiece[pieceCount]);
      // preview piece init 
      for(int i = 0; i < 3; i ++){
        gNextPiece[i].Init(incomingPiece[pieceCount + 1 + i]);
        gNextPiece[i].xOffSet = 4 + 4 * i;
        gNextPiece[i].yOffSet = 14;
      }

      while (GameRunning) {
        while (SDL_PollEvent(&e)) {
          if (e.type == SDL_QUIT)
            GameRunning = false, HomeRunning = false;
          if (e.type == SDL_KEYDOWN) {
            if (e.key.keysym.sym == SDLK_ESCAPE) {
              GameRunning = false, HomeRunning = false;
            }
            if (e.key.keysym.sym == SDLK_LEFT) {
              gCurrentPiece.PieceLeftMove();
              if (!gBoard.IsPosibleMove(gCurrentPiece))
                gCurrentPiece.PieceRightMove();
            }
            if (e.key.keysym.sym == SDLK_DOWN) {
              gCurrentPiece.PieceDownMove();
              if (!gBoard.IsPosibleMove(gCurrentPiece))
                gCurrentPiece.PieceUpMove();
            }
            if (e.key.keysym.sym == SDLK_RIGHT) {
              gCurrentPiece.PieceRightMove();
              if (!gBoard.IsPosibleMove(gCurrentPiece))
                gCurrentPiece.PieceLeftMove();
            }
            if (e.key.keysym.sym == SDLK_z) {
              Mix_PlayChannel(-1, gRotate, 0);
              if(gCurrentPiece.pieceType == O_PIECE) {
                gCurrentPiece.PieceCCWRotateMove(0, 0);
                if (!gBoard.IsPosibleMove(gCurrentPiece))
                  gCurrentPiece.PieceCWRotateMove(0, 0);
              }
              else if(gCurrentPiece.pieceType == I_PIECE) {
                for(int i = 0; i < 5; i ++) {
                  xKick = wallKickData[1][gCurrentPiece.rotation][1][i][0];
                  yKick = wallKickData[1][gCurrentPiece.rotation][1][i][1];
                  gCurrentPiece.PieceCCWRotateMove(xKick, yKick);
                  if (!gBoard.IsPosibleMove(gCurrentPiece))
                    gCurrentPiece.PieceCWRotateMove(-xKick, -yKick);
                  else
                    break;
                }
              }
              else {
                for(int i = 0; i < 5; i ++) {
                  xKick = wallKickData[0][gCurrentPiece.rotation][1][i][0];
                  yKick = wallKickData[0][gCurrentPiece.rotation][1][i][1];   
                  gCurrentPiece.PieceCCWRotateMove(xKick, yKick);
                  if(!gBoard.IsPosibleMove(gCurrentPiece))
                    gCurrentPiece.PieceCWRotateMove(-xKick, -yKick);
                  else  
                    break;
                }
              }
            }
            if (e.key.keysym.sym == SDLK_x) {
              Mix_PlayChannel(-1, gRotate, 0);
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
              Mix_PlayChannel(-1, gRotate, 0);
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
              
              scoreFlag = gBoard.LineClearType(gCurrentPiece);
              gScore.ScoreUpdate(scoreFlag);

              gScore.RenderScore();

              pieceCount = (pieceCount + 1) % 14;
              if (pieceCount == 0)
                secondPieceBag();
              if (pieceCount == 7)
                firstPieceBag();
              gCurrentPiece.Init(incomingPiece[pieceCount]);
              for(int i = 0; i < 3; i ++){
                gNextPiece[i].Init(incomingPiece[(pieceCount + 1 + i) % 14]);
                gNextPiece[i].xOffSet = 4 + 4 * i;
                gNextPiece[i].yOffSet = 14;
              }

              // musicFlag
              
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

              // if any line is cleared, computing the score and combo
              // else reset the combo counter
              if(musicFlag != NO_CLEAR) {
                combo ++;
                if(combo > 0) { // display the combo

                }
              }
              else {  
                combo = -1;
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
                for(int i = 0; i < 3; i ++){
                  gNextPiece[i].Init(incomingPiece[(pieceCount + 1 + i) % 14]);
                  gNextPiece[i].xOffSet = 4 + 4 * i;
                  gNextPiece[i].yOffSet = 14;
                }
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

            scoreFlag = gBoard.LineClearType(gCurrentPiece);
            gScore.ScoreUpdate(scoreFlag);

            gScore.RenderScore();

            pieceCount = (pieceCount + 1) % 14;
            if (pieceCount == 0)
              secondPieceBag();
            if (pieceCount == 7)
              firstPieceBag();
            gCurrentPiece.Init(incomingPiece[pieceCount]);

            // musicFlag;

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
                Mix_PlayChannel(-1, gLineClearSpin, 0);
                break;
              case ALL_CLEAR:
                Mix_PlayChannel(-1, gLineClearAllClear, 0);
                break;
              default:
                break;
            } 

            if(gBoard.IsEnded()){
              // game over screen
              Mix_PlayChannel(-1, gTopOut, 0);
              Mix_HaltMusic();
              bool GameOverRunning = 1;
              while(GameOverRunning){
                while(SDL_PollEvent(&e)){
                  if(e.type == SDL_KEYDOWN){
                    if(e.key.keysym.sym == SDLK_ESCAPE){
                      GameOverRunning = 0;
                    }
                  }
                }

                SDL_RenderClear(gRenderer);
                gGameOverScreen.Render(gRenderer, 0, 0, &gGameOverScreenClip);
                SDL_RenderPresent(gRenderer);
              }
              GameRunning = false;
              continue;
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
        for(int i = 0; i < 3; i ++)
          gNextPiece[i].DrawPiece(gRenderer, gBlock);

        // present the window
        SDL_RenderPresent(gRenderer);
      }

      // reset the game (code soon)
      GameRunning = true;
      gBoard.Reset();
      gPlayButton.buttonSprite = BUTTON_DEFAULT;
    }

    if (gHelpButton.buttonSprite == BUTTON_DOWN) {
      bool HelpRunning = 1;

      while(HelpRunning) {
        while(SDL_PollEvent(&e)){
          if(e.type == SDL_QUIT){
            HelpRunning = 0;
            HomeRunning = 0;
          }
          if (e.type == SDL_MOUSEMOTION || e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP) {
            gBackButton.HandleEvent(&e);
          }
          if(e.type == SDL_KEYDOWN){
            if(e.key.keysym.sym == SDLK_ESCAPE){
              HelpRunning = 0;
            }
          }
        }

        if (gBackButton.buttonSprite == BUTTON_DOWN) {
          HelpRunning = 0;
        }

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
  gHelpScreen.LoadTextureFromFile("images/help_screen.png", gRenderer);
  gGameOverScreen.LoadTextureFromFile("images/home_screen.png", gRenderer);
  gTetrisLogo.LoadTextureFromFile("images/tetris_logo.png", gRenderer);

  gPlayButtonTex[BUTTON_DEFAULT].LoadTextureFromFile("images/play.png", gRenderer);
  gPlayButtonTex[BUTTON_HOVERED].LoadTextureFromFile("images/play_hovered.png", gRenderer);
  gPlayButtonTex[BUTTON_DOWN].LoadTextureFromFile("images/play_clicked.png", gRenderer);
  gHelpButtonTex[BUTTON_DEFAULT].LoadTextureFromFile("images/help.png", gRenderer);
  gHelpButtonTex[BUTTON_HOVERED].LoadTextureFromFile("images/help_hovered.png", gRenderer);
  gHelpButtonTex[BUTTON_DOWN].LoadTextureFromFile("images/help_clicked.png", gRenderer);
  gBackButtonTex[BUTTON_DEFAULT].LoadTextureFromFile("images/back.png", gRenderer);
  gBackButtonTex[BUTTON_HOVERED].LoadTextureFromFile("images/back_hovered.png", gRenderer);

  gBoardTex.LoadTextureFromFile("images/plain_play_field.png", gRenderer);

  gBlock[1].LoadTextureFromFile("images/block_cyan.png", gRenderer);
  gBlock[2].LoadTextureFromFile("images/block_lime.png", gRenderer);
  gBlock[3].LoadTextureFromFile("images/block_navy.png", gRenderer);
  gBlock[4].LoadTextureFromFile("images/block_orange.png", gRenderer);
  gBlock[5].LoadTextureFromFile("images/block_pink.png", gRenderer);
  gBlock[6].LoadTextureFromFile("images/block_red.png", gRenderer);
  gBlock[7].LoadTextureFromFile("images/block_yellow.png", gRenderer);

  gHomeScreenBGM = Mix_LoadMUS("music/menu_music_Aerial_City_Chika.mp3");
  gGameBGM = Mix_LoadMUS("music/game_music_Wind_Trail_Chika.mp3");

  gLineClear = Mix_LoadWAV("sfx/clearline.wav");
  gLineClearQuad = Mix_LoadWAV("sfx/clearquad.wav");
  // gLineClearSpin = Mix_LoadWAV("sfx/clearspin.wav");
  // gLineClearAllClear = Mix_LoadWAV("sfx/allclear.wav");
  gHardDrop = Mix_LoadWAV("sfx/harddrop.wav");
  gHold = Mix_LoadWAV("sfx/hold.wav");
  gRotate = Mix_LoadWAV("sfx/rotate.wav");
  gTopOut = Mix_LoadWAV("sfx/topout.wav");

  InitWallKick();
  
  return success;
}

void Close() {
  gHomeScreen.FreeTexture();
  gHelpScreen.FreeTexture();
  gGameOverScreen.FreeTexture();
  gTetrisLogo.FreeTexture();
  for(int i = 0; i < BUTTON_TOTAL; i ++)
    gPlayButtonTex[i].FreeTexture(),
    gHelpButtonTex[i].FreeTexture(),
    gBackButtonTex[i].FreeTexture();
  gBoardTex.FreeTexture();
  for(int i = 0; i < TOTAL_BLOCK_COLOR; i ++)
    gBlock[i].FreeTexture();
  
  Mix_FreeMusic(gHomeScreenBGM);
  Mix_FreeMusic(gGameBGM);

  Mix_FreeChunk(gLineClear);
  Mix_FreeChunk(gLineClearQuad);
  Mix_FreeChunk(gLineClearSpin);
  Mix_FreeChunk(gLineClearAllClear);
  Mix_FreeChunk(gHardDrop);
  Mix_FreeChunk(gHold);
  Mix_FreeChunk(gRotate);
  Mix_FreeChunk(gTopOut);

  SDL_DestroyWindow(gWindow);
  gWindow = NULL;
  SDL_DestroyRenderer(gRenderer);
  gRenderer = NULL;

  IMG_Quit();
  Mix_Quit();
  SDL_Quit();
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