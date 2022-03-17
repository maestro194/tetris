#include "Pieces.h"

Pieces::Pieces(int pieceNumber) {
  switch (pieceNumber)
  {
  case O_PIECE:
    pieceType = O_PIECE;
    boardPos[0][0] = 1, boardPos[0][1] = 5;
    boardPos[1][0] = 1, boardPos[1][1] = 6;
    boardPos[2][0] = 2, boardPos[2][1] = 5;
    boardPos[3][0] = 2, boardPos[3][1] = 6;
    break;
  case L_PIECE:
    pieceType = L_PIECE;
    boardPos[0][0] = 1, boardPos[0][1] = 6;
    boardPos[1][0] = 2, boardPos[1][1] = 4;
    boardPos[2][0] = 2, boardPos[2][1] = 5;
    boardPos[3][0] = 2, boardPos[3][1] = 6;
    break;
  case REV_L_PIECE:
    pieceType = REV_L_PIECE;
    boardPos[0][0] = 1, boardPos[0][1] = 4;
    boardPos[1][0] = 2, boardPos[1][1] = 4;
    boardPos[2][0] = 2, boardPos[2][1] = 5;
    boardPos[3][0] = 2, boardPos[3][1] = 6;
    break;
  case I_PIECE:
    pieceType = I_PIECE;
    boardPos[0][0] = 2, boardPos[0][1] = 4;
    boardPos[1][0] = 2, boardPos[1][1] = 5;
    boardPos[2][0] = 2, boardPos[2][1] = 6;
    boardPos[3][0] = 2, boardPos[3][1] = 7;
    break;
  case Z_PIECE:
    pieceType = Z_PIECE;
    boardPos[0][0] = 1, boardPos[0][1] = 4;
    boardPos[1][0] = 1, boardPos[1][1] = 5;
    boardPos[2][0] = 2, boardPos[2][1] = 5;
    boardPos[3][0] = 2, boardPos[3][1] = 6;
    break;
  case S_PIECE:
    pieceType = S_PIECE;
    boardPos[0][0] = 1, boardPos[0][1] = 5;
    boardPos[1][0] = 1, boardPos[1][1] = 6;
    boardPos[2][0] = 2, boardPos[2][1] = 4;
    boardPos[3][0] = 2, boardPos[3][1] = 5;
    break;
  case T_PIECE:
    pieceType = T_PIECE;
    boardPos[0][0] = 1, boardPos[0][1] = 5;
    boardPos[1][0] = 2, boardPos[1][1] = 4;
    boardPos[2][0] = 2, boardPos[2][1] = 5;
    boardPos[3][0] = 2, boardPos[3][1] = 6;
    break;

  }
}

Pieces::~Pieces() {

}