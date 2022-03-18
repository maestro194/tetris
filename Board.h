#ifndef BOARD_H
#define BOARD_H

#include "GameBase.h"
#include "Pieces.h"

class Board {
public:
	//Board();
	//~Board();

	bool IsEnded();

	void DeleteRow(int row);
	void DeletePosibleRow();
	bool IsPosibleMove(Piece p);

	void DrawBoard(SDL_Renderer* renderer);

private:
	int CurrentBoard[BOARD_WIDTH][BOARD_HEIGHT];
};

#endif