#ifndef SCORE_H
#define SCORE_H

#include "GameBase.h"
#include "Texture.h"

class Score {
public:
  Score();
  ~Score();

  void Reset();
  int DigitCount();
  void ScoreUpdate(int scoreType);
  void RenderScore();
  
private:
  int score;
  int combo;
  int btbValue;
  int level;
  int lineCleared;
  int scoring[TOTAL_SCORING_TYPE] = {0, 100, 300, 500, 800, 400, 800, 1200, 1600, 3500};
  int levelLineClear[15];
};

#endif