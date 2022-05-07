#include "Game.h"

void Game::HomeScreen(){
  bool HomeRunning = true;
  bool hovered = false;
  SDL_Event e;

  Mix_PlayMusic(gHomeScreenBGM, -1);

  while(HomeRunning){
    while(SDL_PollEvent(&e)){
      if (e.type == SDL_QUIT)
        HomeRunning = false,
        Mix_HaltMusic(),
        screen = QUIT_SCREEN;

      else if (e.type == SDL_MOUSEMOTION || e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP) {
        gPlayButton.HandleEvent(&e);
        gHelpButton.HandleEvent(&e);
      }
    }

    if (hovered == false && (gPlayButton.buttonSprite == BUTTON_HOVERED || gHelpButton.buttonSprite == BUTTON_HOVERED)) {
      hovered = true;
      Mix_PlayChannel(-1, gMenu[MENU_HOVERED], 0);
    }
    else if(gPlayButton.buttonSprite == BUTTON_DEFAULT && gHelpButton.buttonSprite == BUTTON_DEFAULT)
      hovered = false;

    if (gPlayButton.buttonSprite == BUTTON_DOWN) { // game started
      Mix_PlayChannel(-1, gMenu[MENU_CLICKED], 0);
      Mix_HaltMusic();
      screen = GAME_SCREEN;
      gPlayButton.buttonSprite = BUTTON_DEFAULT;
      return;
    }

    if(gHelpButton.buttonSprite == BUTTON_DOWN) { // help screen
      Mix_PlayChannel(-1, gMenu[MENU_CLICKED], 0);
      screen = HELP_SCREEN;
      gHelpButton.buttonSprite = BUTTON_DEFAULT;
      return;
    }

    RenderHomeScreen();
  }
}

void Game::RenderHomeScreen(){
  SDL_RenderClear(renderer);
  gHomeScreen.Render(renderer, 0, 0, &gHomeScreenClip);
  gTetrisLogo.Render(renderer, 415, 0, &gTetrisLogoClip);
  gPlayButton.Render(renderer, gPlayButtonTex[gPlayButton.buttonSprite], &gPlayButtonClip);
  gHelpButton.Render(renderer, gHelpButtonTex[gHelpButton.buttonSprite], &gHelpButtonClip);
  SDL_RenderPresent(renderer);
}

void Game::GameScreen(){
  Mix_PlayMusic(gGameBGM, -1);
  GameReset();

  bool GameRunning = true;
  SDL_Event e;

  moveTime = SDL_GetTicks();

  while(GameRunning){
    while(SDL_PollEvent(&e)){
      if(e.type == SDL_QUIT){
        GameRunning = false;
        screen = QUIT_SCREEN;
      }
      if(e.type == SDL_KEYDOWN){
        if(e.key.keysym.sym == SDLK_ESCAPE){ // change to pause screen soon
          GameRunning = false;
          GameReset();
          screen = HOME_SCREEN;
        }
        else if(e.key.keysym.sym == SDLK_COMMA){
          gBoard.Move(LEFT);
          if (gBoard.GroundMoveDelay())
            moveTime = SDL_GetTicks() + MOVE_DELAY;
          else
            moveTime = SDL_GetTicks();
        }
        else if(e.key.keysym.sym == SDLK_PERIOD){
          gBoard.Move(DOWN);
          if (gBoard.GroundMoveDelay())
            moveTime = SDL_GetTicks() + MOVE_DELAY;
          else
            moveTime = SDL_GetTicks();
        }
        else if(e.key.keysym.sym == SDLK_SLASH){
          gBoard.Move(RIGHT);
          if (gBoard.GroundMoveDelay())
            moveTime = SDL_GetTicks() + MOVE_DELAY;
          else
            moveTime = SDL_GetTicks();
        }
        else if(e.key.keysym.sym == SDLK_z){
          gBoard.Rotate(CCW);
          if (gBoard.GroundMoveDelay())
            moveTime = SDL_GetTicks() + MOVE_DELAY;
          else
            moveTime = SDL_GetTicks();
        }
        else if(e.key.keysym.sym == SDLK_x){
          gBoard.Rotate(CW);
          if (gBoard.GroundMoveDelay())
            moveTime = SDL_GetTicks() + MOVE_DELAY;
          else
            moveTime = SDL_GetTicks();
        }
        else if(e.key.keysym.sym == SDLK_a){
          gBoard.Rotate(FLIP);
          if (gBoard.GroundMoveDelay())
            moveTime = SDL_GetTicks() + MOVE_DELAY;
          else
            moveTime = SDL_GetTicks();
        }
        else if(e.key.keysym.sym == SDLK_SPACE){
          gBoard.Move(DROP);
          moveTime = SDL_GetTicks();

          // score calculation function here
          scoreFlag = gBoard.LineClearType();
          gScore.ScoreUpdate(scoreFlag);
          GamePlaySound(scoreFlag);

          gBoard.NextPiece();
        }
        else if(e.key.keysym.sym == SDLK_LSHIFT){
          gBoard.Hold();
          Mix_PlayChannel(-1, gHold, 0);
          moveTime = SDL_GetTicks();
        }
      }
    }

    if(SDL_GetTicks() > moveTime){
      if(gBoard.ForceMove()){
        // score calculate
        scoreFlag = gBoard.LineClearType();
        gScore.ScoreUpdate(scoreFlag);
        GamePlaySound(scoreFlag);

        moveTime += gravityLevel[gScore.GetLevel()];

        gBoard.NextPiece();
      }
      else {
        if (gBoard.GroundMoveDelay())
          moveTime += MOVE_DELAY;
        else
          moveTime += gravityLevel[gScore.GetLevel()];
      }
    }

    if(gBoard.IsEnded()){
      Mix_PlayChannel(-1, gTopOut, 0);
      Mix_HaltMusic();

      GameOverScreen();
      
      GameRunning = false;
      continue;
    }

    RenderGameScreen();
  }
}

void Game::RenderGameScreen(){
  SDL_RenderClear(renderer);

  // draw game screen here
  // draw board
  gBoard.DrawBoard(renderer);
  // draw score
  gScore.Render(renderer);

  SDL_RenderPresent(renderer);
}

void Game::GameOverScreen() {
  bool GameOverRunning = true;
  SDL_Event e;

  while (GameOverRunning) {
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT) {
        GameOverRunning = false;
        screen = QUIT_SCREEN;
      }
      if (e.type == SDL_KEYDOWN) {
        if (e.key.keysym.sym == SDLK_ESCAPE) {
          GameOverRunning = false;
          screen = HOME_SCREEN;
        }
      }
      if (e.type == SDL_MOUSEMOTION || e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP) {
        gRetryButton.HandleEvent(&e);
        gBackGOButton.HandleEvent(&e);
      }
    }
    // play again + back button
    if (gRetryButton.buttonSprite == BUTTON_DOWN) {
      Mix_PlayChannel(-1, gMenu[MENU_CLICKED], 0);
      GameOverRunning = false;
      screen = GAME_SCREEN;
      gRetryButton.buttonSprite = BUTTON_DEFAULT;
    }
    if (gBackGOButton.buttonSprite == BUTTON_DOWN) {
      Mix_PlayChannel(-1, gMenu[MENU_HOVERED], 0);
      GameOverRunning = false;
      screen = HOME_SCREEN;
      gBackGOButton.buttonSprite = BUTTON_DEFAULT;
    }

    RenderGameOverScreen();
  }
}

void Game::RenderGameOverScreen() {
  SDL_RenderClear(renderer);

  gBoard.DrawBoard(renderer);
  gScore.Render(renderer);
  gGameOverScreen.Render(renderer, 0, 0, &gGameOverScreenClip);
  gScore.RenderGameOverScore(renderer);
  gRetryButton.Render(renderer, gRetryButtonTex[gRetryButton.buttonSprite], &gRetryButtonClip);
  gBackGOButton.Render(renderer, gBackGOButtonTex[gBackGOButton.buttonSprite], &gBackGOButtonClip);

  SDL_RenderPresent(renderer);
}

void Game::GamePlaySound(int scoreFlag){
  if(gScore.GetCombo() > 0){
    int tmpCombo = gScore.GetCombo();
    Mix_PlayChannel(-1, gCombo[tmpCombo], 0);
  }
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

void Game::GameReset(){
  gBoard.Reset();
  gScore.Reset();
}

void Game::HelpScreen(){
  bool HelpRunning = true;
  SDL_Event e;

  while(HelpRunning) {
    while (SDL_PollEvent(&e)){
      if(e.type == SDL_QUIT){
        HelpRunning = false;
        screen = QUIT_SCREEN;
      }
      if (e.type == SDL_MOUSEMOTION || e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP) {
        gBackButton.HandleEvent(&e);
      }
      if(e.type == SDL_KEYDOWN){
        if(e.key.keysym.sym == SDLK_ESCAPE){
          HelpRunning = false;
          screen = HOME_SCREEN;
        }
      } 
    }

    if(gBackButton.buttonSprite == BUTTON_DOWN){
      HelpRunning = false;
      Mix_PlayChannel(-1, gMenu[MENU_CLICKED], 0);
      screen = HOME_SCREEN;
      gBackButton.buttonSprite = BUTTON_DEFAULT;
    }

    RenderHelpScreen();
  }

}

void Game::RenderHelpScreen(){
  SDL_RenderClear(renderer);

  gHomeScreen.Render(renderer, 0, 0, &gHomeScreenClip);
  gHelpScreen.Render(renderer, 10, 10, &gHelpScreenClip);
  gBackButton.Render(renderer, gBackButtonTex[gBackButton.buttonSprite], &gBackButtonClip);

  SDL_RenderPresent(renderer);
}

int Game::GetScreen(){
  return screen;
}

Game::Game(){
  srand(time(0));

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    ErrorLog("SDL could not init video. ", SDL_ERROR);
  }
  else if (SDL_Init(SDL_INIT_AUDIO) < 0) {
    ErrorLog("SDL cound not init audio. ", SDL_ERROR);
  }
  else if (!(IMG_Init(IMG_INIT_PNG))) {
    ErrorLog("IMG_Init failed to init. ", IMG_ERROR);
  }
  else if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 2048)) {
    ErrorLog("SDL_mixer failed to init. ", MIX_ERROR);
  }
  else {
    window = SDL_CreateWindow("Tetris", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
      WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 128);  
  }

  screen = HOME_SCREEN;

  // homescreen
  gHomeScreen.LoadTextureFromFile("images/home_screen.png", renderer);
  gTetrisLogo.LoadTextureFromFile("images/tetris_logo.png", renderer);

  gPlayButtonTex[BUTTON_DEFAULT].LoadTextureFromFile("images/play.png", renderer);
  gPlayButtonTex[BUTTON_HOVERED].LoadTextureFromFile("images/play_hovered.png", renderer);
  gHelpButtonTex[BUTTON_DEFAULT].LoadTextureFromFile("images/help.png", renderer);
  gHelpButtonTex[BUTTON_HOVERED].LoadTextureFromFile("images/help_hovered.png", renderer);

  gPlayButton.Init(540, 450, 200, 85);
  gHelpButton.Init(540, 560, 200, 85);

  gHomeScreenBGM = Mix_LoadMUS("music/menu_music_Aerial_City_Chika.mp3");
  
  gMenu[MENU_HOVERED] = Mix_LoadWAV("sfx/menu_hovered.wav");
  gMenu[MENU_CLICKED] = Mix_LoadWAV("sfx/menu_clicked.wav");

  gCombo[1] = Mix_LoadWAV("sfx/combo_1.wav");
  gCombo[2] = Mix_LoadWAV("sfx/combo_2.wav");
  gCombo[3] = Mix_LoadWAV("sfx/combo_3.wav");
  gCombo[4] = Mix_LoadWAV("sfx/combo_4.wav");
  gCombo[5] = Mix_LoadWAV("sfx/combo_5.wav");
  gLineClear = Mix_LoadWAV("sfx/clearline.wav");
  gLineClearQuad = Mix_LoadWAV("sfx/clearquad.wav");
  gLineClearSpin = Mix_LoadWAV("sfx/clearspin.wav");
  // gLineClearAllClear = Mix_LoadWAV("sfx/allclear.wav");
  gHardDrop = Mix_LoadWAV("sfx/harddrop.wav");
  gHold = Mix_LoadWAV("sfx/hold.wav");
  gRotate = Mix_LoadWAV("sfx/rotate.wav");
  gTopOut = Mix_LoadWAV("sfx/topout.wav");

  // gameover screen
  gGameOverScreen.LoadTextureFromFile("images/gameover_screen.png", renderer);
  gRetryButtonTex[BUTTON_DEFAULT].LoadTextureFromFile("images/retry.png", renderer);
  gRetryButtonTex[BUTTON_HOVERED].LoadTextureFromFile("images/retry_hovered.png", renderer);
  gBackGOButtonTex[BUTTON_DEFAULT].LoadTextureFromFile("images/backgo.png", renderer);
  gBackGOButtonTex[BUTTON_HOVERED].LoadTextureFromFile("images/backgo_hovered.png", renderer);

  gRetryButton.Init(540, 400, 200, 85);
  gBackGOButton.Init(540, 510, 200, 85);

  // playscreen
  gBoard.TextureInit(renderer);
  gScore.TextureInit(renderer);
  gGameBGM = Mix_LoadMUS("music/game_music_Wind_Trail_Chika.mp3");

  // helpscreen
  gHelpScreen.LoadTextureFromFile("images/help_screen.png", renderer);
  gBackButtonTex[BUTTON_DEFAULT].LoadTextureFromFile("images/back.png", renderer);
  gBackButtonTex[BUTTON_HOVERED].LoadTextureFromFile("images/back_hovered.png", renderer);

  gBackButton.Init(25, 25, 150, 50);
}

Game::~Game(){
  // homescreen
  gHomeScreen.FreeTexture();
  gTetrisLogo.FreeTexture();
  for(int i = 0; i < BUTTON_TOTAL; i ++)
    gPlayButtonTex[i].FreeTexture(),
    gHelpButtonTex[i].FreeTexture(),
    gBackButtonTex[i].FreeTexture();
  
  Mix_FreeMusic(gHomeScreenBGM);
  for (int i = 0; i < TOTAL_MENU_SFX; i++)
    Mix_FreeChunk(gMenu[i]);
  for (int i = 1; i < 6; i++)
    Mix_FreeChunk(gCombo[i]);
  Mix_FreeChunk(gLineClear);
  Mix_FreeChunk(gLineClearQuad);
  //Mix_FreeChunk(gLineClearSpin);
  //Mix_FreeChunk(gLineClearAllClear);
  Mix_FreeChunk(gHardDrop);
  Mix_FreeChunk(gHold);
  Mix_FreeChunk(gRotate);
  Mix_FreeChunk(gTopOut);

  // playscreen
  gGameOverScreen.FreeTexture();
  for (int i = 0; i < BUTTON_TOTAL; i++)
    gRetryButtonTex[i].FreeTexture(),
    gBackGOButtonTex[i].FreeTexture();

  Mix_FreeMusic(gGameBGM);

  // helpscreen
  gHelpScreen.FreeTexture();

  SDL_DestroyWindow(window);
  window = NULL;
  SDL_DestroyRenderer(renderer);
  renderer = NULL;
}