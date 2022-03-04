#ifndef PIECES_H
#define PIECES_H

#include "GameBase.h"

class Pieces {
public:
	int GetBlockType(int piece, int rotation, int x, int y);
	int GetXInitPos(int piece, int rotation);
	int GetYInitPos(int piece, int rotation);
private:
	Pieces();
	~Pieces();
};

#endif