#ifndef BOARD_H
#define BOARD_H

#include "GameBase.h"
#include "Pieces.h"
#include "Texture.h"

class Board {
public:
	Board();
	~Board();

	bool IsEnded();

	void Reset();
	void DeleteRow(int row);
	int DeletePosibleRow();
	bool IsPosibleMove(Piece p);
	void MergePiece(Piece p);
	bool TSpinDetection(Piece p);
	int LineClearType(Piece p);

	void DrawBoard(SDL_Renderer* renderer, Texture gBlock[]);

private:
	int board[BOARD_HEIGHT][BOARD_WIDTH];
};

#endif