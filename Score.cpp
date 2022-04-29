#include "Score.h"

Score::Score(){
  score = 0;
  combo = -1;
  btbValue = 1;
  level = 1;
  lineCleared = 0;
}

Score::~Score(){
  
}

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
  if(scoreType == DROP || scoreType == T_SPIN) {
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

void Score::RenderScore(SDL_Renderer* renderer, Texture number[]) {
  SDL_Rect numClip = {0, 0, 30, 40};
  int tmp = score;
  int x = 175, y = 0;
  for (int i = 0; i < 6; i++) {
    int num = tmp % 10;
    number[num].Render(renderer, x, y, &numClip);
    x -= NUMBER_WIDTH + 5;
    tmp /= 10;
  }
}

void Score::RenderLevel(SDL_Renderer* renderer, Texture number[]) {
  SDL_Rect numClip = { 0, 0, 30, 40 };
  int tmp = level;
  int x = 35, y = 50;
  for (int i = 0; i < 2; i++) {
    int num = tmp % 10;
    number[num].Render(renderer, x, y, &numClip);
    x -= NUMBER_WIDTH + 5;
    tmp /= 10;
  }
}

void Score::RenderLineCleared(SDL_Renderer* renderer, Texture number[]) {
  SDL_Rect numClip = { 0, 0, 30, 40 };
  int tmp = lineCleared;
  int x = 70, y = 100;
  for (int i = 0; i < 3; i++) {
    int num = tmp % 10;
    number[num].Render(renderer, x, y, &numClip);
    x -= NUMBER_WIDTH + 5;
    tmp /= 10;
  }
}

void Score::RenderCombo(SDL_Renderer* renderer, Texture number[]){
  SDL_Rect numClip = { 0, 0, 30, 40 };
  int tmp = combo;
  int x = 0, y = 150;
  for (int i = 0; i < 1; i++) {
    int num = tmp % 10;
    number[num].Render(renderer, x, y, &numClip);
    x -= NUMBER_WIDTH + 5;
    tmp /= 10;
  }
}

int Score::GetLevel(){
  return level;
}

int Score::GetCombo() {
  return combo;
}
