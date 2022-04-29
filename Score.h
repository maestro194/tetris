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
  void RenderScore(SDL_Renderer* renderer);
  void RenderLevel(SDL_Renderer* renderer);
  void RenderLineCleared(SDL_Renderer* renderer);
  void RenderCombo(SDL_Renderer* renderer);
  void Render(SDL_Renderer* renderer);
  int GetLevel();
  int GetCombo();

  void TextureInit(SDL_Renderer* renderer);

private:
  int score;
  int combo;
  int btbValue;
  int level;
  int lineCleared;

  Texture number[TOTAL_NUMBER];
  int scoring[TOTAL_SCORING_TYPE] = {0, 100, 300, 500, 800, 400, 800, 1200, 1600, 3500};
  int levelLineClear[16] = {0, 3, 8, 15, 24, 35, 48, 63, 80, 99, 120, 144, 170, 198, 228, 260};
};

#endif
