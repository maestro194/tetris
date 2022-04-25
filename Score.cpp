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
    combo = max(5, combo + 1);
    btbValue = 1;
    lineCleared += scoreType;
    if(levelLineClear[level] <= lineCleared)
      level ++;
    score += (scoring[scoreType] * btbValue + 50 * combo) * level;
  }
  else{
    combo = max(5, combo + 1);
    lineCleared += scoreType - 5;
    if(levelLineClear[level] <= lineCleared)
      level ++;
    score += (scoring[scoreType] * btbValue + 50 * combo) * level;
    btbValue = 2;
  }
}

void Score::RenderScore(){
  // render later when using number
  cerr << score << '\n';
}

void Score::RenderLevel(){
  // render later when using number
  cerr << level << '\n';
}

void Score::RenderCombo(){
  // render later when using number
  cerr << combo << '\n';
}