#ifndef PIECES_H
#define PIECES_H

#include "GameBase.h"

class Pieces {
public:
	Pieces(int pieceNumber);
	~Pieces();

private:
	int boardPos[4][2];
	int pieceType;
};

#endif