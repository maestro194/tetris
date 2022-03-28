#ifndef PIECES_H
#define PIECES_H

#include "GameBase.h"
#include "Texture.h"

class Piece {
public:
	Piece();
	~Piece();

	void Init(int pieceNumber);

	void PieceLeftMove();
	void PieceRightMove();
	void PieceUpMove();
	void PieceDownMove();
	void PieceDropMove();
	void PieceCWRotateMove(int x, int y);
	void PieceCCWRotateMove(int x, int y);
	void PieceFlipMove();

	void DrawPiece(SDL_Renderer* renderer, Texture gBlock[]);

	int pieceType;
	int rotation;
	int xOffSet;
	int yOffSet;
	bool shape[4][4][4];
private:
	
};

#endif