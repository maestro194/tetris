#include "Score.h"

void Score::Reset(){
  score = 0;
  combo = -1;
  btbValue = 1;
  level = 1;
  lineCleared = 0;
}

int Score::DigitCount(){
  int cnt = 0, x = score;
  if(score == 0)
    return 1;
  while(score > 0)
    cnt ++, x /= 10;
  return cnt;
}

void Score::ScoreUpdate(int scoreType){
  if(scoreType == HARD_DROP || scoreType == T_SPIN) {
    combo = -1;
    btbValue = 1;
    score += scoring[scoreType];
  }
  else if(scoreType == SINGLE || scoreType == DOUBLE || scoreType == TRIPLE){
    combo = min(5, combo + 1);
    btbValue = 1;
    lineCleared += scoreType;
    if (lineCleared >= levelLineClear[level])
      level ++;
    score += (scoring[scoreType] * btbValue + 50 * combo) * level;
  }
  else if (scoreType == QUAD) {
    combo = min(5, combo + 1);
    lineCleared += scoreType;
    if (lineCleared >= levelLineClear[level])
      level++;
    score += (scoring[scoreType] * btbValue + 50 * combo) * level;
    btbValue = 2;
  }
  else{
    combo = min(5, combo + 1);
    lineCleared += scoreType - 5;
    if (lineCleared >= levelLineClear[level])
      level ++;
    score += (scoring[scoreType] * btbValue + 50 * combo) * level;
    btbValue = 2;
  }
}

void Score::RenderScore(SDL_Renderer* renderer) {
  SDL_Rect numClip = {0, 0, 30, 40};
  int tmp = score;
  int x = 150, y = 0;
  for (int i = 0; i < 6; i++) {
    int num = tmp % 10;
    number[num].Render(renderer, x, y, &numClip);
    x -= NUMBER_WIDTH;
    tmp /= 10;
  }
}

void Score::RenderLevel(SDL_Renderer* renderer) {
  SDL_Rect numClip = { 0, 0, 30, 40 };
  int tmp = level;
  int x = 30, y = 50;
  for (int i = 0; i < 2; i++) {
    int num = tmp % 10;
    number[num].Render(renderer, x, y, &numClip);
    x -= NUMBER_WIDTH;
    tmp /= 10;
  }
}

void Score::RenderLineCleared(SDL_Renderer* renderer) {
  SDL_Rect numClip = { 0, 0, 30, 40 };
  int tmp = lineCleared;
  int x = 60, y = 100;
  for (int i = 0; i < 3; i++) {
    int num = tmp % 10;
    number[num].Render(renderer, x, y, &numClip);
    x -= NUMBER_WIDTH;
    tmp /= 10;
  }
}

void Score::RenderCombo(SDL_Renderer* renderer){
  SDL_Rect numClip = { 0, 0, 30, 40 };
  int tmp = combo;
  int x = 30, y = 150;
  number[NUM_x].Render(renderer, x, y, &numClip);
  x -= NUMBER_WIDTH + 3;
  for (int i = 0; i < 1; i++) {
    int num = tmp % 10;
    number[num].Render(renderer, x, y, &numClip);
    x -= NUMBER_WIDTH + 3;
    tmp /= 10;
  }
}

void Score::Render(SDL_Renderer* renderer){
  RenderScore(renderer);
  RenderLevel(renderer);
  RenderLineCleared(renderer);
  if(combo > 0)
  RenderCombo(renderer);
}

int Score::GetLevel(){
  return level;
}

int Score::GetCombo() {
  return combo;
}

void Score::TextureInit(SDL_Renderer* renderer){
  number[0].LoadTextureFromFile("images/combo-0.png", renderer);
  number[1].LoadTextureFromFile("images/combo-1.png", renderer);
  number[2].LoadTextureFromFile("images/combo-2.png", renderer);
  number[3].LoadTextureFromFile("images/combo-3.png", renderer);
  number[4].LoadTextureFromFile("images/combo-4.png", renderer);
  number[5].LoadTextureFromFile("images/combo-5.png", renderer);
  number[6].LoadTextureFromFile("images/combo-6.png", renderer);
  number[7].LoadTextureFromFile("images/combo-7.png", renderer);
  number[8].LoadTextureFromFile("images/combo-8.png", renderer);
  number[9].LoadTextureFromFile("images/combo-9.png", renderer);
  number[10].LoadTextureFromFile("images/combo-x.png", renderer);
}

Score::Score(){
  score = 0;
  combo = -1;
  btbValue = 1;
  level = 1;
  lineCleared = 0;
}

Score::~Score(){
  score = 0;
  combo = -1;
  btbValue = 1;
  level = 1;
  lineCleared = 0;

  for (int i = 0; i < TOTAL_NUMBER; i ++)
    number[i].FreeTexture();
}