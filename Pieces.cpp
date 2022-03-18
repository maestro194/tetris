#include "Pieces.h"
#include "GameBase.h"

Piece::Piece(int pieceNumber) {
  rotation = 0;
  for(int i = 0; i < 4; i ++)
    for(int j = 0; j < 4; j ++)
      for(int k = 0; k < 4; k ++)
        shape[i][j][k] = ' ';
  switch (pieceNumber)
  {
  case O_PIECE:
    pieceType = O_PIECE;
    shape[0][0][1] = shape[0][0][2] = shape[0][1][1] = shape[0][1][2] = '*';
    shape[1][0][1] = shape[1][0][2] = shape[1][1][1] = shape[1][1][2] = '*';
    shape[2][0][1] = shape[2][0][2] = shape[2][1][1] = shape[2][1][2] = '*';
    shape[3][0][1] = shape[3][0][2] = shape[3][1][1] = shape[3][1][2] = '*';
    break;
  case L_PIECE:
    pieceType = L_PIECE;
    shape[0][0][2] = shape[0][1][0] = shape[0][1][1] = shape[0][1][2] = '*';
    shape[1][0][1] = shape[1][1][1] = shape[1][2][1] = shape[1][2][2] = '*';
    shape[2][1][0] = shape[2][1][1] = shape[2][1][2] = shape[2][2][0] = '*';
    shape[3][0][0] = shape[3][0][1] = shape[3][1][1] = shape[3][2][1] = '*';
    break;
  case J_PIECE:
    pieceType = J_PIECE;
    shape[0][0][0] = shape[0][1][0] = shape[0][1][1] = shape[0][1][2] = '*';
    shape[1][0][1] = shape[1][0][2] = shape[1][1][1] = shape[1][2][1] = '*';
    shape[2][1][0] = shape[2][1][1] = shape[2][1][2] = shape[2][2][2] = '*';
    shape[3][0][1] = shape[3][1][1] = shape[3][2][0] = shape[3][2][1] = '*';
    break;
  case I_PIECE:
    pieceType = I_PIECE;
    shape[0][1][0] = shape[0][1][1] = shape[0][1][2] = shape[0][1][3] = '*';
    shape[1][0][2] = shape[1][1][2] = shape[1][2][2] = shape[1][3][2] = '*';
    shape[2][2][0] = shape[2][2][1] = shape[2][2][2] = shape[2][2][3] = '*';
    shape[3][1][0] = shape[3][1][1] = shape[3][1][2] = shape[3][1][3] = '*';
    break;
  case Z_PIECE:
    pieceType = Z_PIECE;
    shape[0][0][0] = shape[0][0][1] = shape[0][1][1] = shape[0][1][2] = '*';
    shape[1][0][2] = shape[1][1][1] = shape[1][1][2] = shape[1][2][1] = '*';
    shape[2][1][0] = shape[2][1][1] = shape[2][2][1] = shape[2][2][2] = '*';
    shape[3][0][1] = shape[3][1][0] = shape[3][1][1] = shape[3][2][0] = '*';
    break;
  case S_PIECE:
    pieceType = S_PIECE;
    shape[0][0][1] = shape[0][0][2] = shape[0][1][0] = shape[0][1][1] = '*';
    shape[1][0][1] = shape[1][1][1] = shape[1][1][2] = shape[1][2][2] = '*';
    shape[2][1][1] = shape[2][1][2] = shape[2][2][0] = shape[2][2][1] = '*';
    shape[3][0][0] = shape[3][1][0] = shape[3][1][1] = shape[3][2][1] = '*';
    break;
  case T_PIECE:
    pieceType = T_PIECE;
    shape[0][0][1] = shape[0][1][0] = shape[0][1][1] = shape[0][1][2] = '*';
    shape[1][0][1] = shape[1][1][1] = shape[1][1][2] = shape[1][2][1] = '*';
    shape[2][1][0] = shape[2][1][1] = shape[2][1][2] = shape[2][2][1] = '*';
    shape[3][0][1] = shape[3][1][0] = shape[3][1][1] = shape[3][2][1] = '*';
    break;
  }
}

void Piece::PieceLeftMove(){

}

void Piece::PieceRightMove(){

}

void Piece::PieceDownMove(){

}

void Piece::PieceDropMove(){

}

void Piece::PieceCWRotateMove(){

}

void Piece::PieceCCWRotateMove(){

}

void Piece::PieceFlipMove(){

}

Piece::~Piece() {

}