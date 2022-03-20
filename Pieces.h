#ifndef PIECES_H
#define PIECES_H

#include "GameBase.h"

class Piece {
public:
	Piece(int pieceNumber);
	~Piece();

	void PieceLeftMove();
	void PieceRightMove();
	void PieceDownMove();
	void PieceDropMove();
	void PieceCWRotateMove();
	void PieceCCWRotateMove();
	void PieceFlipMove();

private:
	char shape[4][4][4];
	int pieceType;
	int rotation;
};

#endif